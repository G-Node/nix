// Copyright (c) 2013-2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/file/EntityWithMetadataFS.hpp>

#include <nix/util/util.hpp>
#include <nix/util/filter.hpp>
#include <nix/File.hpp>
#include <nix/hdf5/SectionHDF5.hpp> // FIXME once the Section is implemented

#include <memory>
#include <nix/file/SectionFS.hpp>

using namespace std;
using namespace nix::base;

namespace nix {
namespace file {

EntityWithMetadataFS::EntityWithMetadataFS(const shared_ptr<IFile> &file, const string &loc)
    : NamedEntityFS(file, loc)
{
}


EntityWithMetadataFS::EntityWithMetadataFS(const shared_ptr<IFile> &file, const string &loc, const string &id, const string &type, const string &name)
    : EntityWithMetadataFS(file, loc, id, type, name, util::getTime())
{
}


EntityWithMetadataFS::EntityWithMetadataFS(const shared_ptr<IFile> &file, const string &loc, const string &id, const string &type, const string &name, time_t time)
    : NamedEntityFS(file, loc, id, type, name, time)
{
}


void EntityWithMetadataFS::metadata(const std::string &id) {
    if (fileMode() == FileMode::ReadOnly) {
        throw std::runtime_error("EntityWithMetdata::metadata trying to set metadata in ReadOnly mode.");
    }
    if (id.empty())
        throw EmptyString("metadata");

    if (hasMetadata())
        metadata(none);
    File tmp = file();
    auto found = tmp.findSections(util::IdFilter<Section>(id));
    if (found.empty())
        throw std::runtime_error("EntityWithMetadataFS::metadata: Section not found in file!");

    auto target = dynamic_pointer_cast<SectionFS>(found.front().impl());
    boost::filesystem::path t(target->location()), p(location()), m("metadata");
    target->createLink(p / m);
    //boost::filesystem::create_directory_symlink(t, p / m);
}


shared_ptr<ISection> EntityWithMetadataFS::metadata() const {
    shared_ptr<ISection> sec;
    if (hasMetadata()) {
        boost::filesystem::path p(location()), m("metadata"), other_loc(p/m);
        auto sec_tmp = make_shared<EntityWithMetadataFS>(file(), other_loc.string());
        // re-get above section "sec_tmp": we just got it to have id, parent is missing,
        // findSections will return it with parent!
        auto found = File(file()).findSections(util::IdFilter<Section>(sec_tmp->id()));
        if (found.size() > 0) {
            sec = found.front().impl();
        }
    }
    return sec;
}


void EntityWithMetadataFS::metadata(const none_t t) {
    if (fileMode() == FileMode::ReadOnly) {
        throw std::runtime_error("EntityWithMetdata::metadata trying to set metadata in ReadOnly mode.");
    }
    if (hasMetadata()) {
        boost::filesystem::path p(location()), m("metadata"), other_loc(p/m);
        auto sec_tmp = make_shared<EntityWithMetadataFS>(file(), other_loc.string());
        boost::filesystem::path p1(location()), p2("metadata");
        sec_tmp->unlink(p1 / p2);
        boost::filesystem::remove_all(p1/p2);
    }
    forceUpdatedAt();
}


bool EntityWithMetadataFS::hasMetadata() const {
    boost::filesystem::path p1(location()), p2("metadata");
    return boost::filesystem::exists(p1/p2);
}


EntityWithMetadataFS::~EntityWithMetadataFS() {}

} // ns nix::file
} // ns nix