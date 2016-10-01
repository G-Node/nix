// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include <nix/util/filter.hpp>
#include <nix/File.hpp>
#include "SectionFS.hpp"
#include "PropertyFS.hpp"

namespace bfs = boost::filesystem;

namespace nix {
namespace file {


SectionFS::SectionFS(const std::shared_ptr<base::IFile> &file, const std::string &loc)
    : SectionFS(file, nullptr, loc)
{
}


SectionFS::SectionFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::ISection> &parent,
                     const std::string &loc)
    : NamedEntityFS(file, loc), parent_section(parent)
{
    createSubFolders(file);
}


SectionFS::SectionFS(const std::shared_ptr<base::IFile> &file, const std::string &loc, const std::string &id,
                     const std::string &type, const std::string &name)
    : SectionFS(file, nullptr, loc, id, type, name)
{
}


SectionFS::SectionFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::ISection> &parent,
                     const std::string &loc, const std::string &id, const std::string &type, const std::string &name)
    : SectionFS(file, parent, loc, id, type, name, util::getTime())
{
}


SectionFS::SectionFS(const std::shared_ptr<base::IFile> &file, const std::string &loc, const std::string &id,
                     const std::string &type, const std::string &name, time_t time)
    : SectionFS(file, nullptr, loc, id, type, name, time)
{
}


SectionFS::SectionFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::ISection> &parent,
                     const std::string &loc, const std::string &id, const std::string &type, const std::string &name, time_t time)
    : NamedEntityFS(file, loc, id, type, name, time), parent_section(parent)
{
    createSubFolders(file);
}


void SectionFS::createSubFolders(const std::shared_ptr<base::IFile> &file) {
    bfs::path data("properties");
    bfs::path metadata("sections");
    bfs::path p(location());

    property_dir = Directory(p / data, file->fileMode());
    subsection_dir = Directory(p / metadata, file->fileMode());
}
//--------------------------------------------------
// Attribute getter and setter
//--------------------------------------------------

void SectionFS::repository(const std::string &repository) {
    if (repository.empty()) {
        throw EmptyString("repository");
    } else {
        setAttr("repository", repository);
        forceUpdatedAt();
    }
}


boost::optional<std::string> SectionFS::repository() const {
    boost::optional<std::string> ret;
    std::string repository;
    if (hasAttr("repository")) {
        getAttr("repository", repository);
        ret = repository;
    }
    return ret;
}


void SectionFS::repository(const none_t t) {
    if (hasAttr("repository")) {
        removeAttr("repository");
    }
    forceUpdatedAt();
}


void SectionFS::link(const std::string &id) {
    bfs::path p(location()), l("link");

    if (bfs::exists(p / l)) {
        link(none);
    }

    File tmp = file();
    auto found = tmp.findSections(util::IdFilter<Section>(id));
    if (found.empty()) {
        throw std::runtime_error("SectionFS::link: Section not found in file!");
    }
    auto target = std::dynamic_pointer_cast<SectionFS>(found.front().impl());
    target->createLink(p / l);
    forceUpdatedAt();
}


std::shared_ptr<base::ISection> SectionFS::link() const {
    std::shared_ptr<base::ISection> sec;

    if (bfs::exists({location() + "/link"})) {
        auto sec_tmp = std::make_shared<SectionFS>(file(), location() + "/link");
        // re-get above section "sec_tmp": parent missing, findSections will set it!
        auto found = File(file()).findSections(util::IdFilter<Section>(sec_tmp->id()));
        if (found.size() > 0) {
            sec = found.front().impl();
        }
    }
    return sec;
}


void SectionFS::link(const none_t t) {
    if (bfs::exists({location() + "/link"})) {
        bfs::remove_all({location() + "/link"});
    }
    forceUpdatedAt();
}


void SectionFS::mapping(const std::string &mapping) {
    if (mapping.empty()) {
        throw EmptyString("mapping");
    } else {
        setAttr("mapping", mapping);
        forceUpdatedAt();
    }
}


boost::optional<std::string> SectionFS::mapping() const {
    boost::optional<std::string> ret;
    std::string mapping;
    if (hasAttr("mapping")) {
        getAttr("mapping", mapping);
        ret = mapping;
    }
    return ret;
}


void SectionFS::mapping(const none_t t) {
    if (hasAttr("mapping")) {
        removeAttr("mapping");
    }
    forceUpdatedAt();
}

//--------------------------------------------------
// Methods for parent access
//--------------------------------------------------


std::shared_ptr<base::ISection> SectionFS::parent() const {
    return parent_section;
}


//--------------------------------------------------
// Methods for child section access
//--------------------------------------------------


ndsize_t SectionFS::sectionCount() const {
    return subsection_dir.subdirCount();
}


bool SectionFS::hasSection(const std::string &name_or_id) const {
    return getSection(name_or_id) != nullptr;
}


std::shared_ptr<base::ISection> SectionFS::getSection(const std::string &name_or_id) const {
    std::shared_ptr<base::ISection> sec;
    boost::optional<bfs::path> path = subsection_dir.findByNameOrAttribute("entity_id", name_or_id);
    if (path) {
        SectionFS s(file(), path->string());
        return std::make_shared<SectionFS>(s);
    }
    return sec;
}


std::shared_ptr<base::ISection> SectionFS::getSection(ndsize_t index) const {
    if (index >= sectionCount()) {
        throw OutOfBounds("Trying to access section.subsection with invalid index.", index);
    }
    bfs::path p = subsection_dir.sub_dir_by_index(index);
    return std::make_shared<SectionFS>(file(), p.string());
}


std::shared_ptr<base::ISection> SectionFS::createSection(const std::string &name, const std::string &type) {
    if (hasSection(name)) {
        throw DuplicateName("createSection");
    }
    std::string id = util::createId();
    SectionFS s(file(), shared_from_this(), subsection_dir.location(), id, type, name);
    return std::make_shared<SectionFS>(s);
}


bool SectionFS::deleteSection(const std::string &name_or_id) {
    bool success = true;
    Section s = getSection(name_or_id);
    success = SectionFS::removeSubsections(s);
    if (success) {
        success = success && subsection_dir.removeObjectByNameOrAttribute("entity_id", name_or_id);
    }
    return success;
}


bool SectionFS::removeSubsections(Section &section) {
    bool success = true;
    std::vector<Section> secs = section.sections();
    for(auto &s : secs) {
        success = success  && section.deleteSection(s.name());
    }
    return success;
}

//--------------------------------------------------
// Methods for property access
//--------------------------------------------------

ndsize_t SectionFS::propertyCount() const {
    return property_dir.subdirCount();
}


bool SectionFS::hasProperty(const std::string &name_or_id) const {
    return getProperty(name_or_id) != nullptr;
}


std::shared_ptr<base::IProperty> SectionFS::getProperty(const std::string &name_or_id) const {
    std::shared_ptr<PropertyFS> prop;
    boost::optional<bfs::path> path = property_dir.findByNameOrAttribute("entity_id", name_or_id);
    if (path) {
        PropertyFS p(file(), *path);
        return std::make_shared<PropertyFS>(p);
    }
    return prop;
}


std::shared_ptr<base::IProperty> SectionFS::getProperty(ndsize_t index) const {
    if (index >= propertyCount()) {
        throw OutOfBounds("Trying to access section.property with invalid index.", index);
    }
    bfs::path p = property_dir.sub_dir_by_index(index);
    return std::make_shared<PropertyFS>(file(), p.string());
}


std::shared_ptr<base::IProperty> SectionFS::createProperty(const std::string &name, const DataType &dtype) {
    if (hasProperty(name)) {
        throw DuplicateName("hasProperty");
    }
    std::string new_id = util::createId();
    return std::make_shared<PropertyFS>(file(), property_dir.location(), new_id, name, dtype);
}


std::shared_ptr<base::IProperty> SectionFS::createProperty(const std::string &name, const Value &value) {
    std::shared_ptr<base::IProperty> p = createProperty(name, value.type());
    std::vector<Value> val{value};
    p->values(val);
    return p;
}


std::shared_ptr<base::IProperty> SectionFS::createProperty(const std::string &name, const std::vector<Value> &values) {
    if (values.size() < 1)
        throw std::runtime_error("Trying to create a property without a value!");

    std::shared_ptr<base::IProperty> p = createProperty(name, values[0].type());
    p->values(values);
    return p;
}


bool SectionFS::deleteProperty(const std::string &name_or_id) {
    return property_dir.removeObjectByNameOrAttribute("entity_id", name_or_id);
}

std::shared_ptr<base::IFile> SectionFS::parentFile() const {
    return file();
}

SectionFS::~SectionFS() {}

} // ns nix::file
} // ns nix