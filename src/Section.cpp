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

Section::Section()
    : NamedEntity()
{
}


Section::Section(const Section &other)
    : NamedEntity(other.impl_ptr)
{
}


Section::Section(const shared_ptr<base::ISection> &p_impl)
    : NamedEntity(p_impl)
{
}


//-----------------------------------------------------
// Methods concerning child sections
//-----------------------------------------------------


struct SectionCont {

    SectionCont(Section s, size_t d = 0)
        : section(s), depth(d)
    {}

    Section section;
    size_t depth;
};


std::vector<Section> Section::findSections(std::function<bool (const Section &)> filter,
                                           size_t max_depth) const {
    vector<Section>   results;
    list<SectionCont> todo;

    todo.push_back(SectionCont(*this));

    while(todo.size() > 0) {

        SectionCont current = todo.front();
        todo.pop_front();

        bool filter_ok = filter(current.section);
        if (filter_ok) {
            results.push_back(current.section);
        }

        if (current.depth < max_depth) {
            vector<Section> children = current.section.sections();
            size_t next_depth = current.depth + 1;

            for (auto it = children.begin(); it != children.end(); ++it) {
                todo.push_back(SectionCont(*it, next_depth));
            }
        }

    }

    return results;
}


//------------------------------------------------------
// Operators and other functions
//------------------------------------------------------


std::ostream& nix::operator<<(ostream &out, const Section &ent) {
    out << "Section: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}
