// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/SectionHDF5.hpp>

using namespace std;

namespace nix {
namespace hdf5 {


SectionHDF5::SectionHDF5(const SectionHDF5 &section)
    : NamedEntityHDF5(section.file(), section.group(), section.id())
{
    property_group = section.property_group;
    section_group = section.section_group;
}


SectionHDF5::SectionHDF5(File file, Group group,const  string &id)
    : NamedEntityHDF5(file, group, id)
{
    property_group = group.openGroup("properties");
    section_group = group.openGroup("sections");
}


SectionHDF5::SectionHDF5(File file, Group group, const string &id, time_t time)
    : NamedEntityHDF5(file, group, id, time)
{
    property_group = group.openGroup("properties");
    section_group = group.openGroup("sections");
}

/*
void SectionHDF5::repository(const string &repository) {
    group.setAttr("repository", repository);
}


string SectionHDF5::repository() const {
    string repository;
    group.getAttr("repository", repository);
    return repository;
}


void SectionHDF5::link(const string &link) {
    vector<Section> sects = this->file.findSection(link);
    if(sects.size() == 1 && sects[0].type().compare(this->type()) == 0) {
        group.setAttr("link", link);
    }
    else if (sects.empty()) {
        throw std::runtime_error(
                    "Cannot create link! Linked section does not exist!");
    }
    else {
        throw std::runtime_error(
                    "Cannot create link to a section of deviating type!");
    }
}


string SectionHDF5::link() const {
    string link;
    group.getAttr("link", link);
    return link;
}


void SectionHDF5::mapping(const string &mapping) {
    group.setAttr("mapping", mapping);
}


string SectionHDF5::mapping() const {
    string mapping;
    group.getAttr("mapping", mapping);
    return mapping;
}


void SectionHDF5::parent(const string &parent) {
    group.setAttr("parent", parent);
}


string SectionHDF5::parent() const {
    string parent;
    group.getAttr("parent", parent);
    return parent;
}


bool SectionHDF5::hasParent() const{
    return (parent().length() != 0 && !file.findSection(parent()).empty());
}


Section SectionHDF5::findParent() const{
    std::vector<Section> s = file.findSection(parent());
    if(s.empty())
        throw std::runtime_error(
                "Section has no parent or parent could not be found!");
    return file.findSection(parent())[0];
}


Section SectionHDF5::addSection(const std::string &name, const std::string &type) {
    string id = util::createId("section");
    Section s(file, section_group.openGroup(id,true), id);
    s.name(name);
    s.type(type);
    s.parent(this->id());
    return s;
}


bool SectionHDF5::removeSection(const std::string &id) {
    bool success = false;
    if(section_group.hasGroup(id)){
        section_group.removeGroup(id);
        success = true;
    }
    return success;
}


bool SectionHDF5::hasSection(const std::string &id) const {
    return section_group.hasGroup(id);
}


std::vector<Section> SectionHDF5::getRelatedSections(const std::string &type) const {
    std::vector<Section> victor = findDownstream(type);
    if(victor.size() != 0){
        return victor;
    }
    victor = findUpstream(type);
    if(victor.size() != 0){
        return victor;
    }
    victor = findSideways(type);
    return victor;
}


bool SectionHDF5::hasRelatedSection(const std::string &type) const {
    std::vector<Section> victor = findDownstream(type);
    if(victor.size() != 0){
        return true;
    }
    victor = findUpstream(type);
    if(victor.size() > 0){
        return true;
    }
    return findSideways(type).size() != 0;
}


std::vector<Section> SectionHDF5::findDownstream(const std::string &type) const {
    std::vector<Section> victor;
    victor = collectIf([&](const Section &section) {
        bool found = section.type() == type;
        return found;
    });
    return victor;
}


std::vector<Section> SectionHDF5::findUpstream(const std::string &type) const {
    std::vector<Section> secs;
    if(hasParent()){
        Section p = findParent();
        if(p.type().compare(type) == 0){
            secs.push_back(p);
            return secs;
        }
        secs = p.findUpstream(type);
        if(secs.size() > 0){
            return secs;
        }
    }
    return secs;
}


std::vector<Section> SectionHDF5::findSideways(const std::string &type) const {
    std::vector<Section> victor;
    if(hasParent()){
        Section p = findParent();
        victor = p.collectIf([&](const Section &section) {
            bool found = section.type() == type;
            return found;
        },true,1);
        if(victor.size() > 0){
            return victor;
        }
        victor = p.findSideways(type);
        if(victor.size() > 0){
            return victor;
        }
    }
    return victor;
}


std::vector<Section> SectionHDF5::sections() const {
    vector<Section>  section_obj;
    size_t section_count = section_group.objectCount();
    for (size_t i = 0; i < section_count; i++) {
        string id = section_group.objectName(i);
        Group g = section_group.openGroup(id,false);
        Section s(file,g,id);
        section_obj.push_back(s);
    }
    return section_obj;
}


Section::size_type SectionHDF5::childCount() const
{
    return section_group.objectCount();
}


Section SectionHDF5::getChild(size_t index) const {
    string id = section_group.objectName(index);
    Section s(file, section_group.openGroup(id), id);
    return s;
}


std::vector<Property> SectionHDF5::properties() const {
    std::vector<Property> props;
    for (size_t i = 0; i < propertyCount(); i++){
        string id = property_group.objectName(i);
        Group g = property_group.openGroup(id,false);
        Property p = Property(g,id);
        props.push_back(p);
    }
    return props;
}


std::vector<Property> SectionHDF5::inheritedProperties() const {
    std::vector<Property> props;
    if(this->link().length() > 0){
        std::vector<Section> sects = file.findSection(this->link());
        if(!sects.empty()){
            props = sects[0].properties();
        }
    }
    return props;
}


Property SectionHDF5::getProperty(const std::string &id) const {
    if (property_group.hasGroup(id)) {
        return Property(property_group.openGroup(id, false), id);
    } else {
        throw std::runtime_error(
                    "Requested Property does not exist! Always check with hasProperty!");
    }
}


Property SectionHDF5::getPropertyByName(const std::string &name) const {
    std::vector<Property> props = properties();
    for (size_t i = 0; i < props.size(); i ++){
        if (props[i].name().compare(name) == 0)
            return props[i];
    }
    props = inheritedProperties();
    for(size_t i = 0; i < props.size(); i++)
        if(props[i].name().compare(name) == 0)
            return props[i];

    throw std::runtime_error(
                "Requested Property does not exist! Always check with hasPropertyByName!");
}


Property SectionHDF5::addProperty(const std::string &name) {
    if (hasPropertyByName(name)) {
        throw std::runtime_error("Attempt to add a property that already exists!");
    }
    string new_id = util::createId("property");
    while (property_group.hasObject(new_id))
        new_id = util::createId("property");
    Property p(property_group.openGroup(new_id, true), new_id);
    p.name(name);
    return p;
}


void SectionHDF5::removeProperty(const std::string &id) {
    if (property_group.hasObject(id)) {
        property_group.removeGroup(id);
    }
}


size_t SectionHDF5::propertyCount() const {
    size_t count = property_group.objectCount();
    if (group.hasGroup("values"))
        count--;
    return count;
}


bool SectionHDF5::hasPropertyByName(const string &name) const {
    std::vector<Property> props = properties();
    for (size_t i = 0; i < props.size(); i++){
        if (props[i].name().compare(name)==0)
            return true;
    }
    return false;
}

bool SectionHDF5::hasProperty(const std::string &id) const {
    return property_group.hasGroup(id);
}

*/


SectionHDF5::~SectionHDF5() {}


} // namespace hdf5
} // namespace nix
