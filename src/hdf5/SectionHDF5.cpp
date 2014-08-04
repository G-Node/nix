// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include <nix/util/filter.hpp>
#include <nix/hdf5/SectionHDF5.hpp>
#include <nix/hdf5/PropertyHDF5.hpp>

using namespace std;
using namespace nix;
using namespace nix::base;
using namespace nix::hdf5;


SectionHDF5::SectionHDF5(const SectionHDF5 &section)
    : NamedEntityHDF5(section.file(), section.group(), section.id(), section.type(), section.name())
{
    property_group = section.property_group;
    section_group = section.section_group;
}


SectionHDF5::SectionHDF5(const File &file, const Group &group, const string &id,
                         const string &type, const string &name)
    : SectionHDF5(file, nullptr, group, id, type, name)
{
}


SectionHDF5::SectionHDF5(const File &file, const Section &parent, const Group &group,
                         const string &id, const string &type, const string &name)
    : SectionHDF5(file, parent, group, id, type, name, util::getTime())
{
}


SectionHDF5::SectionHDF5(const File &file, const Group &group, const string &id,
                         const string &type, const string &name, time_t time)
    : SectionHDF5(file, nullptr, group, id, type, name, time)
{
}


SectionHDF5::SectionHDF5(const File &file, const Section &parent, const Group &group,
                         const string &id, const string &type, const string &name, time_t time)
    : NamedEntityHDF5(file, group, id, type, name, time), parent_section(parent)
{
    property_group = this->group().openGroup("properties");
    section_group = this->group().openGroup("sections");
}

//--------------------------------------------------
// Attribute getter and setter
//--------------------------------------------------

void SectionHDF5::repository(const string &repository) {
    if(repository.empty()) {
        throw EmptyString("repository");
    } else {
        group().setAttr("repository", repository);
        forceUpdatedAt();
    }
}


boost::optional<string> SectionHDF5::repository() const {
    boost::optional<string> ret;
    string repository;
    if(group().getAttr("repository", repository)) {
        ret = repository;
    }
    return ret;
}


void SectionHDF5::repository(const none_t t) {
    if(group().hasAttr("repository")) {
        group().removeAttr("repository");
    }
    forceUpdatedAt();
}


void SectionHDF5::link(const std::string &id) {
    if(id.empty()) {
        throw EmptyString("mapping");
    } else if (file().hasSection(id)) {
        group().setAttr("link", id);
    } else {
        throw std::runtime_error("Section not found in file!");
    }
}


shared_ptr<ISection> SectionHDF5::link() const {
    shared_ptr<ISection> sec;

    string id;
    group().getAttr("link", id);

    vector<Section> found;
    if (id != "") {
        found = file().findSections(util::IdFilter<Section>(id));
    }

    if (found.size() > 0) {
        sec = found[0].impl(); // TODO fix this when base entities are fixed
    }

    return sec;
}


void SectionHDF5::link(const none_t t) {
    if(group().hasAttr("link")) {
        group().removeAttr("link");
    }
    forceUpdatedAt();
}


void SectionHDF5::mapping(const string &mapping) {
    if(mapping.empty()) {
        throw EmptyString("mapping");
    } else {
        group().setAttr("mapping", mapping);
        forceUpdatedAt();
    }
}


boost::optional<string> SectionHDF5::mapping() const {
    boost::optional<string> ret;
    string mapping;
    if(group().getAttr("mapping", mapping)) {
        ret = mapping;
    }
    return ret;
}


void SectionHDF5::mapping(const none_t t) {
    if(group().hasAttr("mapping")) {
        group().removeAttr("mapping");
    }
    forceUpdatedAt();
}

//--------------------------------------------------
// Methods for parent access
//--------------------------------------------------


shared_ptr<ISection> SectionHDF5::parent() const {
    // TODO fix this when base entities are fixed
    return parent_section.impl();
}


//--------------------------------------------------
// Methods for child section access
//--------------------------------------------------


size_t SectionHDF5::sectionCount() const {
    return section_group.objectCount();
}


bool SectionHDF5::hasSection(const string &id) const {
    return section_group.hasGroup(id);
}


shared_ptr<ISection> SectionHDF5::getSection(const string &id) const {
    shared_ptr<ISection> sec;

    if (section_group.hasGroup(id)) {
        Group group = section_group.openGroup(id, false);
        std::string type;
        std::string name;
        group.getAttr("type", type);
        group.getAttr("name", name);

        Section parent(const_pointer_cast<SectionHDF5>(shared_from_this()));
        sec = make_shared<SectionHDF5>(file(), parent, group, id, type, name);
    }

    return sec;
}


shared_ptr<ISection> SectionHDF5::getSection(size_t index) const {
    string id = section_group.objectName(index);

    return getSection(id);
}


shared_ptr<ISection> SectionHDF5::createSection(const string &name, const string &type) {
    string new_id = util::createId("section");
    while (section_group.hasObject(new_id)) {
        new_id = util::createId("section");
    }

    Section parent(const_pointer_cast<SectionHDF5>(shared_from_this()));

    Group grp = section_group.openGroup(new_id, true);
    return make_shared<SectionHDF5>(file(), parent, grp, new_id, type, name);
}


bool SectionHDF5::deleteSection(const string &id) {
    if (section_group.hasGroup(id)) {
        section_group.removeGroup(id);
        return true;
    } else {
        return false;
    }
}


//--------------------------------------------------
// Methods for property access
//--------------------------------------------------


size_t SectionHDF5::propertyCount() const {
    return property_group.objectCount();
}


bool SectionHDF5::hasProperty(const string &id) const {
    return property_group.hasObject(id);
}


shared_ptr<IProperty> SectionHDF5::getProperty(const string &id) const {
    shared_ptr<IProperty> prop;

    if (property_group.hasData(id)) {
        DataSet dset = property_group.openData(id);
        string name;
        dset.getAttr("name", name);
        prop = make_shared<PropertyHDF5>(file(), property_group, dset, id, name);
    }

    return prop;
}


shared_ptr<IProperty> SectionHDF5::getProperty(size_t index) const {
    string id = property_group.objectName(index);

    return getProperty(id);
}


bool SectionHDF5::hasPropertyWithName(const string &name) const {
    bool found = false;

    // TODO could be implemented using Section::properties
    for (size_t i = 0; i < propertyCount() && !found; i++) {
        string id = property_group.objectName(i);
        DataSet dset = property_group.openData(id);

        string other_name;
        dset.getAttr("name", other_name);

        if (other_name == name) {
            found = true;
        }
    }
    return found;
}


shared_ptr<IProperty> SectionHDF5::getPropertyByName(const string &name) const {
    shared_ptr<IProperty> prop;

    // TODO could be implemented using Section::properties
    for (size_t i = 0; i < propertyCount() && !prop; i++) {
        string id = property_group.objectName(i);
        DataSet dset = property_group.openData(id);

        string other_name;
        dset.getAttr("name", other_name);

        if (other_name == name) {
            prop = make_shared<PropertyHDF5>(file(), property_group, dset, id, name);
        }
    }

    return prop;
}


shared_ptr<IProperty> SectionHDF5::createProperty(const string &name, const DataType &dtype) {
    if (hasPropertyWithName(name))
        throw runtime_error("Try to create a property with existing name: " + name);

    string new_id = util::createId("property");
    while (property_group.hasData(new_id))
        new_id = util::createId("property");

    H5::DataType fileType = DataSet::fileTypeForValue(dtype);
    DataSet dataset = DataSet::create(property_group.h5Group(), new_id, fileType, {0});

    return make_shared<PropertyHDF5>(file(), property_group, dataset, new_id, name);
}


shared_ptr<IProperty> SectionHDF5::createProperty(const string &name, const Value &value) {
    shared_ptr<IProperty> p = createProperty(name, value.type());

    vector<Value> val{value};
    p->values(val);

    return p;
}


shared_ptr<IProperty> SectionHDF5::createProperty(const string &name, const vector<Value> &values) {
    if (values.size() < 1)
        throw runtime_error("Trying to create a property without a value!");

    shared_ptr<IProperty> p = createProperty(name, values[0].type());
    p->values(values);

    return p;
}


bool SectionHDF5::deleteProperty(const string &id) {
    if (property_group.hasData(id)) {
        property_group.removeData(id);
        return true;
    } else {
        return false;
    }
}


SectionHDF5::~SectionHDF5() {}
