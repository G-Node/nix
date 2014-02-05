// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include <nix/hdf5/SectionHDF5.hpp>
#include <nix/hdf5/PropertyHDF5.hpp>

using namespace std;

namespace nix {
namespace hdf5 {


SectionHDF5::SectionHDF5(const SectionHDF5 &section)
    : NamedEntityHDF5(section.file(), section.group(), section.id())
{
    property_group = section.property_group;
    section_group = section.section_group;
}


SectionHDF5::SectionHDF5(const File &file, const Group &group, const string &id)
    : NamedEntityHDF5(file, group, id)
{
    property_group = this->group().openGroup("properties");
    section_group = this->group().openGroup("sections");
}


SectionHDF5::SectionHDF5(const File &file, const Group &group, const string &id, time_t time)
    : NamedEntityHDF5(file, group, id, time)
{
    property_group = this->group().openGroup("properties");
    section_group = this->group().openGroup("sections");
}

//--------------------------------------------------
// Attribute getter and setter
//--------------------------------------------------

void SectionHDF5::repository(const string &repository) {
    group().setAttr("repository", repository);
}


string SectionHDF5::repository() const {
    string repository;
    group().getAttr("repository", repository);
    return repository;
}


void SectionHDF5::link(const string &link) {
    // TODO check existence and type of the linked section
    group().setAttr("link", link);
}


string SectionHDF5::link() const {
    string link;
    group().getAttr("link", link);
    return link;
}


void SectionHDF5::mapping(const string &mapping) {
    group().setAttr("mapping", mapping);
}


string SectionHDF5::mapping() const {
    string mapping;
    group().getAttr("mapping", mapping);
    return mapping;
}

//--------------------------------------------------
// Methods for parent access
//--------------------------------------------------

Section SectionHDF5::parent() const {
    // TODO implement
    return Section();
}


bool SectionHDF5::hasParent() const {
    // TODO implement
    return false;
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


Section SectionHDF5::getSection(const string &id) const {
    Group g = section_group.openGroup(id, false);

    shared_ptr<SectionHDF5> tmp(new SectionHDF5(file(), g, id));
    return Section(tmp);
}


Section SectionHDF5::getSection(size_t index) const {
    string id = section_group.objectName(index);

    return getSection(id);
}


vector<Section> SectionHDF5::sections() const {
    vector<Section>  secs;

    size_t section_count = section_group.objectCount();
    for (size_t i = 0; i < section_count; i++) {
        string id = section_group.objectName(i);
        Group grp = section_group.openGroup(id, false);

        shared_ptr<SectionHDF5> tmp(new SectionHDF5(file(), grp, id));
        secs.push_back(Section(tmp));
    }

    return secs;
}


Section SectionHDF5::createSection(const string &name, const string &type) {
    string new_id = util::createId("section");

    while (section_group.hasObject(new_id)) {
        new_id = util::createId("section");
    }

    Group g = section_group.openGroup(new_id, true);
    shared_ptr<SectionHDF5> tmp(new SectionHDF5(file(), g, new_id));
    tmp->name(name);
    tmp->type(type);

    return Section(tmp);
}


bool SectionHDF5::removeSection(const string &id) {
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


vector<Property> SectionHDF5::properties() const {
    vector<Property> props;

    for (size_t i = 0; i < propertyCount(); i++){
        string id = property_group.objectName(i);
        Group g = property_group.openGroup(id,false);

        shared_ptr<PropertyHDF5> tmp(new PropertyHDF5(file(), g, id));
        props.push_back(Property(tmp));
    }

    return props;
}


Property SectionHDF5::getProperty(const string &id) const {
    if (property_group.hasGroup(id)) {
        Group g = property_group.openGroup(id,false);

        shared_ptr<PropertyHDF5> tmp(new PropertyHDF5(file(), g, id));
        return Property(tmp);
    } else {
        throw runtime_error("Requested Property does not exist! Always check with hasProperty!");
    }
}


Property SectionHDF5::createProperty(const string &name) {
    // check for existing property name
    //if (hasPropertyByName(name)) {
    //    throw runtime_error("Attempt to add a property that already exists!");
    //}
    string new_id = util::createId("property");
    while (property_group.hasObject(new_id))
        new_id = util::createId("property");

    Group g = property_group.openGroup(new_id, true);

    shared_ptr<PropertyHDF5> tmp(new PropertyHDF5(file(), g, new_id));
    tmp->name(name);

    return Property(tmp);
}


bool SectionHDF5::removeProperty(const string &id) {
    if (property_group.hasObject(id)) {
        property_group.removeGroup(id);
        return true;
    } else {
        return false;
    }
}


size_t SectionHDF5::propertyCount() const {
    return property_group.objectCount();
}


bool SectionHDF5::hasProperty(const string &id) const {
    return property_group.hasGroup(id);
}



SectionHDF5::~SectionHDF5() {}


} // namespace hdf5
} // namespace nix
