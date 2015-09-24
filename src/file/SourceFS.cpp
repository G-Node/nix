// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/file/SourceFS.hpp>
#include <nix/util/util.hpp>
#include <nix/Source.hpp>

namespace bfs = boost::filesystem;
namespace nix {
namespace file {


SourceFS::SourceFS(const std::shared_ptr<base::IFile> &file, const std::string &loc)
    : EntityWithMetadataFS(file, loc)
{
    createSubFolders(file);
}


SourceFS::SourceFS(const std::shared_ptr<base::IFile> &file, const std::string &loc, const std::string &id,
                   const std::string &type, const std::string &name)
    : SourceFS(file, loc, id, type, name, util::getTime())
{
}


SourceFS::SourceFS(const std::shared_ptr<base::IFile> &file, const std::string &loc, const std::string &id,
                   const std::string &type, const std::string &name, time_t time)
    : EntityWithMetadataFS(file, loc, id, type, name, time)
{
    createSubFolders(file);
}


void SourceFS::createSubFolders(const std::shared_ptr<base::IFile> &file) {
    bfs::path s("sources"), p(location());
    sources_dir = Directory(p / s, file->fileMode());
}

bool SourceFS::hasSource(const std::string &name_or_id) const {
    return getSource(name_or_id) != nullptr;
}


std::shared_ptr<base::ISource> SourceFS::getSource(const std::string &name_or_id) const {
    std::shared_ptr<SourceFS> source;
    boost::optional<bfs::path> p = sources_dir.findByNameOrAttribute("entity_id", name_or_id);
    if (p) {
        source = std::make_shared<SourceFS>(file(), p->string());
    }

    return source;
}


std::shared_ptr<base::ISource> SourceFS::getSource(ndsize_t index) const {
    if (index >= sourceCount()) {
        throw OutOfBounds("Trying to access source.source with invalid index.", index);
    }
    bfs::path p = sources_dir.sub_dir_by_index(index);
    return std::make_shared<SourceFS>(file(), p.string());
}


ndsize_t SourceFS::sourceCount() const {
    return sources_dir.subdirCount();
}


std::shared_ptr<base::ISource> SourceFS::createSource(const std::string &name, const std::string &type) {
    if (name.empty()) {
        throw EmptyString("name");
    }
    if (hasSource(name)) {
        throw DuplicateName("createSource");
    }
    std::string id = util::createId();
    return std::make_shared<SourceFS>(file(), sources_dir.location(), id, type, name);
}


bool SourceFS::deleteSource(const std::string &name_or_id) {
    return sources_dir.removeObjectByNameOrAttribute("entity_id", name_or_id);
}


SourceFS::~SourceFS() {}

} // ns nix::file
} // ns nix
