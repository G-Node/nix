// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "SectionHDF5.hpp"

#include <nix/util/util.hpp>
#include <nix/util/filter.hpp>
#include <nix/File.hpp>
#include <nix/Section.hpp>

#include "PropertyHDF5.hpp"

using namespace std;
using namespace nix::base;

namespace nix {
namespace hdf5 {


SectionHDF5::SectionHDF5(const std::shared_ptr<base::IFile> &file, const H5Group &group)
    : SectionHDF5(file, nullptr, group)
{
}


SectionHDF5::SectionHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::ISection> &parent, const H5Group &group)
    : NamedEntityHDF5(file, group), parent_section(parent)
{
    property_group = this->group().openOptGroup("properties");
    section_group = this->group().openOptGroup("sections");
}


SectionHDF5::SectionHDF5(const shared_ptr<IFile> &file, const H5Group &group, const string &id,
                         const string &type, const string &name)
    : SectionHDF5(file, nullptr, group, id, type, name)
{
}


SectionHDF5::SectionHDF5(const shared_ptr<IFile> &file, const shared_ptr<ISection> &parent, const H5Group &group,
                         const string &id, const string &type, const string &name)
    : SectionHDF5(file, parent, group, id, type, name, util::getTime())
{
}


SectionHDF5::SectionHDF5(const shared_ptr<IFile> &file, const H5Group &group, const string &id,
                         const string &type, const string &name, time_t time)
    : SectionHDF5(file, nullptr, group, id, type, name, time)
{
}


SectionHDF5::SectionHDF5(const shared_ptr<IFile> &file, const shared_ptr<ISection> &parent, const H5Group &group,
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
    group().setAttr("repository", repository);
    forceUpdatedAt();
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
        H5Group other_group = group().openGroup("link", false);
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


//--------------------------------------------------
// Methods for parent access
//--------------------------------------------------


shared_ptr<ISection> SectionHDF5::parent() const {
    return parent_section;
}


//--------------------------------------------------
// Methods for child section access
//--------------------------------------------------


ndsize_t SectionHDF5::sectionCount() const {
    boost::optional<H5Group> g = section_group();
    return g ? g->objectCount() : size_t(0);
}


bool SectionHDF5::hasSection(const string &name_or_id) const {
    return getSection(name_or_id) != nullptr;
}


shared_ptr<ISection> SectionHDF5::getSection(const string &name_or_id) const {
    shared_ptr<SectionHDF5> section;
    boost::optional<H5Group> g = section_group();

    if(g) {
        boost::optional<H5Group> group = g->findGroupByNameOrAttribute("entity_id", name_or_id);
        if (group) {
            auto p = const_pointer_cast<SectionHDF5>(shared_from_this());
            section = make_shared<SectionHDF5>(file(), p, *group);
        }
    }

    return section;
}


shared_ptr<ISection> SectionHDF5::getSection(ndsize_t index) const {
    boost::optional<H5Group> g = section_group();
    string name = g ? g->objectName(index) : "";
    return getSection(name);
}


shared_ptr<ISection> SectionHDF5::createSection(const string &name, const string &type) {
    string new_id = util::createId();
    boost::optional<H5Group> g = section_group(true);

    auto p = const_pointer_cast<SectionHDF5>(shared_from_this());
    H5Group grp = g->openGroup(name, true);
    return make_shared<SectionHDF5>(file(), p, grp, new_id, type, name);
}


bool SectionHDF5::deleteSection(const string &name_or_id) {
    boost::optional<H5Group> g = section_group();
    bool deleted = false;

    if (g) {
        // call deleteSection on sections to trigger recursive call to all sub-sections
        if (hasSection(name_or_id)) {
            // get instance of section about to get deleted
            Section section = getSection(name_or_id);
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


ndsize_t SectionHDF5::propertyCount() const {
    boost::optional<H5Group> g = property_group();
    return g ? g->objectCount() : size_t(0);
}


bool SectionHDF5::hasProperty(const string &name_or_id) const {
    return getProperty(name_or_id) != nullptr;
}


shared_ptr<IProperty> SectionHDF5::getProperty(const string &name_or_id) const {
    shared_ptr<PropertyHDF5> prop;
    boost::optional<H5Group> g = property_group();

    if (g) {
        boost::optional<DataSet> dset = g->findDataByNameOrAttribute("entity_id", name_or_id);
        if (dset)
            prop = make_shared<PropertyHDF5>(file(), *dset);
    }

    return prop;
}


shared_ptr<IProperty> SectionHDF5::getProperty(ndsize_t index) const {
    boost::optional<H5Group> g = property_group();
    string name = g ? g->objectName(index) : "";
    return getProperty(name);
}


shared_ptr<IProperty> SectionHDF5::createProperty(const string &name, const DataType &dtype, const NDSize &shape) {
    string new_id = util::createId();
    boost::optional<H5Group> g = property_group(true);
    DataSet ds = g->createData(name, data_type_to_h5_filetype(dtype), shape);
    return make_shared<PropertyHDF5>(file(), ds, new_id, name);
}


shared_ptr<IProperty> SectionHDF5::createProperty(const string &name, const DataType &dtype) {
    shared_ptr<IProperty> p = createProperty(name, dtype, {8});
    return p;
}


shared_ptr<IProperty> SectionHDF5::createProperty(const string &name, const Variant &value) {
    shared_ptr<IProperty> p = createProperty(name, value.type(), {8});
    vector<Variant> val{value};
    p->values(val);
    return p;
}


shared_ptr<IProperty> SectionHDF5::createProperty(const string &name, const vector<Variant> &values) {
    NDSize shape(1, values.size());
    shared_ptr<IProperty> p = createProperty(name, values[0].type(), shape);
    p->values(values);
    return p;
}


bool SectionHDF5::deleteProperty(const string &name_or_id) {
    boost::optional<H5Group> g = property_group();
    bool deleted = false;
    if (g && hasProperty(name_or_id)) {
        g->removeData(getProperty(name_or_id)->name());
        deleted = true;
    }

    return deleted;
}


shared_ptr<IFile> SectionHDF5::parentFile() const {
    return file();
}

SectionHDF5::~SectionHDF5() {}

} // ns nix::hdf5
} // ns nix
