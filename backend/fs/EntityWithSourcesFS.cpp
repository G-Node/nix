// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "EntityWithSourcesFS.hpp"

#include <nix/util/util.hpp>
#include <nix/Block.hpp>

#include <algorithm>
#include <functional>

namespace bfs = boost::filesystem;

namespace nix {
namespace file {

EntityWithSourcesFS::EntityWithSourcesFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                                         const bfs::path &loc)
    : EntityWithMetadataFS(file, loc),
      sources_dir(bfs::path(loc) / bfs::path("sources"), file->fileMode()), entity_block(block)
{
}

EntityWithSourcesFS::EntityWithSourcesFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
										 const std::string &loc)
	: EntityWithSourcesFS(file, block, bfs::path(loc))
{
}


EntityWithSourcesFS::EntityWithSourcesFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                                         const bfs::path &loc, const std::string &id, const std::string &type,
                                         const std::string &name)
    : EntityWithSourcesFS(file, block, loc, id, type, name, util::getTime())
{
}

EntityWithSourcesFS::EntityWithSourcesFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
										 const std::string &loc, const std::string &id, const std::string &type,
										 const std::string &name)
	: EntityWithSourcesFS(file, block, bfs::path(loc), id, type, name)
{
}


EntityWithSourcesFS::EntityWithSourcesFS (const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                                          const bfs::path &loc, const std::string &id, const std::string &type,
                                          const std::string &name, time_t time)
    : EntityWithMetadataFS(file, loc, id, type, name, time),
      sources_dir(bfs::path(loc) / bfs::path(name + "/" + "sources"),
                  file->fileMode()), entity_block(block)
{
}

EntityWithSourcesFS::EntityWithSourcesFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
										 const std::string &loc, const std::string &id, const std::string &type,
										 const std::string &name, time_t time)
	: EntityWithSourcesFS(file, block, bfs::path(loc), id, type, name, time)
{
}


ndsize_t EntityWithSourcesFS::sourceCount() const {
    return sources_dir.subdirCount();
}


bool EntityWithSourcesFS::hasSource(const std::string &id) const {
    boost::optional<bfs::path> p = sources_dir.findByNameOrAttribute("entity_id", id);
    return p ? true : false;
}


std::shared_ptr<base::ISource> EntityWithSourcesFS::getSource(const std::string &name_or_id) const {
    std::shared_ptr<base::ISource> source;
    boost::optional<bfs::path> path = sources_dir.findByNameOrAttribute("name", name_or_id);
    if (path) {
        return std::make_shared<SourceFS>(file(), entity_block, path->string());

    }
    return source;
}

std::shared_ptr<base::ISource> EntityWithSourcesFS::getSource(const size_t index) const {
    std::shared_ptr<base::ISource> source;
    bfs::path p = sources_dir.sub_dir_by_index(index);
    if (!p.empty()) {
        return std::make_shared<SourceFS>(file(), entity_block, p.string());

    }
    return source;
}

void EntityWithSourcesFS::sources(const std::vector<Source> &sources) {
    while (sourceCount() > 0) {
        removeSource(getSource(0)->id());
    }
    for (const auto &src : sources) {
        if (block()->hasEntity(src) ) {
            addSource(src.id());
        }
    }
}

void EntityWithSourcesFS::addSource(const std::string &id) {
    if (id.empty())
        throw EmptyString("addSource");

    Block tmp(entity_block);
    auto found = tmp.findSources(util::IdFilter<Source>(id));
    if (found.empty())
        throw std::runtime_error("EntityWithSourcesFS::addSource: Given source does not exist in this block!");

    auto target = std::dynamic_pointer_cast<SourceFS>(found.front().impl());
    sources_dir.createDirectoryLink(target->location(), target->id());
}


bool EntityWithSourcesFS::removeSource(const std::string &id) {
    bool removed = false;
    if(sources_dir.hasObject(id)) {
        removed = sources_dir.removeObjectByNameOrAttribute("entity_id", id);
    }
    return removed;
}


std::shared_ptr<base::IBlock> EntityWithSourcesFS::block() const {
    return entity_block;
}


EntityWithSourcesFS::~EntityWithSourcesFS() {}

} // ns nix::file
} // ns nix
