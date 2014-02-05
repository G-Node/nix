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


//------------------------------------------------------
// Operators and other functions
//------------------------------------------------------

std::ostream& nix::operator<<(ostream &out, const Section &ent) {
    out << "Section: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}
