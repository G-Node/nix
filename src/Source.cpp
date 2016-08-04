// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/Source.hpp>

#include <queue>
#include <nix/File.hpp>
#include <nix/util/util.hpp>

using namespace nix;

Source::Source()
    : EntityWithMetadata()
{
}


Source::Source(const Source &other)
    : EntityWithMetadata(other.impl())
{
}


Source::Source(const std::shared_ptr<base::ISource> &p_impl)
    : EntityWithMetadata(p_impl)
{
}


Source::Source(std::shared_ptr<base::ISource> &&ptr)
    : EntityWithMetadata(std::move(ptr))
{
}

//--------------------------------------------------
// Methods concerning child sources
//--------------------------------------------------

Source Source::createSource(const std::string &name, const std::string &type) {
    util::checkEntityNameAndType(name, type);
    if(backend()->hasSource(name)) {
        throw DuplicateName("Source with the given name already exists!");
    }
    return backend()->createSource(name, type);
}


bool Source::hasSource(const Source &source) const {
    if (!util::checkEntityInput(source, false)) {
        return  false;
    }
    return backend()->hasSource(source.id());
}


std::vector<Source> Source::sources(const util::Filter<Source>::type &filter) const {
    auto f = [this] (ndsize_t i) { return getSource(i); };
    return getEntities<Source>(f, sourceCount(), filter);
}


bool Source::deleteSource(const Source &source) {
    if (!util::checkEntityInput(source, false)) {
        return  false;
    }
    return backend()->deleteSource(source.id());
}



struct SourceCont {
    Source entity;
    size_t depth;
};


std::vector<Source> Source::findSources(const util::Filter<Source>::type &filter,
                                        size_t max_depth) const
{
    std::vector<Source>  results;
    std::queue<SourceCont> todo;

    todo.push({*this, 0});

    while (todo.size() > 0)
    {
        SourceCont current = todo.front();
        todo.pop();

        bool filter_ok = filter(current.entity);
        if (filter_ok) {
            results.push_back(current.entity);
        }

        if (current.depth < max_depth) {
            std::vector<Source> children = current.entity.sources();
            size_t next_depth = current.depth + 1;

            for (auto it = children.begin(); it != children.end(); ++it) {
                todo.push({*it, next_depth});
            }
        }
    }

    return results;
}

//------------------------------------------------------
// Operators and other functions
//------------------------------------------------------


std::vector<nix::DataArray> Source::referringDataArrays() const {
    std::vector<nix::DataArray> arrays;
    nix::File f = backend()->parentFile();
    for (auto b : f.blocks()) {
        std::vector<nix::DataArray> temp = referringDataArrays(b);
        arrays.insert(arrays.end(), temp.begin(), temp.end());
    }
    return arrays;
}


std::vector<nix::DataArray> Source::referringDataArrays(const nix::Block & block) const {
    std::vector<nix::DataArray> arrays;
    if (block) {
        arrays = block.dataArrays(nix::util::SourceFilter<nix::DataArray>(id()));
    }
    return arrays;
}


std::vector<nix::Tag> Source::referringTags() const {
    std::vector<nix::Tag> tags;
    nix::File f = backend()->parentFile();
    for (auto b : f.blocks()) {
        std::vector<nix::Tag> temp = referringTags(b);
        tags.insert(tags.end(), temp.begin(), temp.end());
    }
    return tags;
}


std::vector<nix::Tag> Source::referringTags(const Block &b) const {
    std::vector<nix::Tag> tags;
    if (b) {
        tags = b.tags(nix::util::SourceFilter<nix::Tag>(id()));
    }
    return tags;
}


std::vector<nix::MultiTag> Source::referringMultiTags() const {
    std::vector<nix::MultiTag> tags;
    nix::File f = backend()->parentFile();
    for (auto b : f.blocks()) {
        std::vector<nix::MultiTag> temp = referringMultiTags(b);
        tags.insert(tags.end(), temp.begin(), temp.end());
    }
    return tags;
}


std::vector<nix::MultiTag> Source::referringMultiTags(const Block &b) const {
    std::vector<nix::MultiTag> tags;
    if (b) {
        tags = b.multiTags(nix::util::SourceFilter<nix::MultiTag>(id()));
    }
    return tags;
}


std::ostream& nix::operator<<(std::ostream &out, const Source &ent) {
    out << "Source: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}

