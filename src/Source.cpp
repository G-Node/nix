// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <queue>

#include <nix/Source.hpp>

using namespace std;
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


bool Source::hasSource(const Source &source) const {
    if (source == none) {
        throw std::runtime_error("Source::hasSource: emtpy Source entity given!");
    }
    return backend()->hasSource(source.id());
}


std::vector<Source> Source::sources(const util::Filter<Source>::type &filter) const {
    auto f = [this] (size_t i) { return getSource(i); };
    return getEntities<Source>(f,
                               sourceCount(),
                               filter);
}


bool Source::deleteSource(const Source &source) {
    if (source == none) {
        throw std::runtime_error("Source::deleteSource: empty Source entity given!");
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


std::ostream& nix::operator<<(ostream &out, const Source &ent) {
    out << "Source: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}
