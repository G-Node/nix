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
    : SectionHDF5(file, nullptr, group, id)
{
}


SectionHDF5::SectionHDF5(const File &file, const Section &parent, const Group &group,
                         const string &id)
    : NamedEntityHDF5(file, group, id), parent_section(parent)
{
    property_group = this->group().openGroup("properties");
    section_group = this->group().openGroup("sections");
}


SectionHDF5::SectionHDF5(const File &file, const Group &group, const string &id, time_t time)
    : SectionHDF5(file, nullptr, group, id, time)
{
}


SectionHDF5::SectionHDF5(const File &file, const Section &parent, const Group &group,
                         const string &id, time_t time)
    : NamedEntityHDF5(file, group, id, time), parent_section(parent)
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


void SectionHDF5::link(const Section &link) {
    if (link == none) {
        SectionHDF5::link(none);
    } else {
        group().setAttr("link", link.id());
    }
}


Section SectionHDF5::link() const {
    string id;
    group().getAttr("link", id);

    vector<Section> found;
    if (id != "") {
        auto filter = [&](const Section &s) {
            return id == s.id();
        };

        found = file().findSections(filter);
    }

    if (found.size() > 0)
        return found[0];
    else
        return Section();
}


void SectionHDF5::link(const none_t t) {
    if(group().hasAttr("link")) {
        group().removeAttr("link");
    }
    forceUpdatedAt();
}


void SectionHDF5::mapping(const string &mapping) {
    group().setAttr("mapping", mapping);
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


Section SectionHDF5::parent() const {
    return parent_section;
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
    if (section_group.hasGroup(id)) {
        Group grp = section_group.openGroup(id, false);

        auto tmp = make_shared<SectionHDF5>(file(), grp, id);
        return Section(tmp);
    } else {
        return Section();
    }
}


Section SectionHDF5::getSection(size_t index) const {
    string id = section_group.objectName(index);

    return getSection(id);
}


Section SectionHDF5::createSection(const string &name, const string &type) {
    string new_id = util::createId("section");

    while (section_group.hasObject(new_id)) {
        new_id = util::createId("section");
    }

    Section parent(const_pointer_cast<SectionHDF5>(shared_from_this()));

    Group grp = section_group.openGroup(new_id, true);
    auto tmp = make_shared<SectionHDF5>(file(), parent, grp, new_id);
    tmp->name(name);
    tmp->type(type);

    return Section(tmp);
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
    return property_group.hasGroup(id);
}


Property SectionHDF5::getProperty(const string &id) const {
    if (property_group.hasGroup(id)) {
        Group g = property_group.openGroup(id,false);

        auto tmp = make_shared<PropertyHDF5>(file(), g, id);
        return Property(tmp);
    } else {
        return Property();
    }
}


Property SectionHDF5::getProperty(size_t index) const {
    string id = property_group.objectName(index);

    return getProperty(id);
}


bool SectionHDF5::hasPropertyWithName(const string &name) const {
    bool found = false;

    for (size_t i = 0; i < propertyCount(); i++) {
        string id = property_group.objectName(i);
        Group grp = property_group.openGroup(id);

        string other_name;
        grp.getAttr("name", other_name);

        if (other_name == name) {
            found = true;
            break;
        }
    }

    return found;
}


Property SectionHDF5::getPropertyByName(const string &name) const {
    Property prop;

    for (size_t i = 0; i < propertyCount(); i++) {
        string id = property_group.objectName(i);
        Group grp = property_group.openGroup(id);

        string other_name;
        grp.getAttr("name", other_name);

        if (other_name == name) {
            auto tmp = make_shared<PropertyHDF5>(file(), grp, id);
            prop = Property(tmp);
            break;
        }
    }

    return prop;
}


Property SectionHDF5::createProperty(const string &name) {
    if (hasPropertyWithName(name))
        throw runtime_error("Try to create a property with existing name: " + name);

    string new_id = util::createId("property");

    while (property_group.hasObject(new_id))
        new_id = util::createId("property");

    Group grp = property_group.openGroup(new_id, true);

    auto tmp = make_shared<PropertyHDF5>(file(), grp, new_id);
    tmp->name(name);

    return Property(tmp);
}


bool SectionHDF5::deleteProperty(const string &id) {
    if (property_group.hasObject(id)) {
        property_group.removeGroup(id);
        return true;
    } else {
        return false;
    }
}


SectionHDF5::~SectionHDF5() {}


} // namespace hdf5
} // namespace nix
