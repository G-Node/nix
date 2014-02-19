// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <list>

#include <nix.hpp>

using namespace std;
using namespace nix;

Source::Source()
    : EntityWithMetadata()
{
}


Source::Source(const Source &other)
    : EntityWithMetadata(other.impl_ptr)
{
}


Source::Source(const std::shared_ptr<base::ISource> &p_impl)
    : EntityWithMetadata(p_impl)
{
}


//--------------------------------------------------
// Methods concerning child sources
//--------------------------------------------------


/*
 * Helper struct for {@link findSources}.
 */
struct SourceCont {
    SourceCont(Source s, size_t d = 0)
        : entity(s), depth(d)
    {}

    Source entity;
    size_t depth;
};


std::vector<Source> Source::findSources(std::function<bool(Source)> filter,
                                size_t max_depth) const 
{
    std::vector<Source>  results;
    std::list<SourceCont> todo;

    todo.push_back(SourceCont(*this));

    while(todo.size() > 0) 
    {
        SourceCont current = todo.front();
        todo.pop_front();

        bool filter_ok = filter(current.entity);
        if (filter_ok) {
            results.push_back(current.entity);
        }

        if (current.depth < max_depth) {
            std::vector<Source> children = current.entity.source();
            size_t next_depth = current.depth + 1;

            for (auto it = children.begin(); it != children.end(); ++it) {
                todo.push_back(SourceCont(*it, next_depth));
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
