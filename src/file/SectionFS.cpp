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
    createSubFolders();
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
    cerr << "Section:: location!  "<< this->location() << endl;
    createSubFolders();
}


void SectionFS::createSubFolders() {
    boost::filesystem::path props("properties"), secs("sections");
    boost::filesystem::create_directories(boost::filesystem::path(location().c_str() / props));
    boost::filesystem::create_directories(boost::filesystem::path(location().c_str() / secs));
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

    if (boost::filesystem::exists({location() + "/link"}))
        link(none);

    File tmp = file();
    auto found = tmp.findSections(util::IdFilter<Section>(id));
    if (found.empty())
        throw std::runtime_error("SectionFS::link: Section not found in file!");

    auto target = dynamic_pointer_cast<SectionFS>(found.front().impl());
    boost::filesystem::create_directory_symlink(target->location(), {"link"});
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
    if (boost::filesystem::exists({location() + "/mapping"})) {
        boost::filesystem::remove_all({location() + "/mapping"});
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
    ndsize_t count = 0;
    boost::filesystem::directory_iterator end;
    boost::filesystem::path p(location() + "/sections");
    if (boost::filesystem::exists(p) &&
        boost::filesystem::is_directory(p)) {
        boost::filesystem::directory_iterator di(p);
        while (di != end) {
            count ++;
            ++di;
        }
    }
    return count;
}


bool SectionFS::hasSection(const string &name_or_id) const {
    return getSection(name_or_id) != nullptr;
}


shared_ptr<ISection> SectionFS::getSection(const string &name_or_id) const {
    shared_ptr<SectionFS> section;
    boost::filesystem::path p(location() + "/sections");
    /*
    if(boost::filesystem::exists({location() + "/sections"})) {
        boost::optional<Group> group = g->findGroupByNameOrAttribute("entity_id", name_or_id);
        if (group) {
            auto p = const_pointer_cast<SectionHDF5>(shared_from_this());
            section = make_shared<SectionHDF5>(file(), p, *group);
        }
    }
    */
    //FIXME
    return section;
}


shared_ptr<ISection> SectionFS::getSection(ndsize_t index) const {
    // boost::optional<Group> g = section_group(); FIXME
    // string name = g ? g->objectName(index) : "";
    return getSection("");
}


shared_ptr<ISection> SectionFS::createSection(const string &name, const string &type) {
    if (hasSection(name)) {
        throw DuplicateName("createSection");
    }
    string new_id = util::createId();
    //boost::optional<Group> g = section_group(true);

    auto p = const_pointer_cast<SectionFS>(shared_from_this());
    //Group grp = g->openGgrproup(name, true);
    return make_shared<SectionFS>(file(), p, location() + "/" + name, new_id, type, name); //FIXME this needs to go to the sections subfolder!
}


bool SectionFS::deleteSection(const string &name_or_id) {
    //boost::optional<Group> g = section_group();
    bool deleted = false;
    /*
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
    */ //FIXME
    return deleted;
}


//--------------------------------------------------
// Methods for property access
//--------------------------------------------------


ndsize_t SectionFS::propertyCount() const {
    // boost::optional<Group> g = property_group(); FIXME
    return 0;
}


bool SectionFS::hasProperty(const string &name_or_id) const {
    return getProperty(name_or_id) != nullptr;
}


shared_ptr<IProperty> SectionFS::getProperty(const string &name_or_id) const {
    shared_ptr<PropertyFS> prop;
    /*
    boost::optional<Group> g = property_group();

    if (g) {
        boost::optional<DataSet> dset = g->findDataByNameOrAttribute("entity_id", name_or_id);
        if (dset)
            prop = make_shared<PropertyHDF5>(file(), *dset);
    }
    */
    return prop;
}


shared_ptr<IProperty> SectionFS::getProperty(ndsize_t index) const {
    // boost::optional<Group> g = property_group();
    //string name = g ? g->objectName(index) : ""; FIXME
    return getProperty("");
}


shared_ptr<IProperty> SectionFS::createProperty(const string &name, const DataType &dtype) {
    if (hasProperty(name)) {
        throw DuplicateName("hasProperty");
    }
    string new_id = util::createId();
    return make_shared<PropertyFS>(file(), new_id, name, dtype);
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
    /*
    boost::optional<Group> g = property_group();
    bool deleted = false;

    if (g && hasProperty(name_or_id)) {
        g->removeData(getProperty(name_or_id)->name());
        deleted = true;
    }
    */
    return false;
}


SectionFS::~SectionFS() {}

} // ns nix::file
} // ns nix