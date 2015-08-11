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
#include <nix/file/SectionFS.hpp>
#include <nix/file/PropertyFS.hpp>


using namespace std;
using namespace nix::base;

namespace nix {
namespace file {


SectionFS::SectionFS(const std::shared_ptr<base::IFile> &file, const string &loc)
    : SectionFS(file, nullptr, loc)
{
}


SectionFS::SectionFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::ISection> &parent,
                     const string &loc)
    : NamedEntityFS(file, loc), parent_section(parent)
{
    createSubFolders(file);
}


SectionFS::SectionFS(const shared_ptr<IFile> &file, const string &loc, const string &id,
                     const string &type, const string &name)
    : SectionFS(file, nullptr, loc, id, type, name)
{
}


SectionFS::SectionFS(const shared_ptr<IFile> &file, const shared_ptr<ISection> &parent, const string &loc,
                     const string &id, const string &type, const string &name)
    : SectionFS(file, parent, loc, id, type, name, util::getTime())
{
}


SectionFS::SectionFS(const shared_ptr<IFile> &file, const string &loc, const string &id,
                     const string &type, const string &name, time_t time)
    : SectionFS(file, nullptr, loc, id, type, name, time)
{
}


SectionFS::SectionFS(const shared_ptr<IFile> &file, const shared_ptr<ISection> &parent, const string &loc,
                     const string &id, const string &type, const string &name, time_t time)
    : NamedEntityFS(file, loc, id, type, name, time), parent_section(parent)
{
    createSubFolders(file);
}


void SectionFS::createSubFolders(const shared_ptr<IFile> &file) {
    boost::filesystem::path data("properties");
    boost::filesystem::path metadata("sections");
    boost::filesystem::path p(location());

    property_dir = Directory(p / data, file->fileMode());
    subsection_dir = Directory(p / metadata, file->fileMode());
}
//--------------------------------------------------
// Attribute getter and setter
//--------------------------------------------------

void SectionFS::repository(const string &repository) {
    if (repository.empty()) {
        throw EmptyString("repository");
    } else {
        setAttr("repository", repository);
        forceUpdatedAt();
    }
}


boost::optional<string> SectionFS::repository() const {
    boost::optional<string> ret;
    string repository;
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
    if (id.empty())
        throw EmptyString("link");
    boost::filesystem::path p(location()), l("link");

    if (boost::filesystem::exists(p / l))
        link(none);

    File tmp = file();
    auto found = tmp.findSections(util::IdFilter<Section>(id));
    if (found.empty())
        throw std::runtime_error("SectionFS::link: Section not found in file!");

    auto target = dynamic_pointer_cast<SectionFS>(found.front().impl());
    boost::filesystem::path t(target->location());
    boost::filesystem::create_directory_symlink(t, p / l);
    forceUpdatedAt();
}


shared_ptr<ISection> SectionFS::link() const {
    shared_ptr<ISection> sec;

    if (boost::filesystem::exists({location() + "/link"})) {
        auto sec_tmp = make_shared<SectionFS>(file(), location() + "/link");
        // re-get above section "sec_tmp": parent missing, findSections will set it!
        auto found = File(file()).findSections(util::IdFilter<Section>(sec_tmp->id()));
        if (found.size() > 0) {
            sec = found.front().impl();
        }
    }
    return sec;
}


void SectionFS::link(const none_t t) {
    if (boost::filesystem::exists({location() + "/link"})) {
        boost::filesystem::remove_all({location() + "/link"});
    }
    forceUpdatedAt();
}


void SectionFS::mapping(const string &mapping) {
    if (mapping.empty()) {
        throw EmptyString("mapping");
    } else {
        setAttr("mapping", mapping);
        forceUpdatedAt();
    }
}


boost::optional<string> SectionFS::mapping() const {
    boost::optional<string> ret;
    string mapping;
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


shared_ptr<ISection> SectionFS::parent() const {
    return parent_section;
}


//--------------------------------------------------
// Methods for child section access
//--------------------------------------------------


ndsize_t SectionFS::sectionCount() const {
    return subsection_dir.subdirCount();
}


bool SectionFS::hasSection(const string &name_or_id) const {
    return getSection(name_or_id) != nullptr;
}


shared_ptr<ISection> SectionFS::getSection(const string &name_or_id) const {
    shared_ptr<base::ISection> sec;
    boost::optional<boost::filesystem::path> path = subsection_dir.findByNameOrAttribute("entity_id", name_or_id);
    if (path) {
        SectionFS s(file(), path->string());
        return make_shared<SectionFS>(s);
    }
    return sec;
}


shared_ptr<ISection> SectionFS::getSection(ndsize_t index) const {
    if (index >= sectionCount()) {
        throw OutOfBounds("Trying to access section.subsection with invalid index.", index);
    }
    boost::filesystem::path p = subsection_dir.sub_dir_by_index(index);
    shared_ptr<SectionFS> sec;
    sec = make_shared<SectionFS>(file(), p.string());
    return sec;
}


shared_ptr<ISection> SectionFS::createSection(const string &name, const string &type) {
    if (hasSection(name)) {
        throw DuplicateName("createSection");
    }
    string id = util::createId();
    SectionFS s(file(), shared_from_this(), subsection_dir.location(), id, type, name);
    return make_shared<SectionFS>(s);
}


bool SectionFS::deleteSection(const string &name_or_id) {
    return subsection_dir.removeObjectByNameOrAttribute("entity_id", name_or_id);
}


//--------------------------------------------------
// Methods for property access
//--------------------------------------------------


ndsize_t SectionFS::propertyCount() const {
    return property_dir.subdirCount();
}


bool SectionFS::hasProperty(const string &name_or_id) const {
    return getProperty(name_or_id) != nullptr;
}


shared_ptr<IProperty> SectionFS::getProperty(const string &name_or_id) const {
    shared_ptr<PropertyFS> prop;
    boost::optional<boost::filesystem::path> path = property_dir.findByNameOrAttribute("entity_id", name_or_id);
    if (path) {
        PropertyFS p(file(), *path);
        return make_shared<PropertyFS>(p);
    }
    return prop;
}


shared_ptr<IProperty> SectionFS::getProperty(ndsize_t index) const {
    if (index >= propertyCount()) {
        throw OutOfBounds("Trying to access section.property with invalid index.", index);
    }
    boost::filesystem::path p = property_dir.sub_dir_by_index(index);
    shared_ptr<PropertyFS> prop;
    prop = make_shared<PropertyFS>(file(), p.string());
    return prop;
}


shared_ptr<IProperty> SectionFS::createProperty(const string &name, const DataType &dtype) {
    if (hasProperty(name)) {
        throw DuplicateName("hasProperty");
    }
    string new_id = util::createId();
    PropertyFS p(file(), property_dir.location(), new_id, name, dtype);
    return make_shared<PropertyFS>(p);
}


shared_ptr<IProperty> SectionFS::createProperty(const string &name, const Value &value) {
    shared_ptr<IProperty> p = createProperty(name, value.type());
    vector<Value> val{value};
    p->values(val);
    return p;
}


shared_ptr<IProperty> SectionFS::createProperty(const string &name, const vector<Value> &values) {
    if (values.size() < 1)
        throw runtime_error("Trying to create a property without a value!");

    shared_ptr<IProperty> p = createProperty(name, values[0].type());
    p->values(values);
    return p;
}


bool SectionFS::deleteProperty(const string &name_or_id) {
    return property_dir.removeObjectByNameOrAttribute("entity_id", name_or_id);
}


SectionFS::~SectionFS() {}

} // ns nix::file
} // ns nix