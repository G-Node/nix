// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/file/EntityWithSourcesFS.hpp>

#include <nix/util/util.hpp>
#include <nix/Block.hpp>

#include <algorithm>
#include <functional>

using namespace std;
using namespace nix::base;
using namespace boost::filesystem;

namespace nix {
namespace file {

EntityWithSourcesFS::EntityWithSourcesFS(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const string &loc)
    : EntityWithMetadataFS(file, loc),
      sources_dir(loc + path::preferred_separator + "sources", file->fileMode()), entity_block(block)
{
}


EntityWithSourcesFS::EntityWithSourcesFS(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const string &loc, const string &id,
                                             const string &type, const string &name)
    : EntityWithSourcesFS(file, block, loc, id, type, name, util::getTime())
{
}


EntityWithSourcesFS::EntityWithSourcesFS (const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const string &loc, const string &id,
                                              const string &type, const string &name, time_t time)
    : EntityWithMetadataFS(file, loc, id, type, name, time),
      sources_dir(loc + path::preferred_separator + name + path::preferred_separator + "sources", file->fileMode()), entity_block(block)
{
}


ndsize_t EntityWithSourcesFS::sourceCount() const {
    return sources_dir.subdirCount();
}


bool EntityWithSourcesFS::hasSource(const string &id) const {
    return !sources_dir.findByNameOrAttribute("entity_id", id)->empty();
}


shared_ptr<ISource> EntityWithSourcesFS::getSource(const string &name_or_id) const {
    shared_ptr<base::ISource> source;
    boost::optional<path> path = sources_dir.findByNameOrAttribute("name", name_or_id);
    if (path) {
        return make_shared<SourceFS>(file(), path->string());

    }
    return source;
}

shared_ptr<ISource> EntityWithSourcesFS::getSource(const size_t index) const {
    shared_ptr<base::ISource> source;
    path p = sources_dir.sub_dir_by_index(index);
    if (!p.empty()) {
        return make_shared<SourceFS>(file(), p.string());

    }
    return source;
}

void EntityWithSourcesFS::sources(const std::vector<Source> &sources) {
    // extract vectors of ids from vectors of new & old sources
    std::vector<std::string> ids_new(sources.size());
    transform(sources.begin(), sources.end(), ids_new.begin(), util::toId<Source>);
    // FIXME: issue #473
    // FIXME: this could go to a front-end like thing
    std::vector<Source> sources_old(static_cast<size_t>(sourceCount()));
    for (size_t i = 0; i < sources_old.size(); i++) sources_old[i] = getSource(i);
    std::vector<std::string> ids_old(sources_old.size());
    transform(sources_old.begin(), sources_old.end(), ids_old.begin(), util::toId<Source>);
    // sort them
    std::sort(ids_new.begin(), ids_new.end());
    std::sort(ids_old.begin(), ids_old.end());
    // get ids only in ids_new (add), ids only in ids_old (remove) & ignore rest
    std::vector<std::string> ids_add;
    std::vector<std::string> ids_rem;
    std::set_difference(ids_new.begin(), ids_new.end(), ids_old.begin(), ids_old.end(),
                        std::inserter(ids_add, ids_add.begin()));
    std::set_difference(ids_old.begin(), ids_old.end(), ids_new.begin(), ids_new.end(),
                        std::inserter(ids_rem, ids_rem.begin()));

    // check if all new sources exist
    Block tmp(entity_block);
    auto found = tmp.findSources(util::IdsFilter<Source>(ids_add));
    if (ids_add.size() != found.size())
        throw std::runtime_error("One or more sources do not exist in this block!");
    // add sources
    for (auto id : ids_add) {
        addSource(id);
    }
    // remove sources
    for (auto id : ids_rem) {
        removeSource(id);
    }
}

void EntityWithSourcesFS::addSource(const string &id) {
    if (id.empty())
        throw EmptyString("addSource");

    Block tmp(entity_block);
    auto found = tmp.findSources(util::IdFilter<Source>(id));
    if (found.empty())
        throw std::runtime_error("EntityWithSourcesFS::addSource: Given source does not exist in this block!");

    auto target = dynamic_pointer_cast<SourceFS>(found.front().impl());
    sources_dir.createDirectoryLink(target->location(), target->id());
}


bool EntityWithSourcesFS::removeSource(const string &id) {
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

