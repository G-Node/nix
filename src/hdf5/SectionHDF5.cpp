// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include <nix/util/filter.hpp>
#include <nix/File.hpp>
#include <nix/Section.hpp>
#include <nix/hdf5/SectionHDF5.hpp>
#include <nix/hdf5/PropertyHDF5.hpp>

using namespace std;
using namespace nix;
using namespace nix::base;
using namespace nix::hdf5;


SectionHDF5::SectionHDF5(std::shared_ptr<base::IFile> file, const Group &group)
    : SectionHDF5(file, nullptr, group)
{
}
    

SectionHDF5::SectionHDF5(std::shared_ptr<base::IFile> file, std::shared_ptr<base::ISection> parent, const Group &group)
    : NamedEntityHDF5(file, group), parent_section(parent)
{
    property_group = this->group().openOptGroup("properties");
    section_group = this->group().openOptGroup("sections");
}


SectionHDF5::SectionHDF5(shared_ptr<IFile> file, const Group &group, const string &id,
                         const string &type, const string &name)
    : SectionHDF5(file, nullptr, group, id, type, name)
{
}


SectionHDF5::SectionHDF5(shared_ptr<IFile> file, shared_ptr<ISection> parent, const Group &group,
                         const string &id, const string &type, const string &name)
    : SectionHDF5(file, parent, group, id, type, name, util::getTime())
{
}


SectionHDF5::SectionHDF5(shared_ptr<IFile> file, const Group &group, const string &id,
                         const string &type, const string &name, time_t time)
    : SectionHDF5(file, nullptr, group, id, type, name, time)
{
}


SectionHDF5::SectionHDF5(shared_ptr<IFile> file, shared_ptr<ISection> parent, const Group &group,
                         const string &id, const string &type, const string &name, time_t time)
    : NamedEntityHDF5(file, group, id, type, name, time), parent_section(parent)
{
    property_group = this->group().openOptGroup("properties");
    section_group = this->group().openOptGroup("sections");
}

//--------------------------------------------------
// Attribute getter and setter
//--------------------------------------------------

void SectionHDF5::repository(const string &repository) {
    if (repository.empty()) {
        throw EmptyString("repository");
    } else {
        group().setAttr("repository", repository);
        forceUpdatedAt();
    }
}


boost::optional<string> SectionHDF5::repository() const {
    boost::optional<string> ret;
    string repository;
    if (group().getAttr("repository", repository)) {
        ret = repository;
    }
    return ret;
}


void SectionHDF5::repository(const none_t t) {
    if (group().hasAttr("repository")) {
        group().removeAttr("repository");
    }
    forceUpdatedAt();
}


void SectionHDF5::link(const std::string &id) {
    if (id.empty())
        throw EmptyString("link");

    if (group().hasGroup("link"))
        link(none);
        
    File tmp = file();
    auto found = tmp.findSections(util::IdFilter<Section>(id));
    if (found.empty())
        throw std::runtime_error("SectionHDF5::link: Section not found in file!");
    
    auto target = dynamic_pointer_cast<SectionHDF5>(found.front().impl());

    group().createLink(target->group(), "link");
}


shared_ptr<ISection> SectionHDF5::link() const {
    shared_ptr<ISection> sec;

    if (group().hasGroup("link")) {
        Group other_group = group().openGroup("link", false);
        auto sec_tmp = make_shared<SectionHDF5>(file(), other_group);
        // re-get above section "sec_tmp": parent missing, findSections will set it!
        auto found = File(file()).findSections(util::IdFilter<Section>(sec_tmp->id()));
        if (found.size() > 0) {
            sec = found.front().impl();
        }
    }

    return sec;
}


void SectionHDF5::link(const none_t t) {
    if (group().hasGroup("link")) {
        group().removeGroup("link");
    }
    forceUpdatedAt();
}


void SectionHDF5::mapping(const string &mapping) {
    if (mapping.empty()) {
        throw EmptyString("mapping");
    } else {
        group().setAttr("mapping", mapping);
        forceUpdatedAt();
    }
}


boost::optional<string> SectionHDF5::mapping() const {
    boost::optional<string> ret;
    string mapping;
    if (group().getAttr("mapping", mapping)) {
        ret = mapping;
    }
    return ret;
}


void SectionHDF5::mapping(const none_t t) {
    if (group().hasAttr("mapping")) {
        group().removeAttr("mapping");
    }
    forceUpdatedAt();
}

//--------------------------------------------------
// Methods for parent access
//--------------------------------------------------


shared_ptr<ISection> SectionHDF5::parent() const {
    return parent_section;
}


//--------------------------------------------------
// Methods for child section access
//--------------------------------------------------


size_t SectionHDF5::sectionCount() const {
    boost::optional<Group> g = section_group();
    return g ? g->objectCount() : size_t(0);
}


bool SectionHDF5::hasSectionByName(const string &name) const {
    // let getSectionByName try to look up object by name
    return getSectionByName(name) != nullptr;
}


bool SectionHDF5::hasSection(const string &id) const {
    // let getSection try to look up object by id
    return getSection(id) != nullptr;
}


shared_ptr<ISection> SectionHDF5::getSection(const string &id) const {
    shared_ptr<SectionHDF5> section;
    boost::optional<Group> g = section_group();

    if(g) {
        boost::optional<Group> group = g->findGroupByAttribute("entity_id", id);
        if (group) {
            auto p = const_pointer_cast<SectionHDF5>(shared_from_this());
            section = make_shared<SectionHDF5>(file(), p, *group);
        }
    }

    return section;
}


shared_ptr<ISection> SectionHDF5::getSectionByName(const string &name) const {
    shared_ptr<SectionHDF5> sec;    
    boost::optional<Group> g = section_group();

    if(g) {
        if (g->hasObject(name)) {
            Group group = g->openGroup(name, false);
            auto p = const_pointer_cast<SectionHDF5>(shared_from_this());
            sec = make_shared<SectionHDF5>(file(), p, group);
        }
    }
    
    return sec;
}


shared_ptr<ISection> SectionHDF5::getSection(size_t index) const {
    boost::optional<Group> g = section_group();
    string name = g ? g->objectName(index) : "";
    return getSectionByName(name);
}


shared_ptr<ISection> SectionHDF5::createSection(const string &name, const string &type) {
    if (hasSectionByName(name)) {
        throw DuplicateName("createSection");
    }
    string new_id = util::createId("section");
    boost::optional<Group> g = section_group(true);

    auto p = const_pointer_cast<SectionHDF5>(shared_from_this());
    Group grp = g->openGroup(name, true);
    return make_shared<SectionHDF5>(file(), p, grp, new_id, type, name);
}


bool SectionHDF5::deleteSection(const string &id) {
    boost::optional<Group> g = section_group();
    bool deleted = false;

    if (g) {
        // call deleteSection on sections to trigger recursive call to all sub-sections
        if (hasSection(id)) {
            // get instance of section about to get deleted
            Section section = getSection(id);
            // loop through all child sections and call deleteSection on them
            for (auto &child : section.sections()) {
                section.deleteSection(child.id());
            }
            // if hasSection is true then section_group always exists
            deleted = g->removeAllLinks(section.name());
        }
    }

    return deleted;
}


//--------------------------------------------------
// Methods for property access
//--------------------------------------------------


size_t SectionHDF5::propertyCount() const {
    boost::optional<Group> g = property_group();
    return g ? g->objectCount() : size_t(0);
}


bool SectionHDF5::hasProperty(const string &id) const {
    // let getProperty try to look up object by id    
    return getProperty(id) != nullptr;
}


bool SectionHDF5::hasPropertyByName(const string &name) const {
    // let getPropertyByName try to look up object by name    
    return getPropertyByName(name) != nullptr;
}


shared_ptr<IProperty> SectionHDF5::getProperty(const string &id) const {
    shared_ptr<PropertyHDF5> prop;
    boost::optional<Group> g = property_group();

    if (g) {
        boost::optional<DataSet> dset = g->findDataByAttribute("entity_id", id);
        if (dset)
            prop = make_shared<PropertyHDF5>(file(), *dset);
    }

    return prop;
}


shared_ptr<IProperty> SectionHDF5::getProperty(size_t index) const {
    boost::optional<Group> g = property_group();
    string name = g ? g->objectName(index) : "";
    return getPropertyByName(name);
}


shared_ptr<IProperty> SectionHDF5::getPropertyByName(const string &name) const {
    shared_ptr<PropertyHDF5> prop;
    boost::optional<Group> g = property_group();
    
    if (g) {
        if (g->hasObject(name)) {
            DataSet dset = g->openData(name);
            prop = make_shared<PropertyHDF5>(file(), dset);
        }
    }
    
    return prop;
}


shared_ptr<IProperty> SectionHDF5::createProperty(const string &name, const DataType &dtype) {
    if (hasPropertyByName(name)) {
        throw DuplicateName("hasPropertyByName");
    }
    string new_id = util::createId("property");
    boost::optional<Group> g = property_group(true);

    H5::DataType fileType = DataSet::fileTypeForValue(dtype);
    DataSet dataset = DataSet::create(g->h5Group(), name, fileType, {0});

    return make_shared<PropertyHDF5>(file(), dataset, new_id, name);
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
    boost::optional<Group> g = property_group();
    bool deleted = false;

    if (g && hasProperty(id)) {
        g->removeData(getProperty(id)->name());
        deleted = true;
    }

    return deleted;
}


SectionHDF5::~SectionHDF5() {}
