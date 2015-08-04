// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/file/NamedEntityFS.hpp>

#include <nix/util/util.hpp>

#include <ctime>

using namespace std;

using namespace nix::base;

namespace nix {
namespace file {


NamedEntityFS::NamedEntityFS(const std::shared_ptr<IFile> &file, const string &loc)
    : EntityFS(file, loc)
{
}


NamedEntityFS::NamedEntityFS(const std::shared_ptr<IFile> &file, const string &loc, const string &id, const string &type,
                                 const string &name)
    : NamedEntityFS(file, loc, id, type, name, util::getTime())
{
}


NamedEntityFS::NamedEntityFS(const std::shared_ptr<IFile> &file, const string &loc, const string &id, const string &type,
                                 const string &name, time_t time)
    : EntityFS(file, loc, id, time)
{
    cerr << "File location:  " << file->location() << endl;
    this->type(type);
    if (name.empty()) {
        throw EmptyString("name");
    } else {
        setAttr("name", name);
        forceUpdatedAt();
    }
}


void NamedEntityFS::type(const string &type) {
    if (type.empty()) {
        throw EmptyString("type");
    } else {
        setAttr("type", type);
        forceUpdatedAt();
    }
}


string NamedEntityFS::type() const {
    string type;
    if (hasAttr("type")) {
        getAttr("type", type);
        return type;
    } else {
        throw MissingAttr("type");
    }
}


string NamedEntityFS::name() const {
    string name;
    if (hasAttr("name")) {
        getAttr("name", name);
        return name;
    } else {
        throw MissingAttr("name");
    }
}


void NamedEntityFS::definition(const string &definition) {
    if (definition.empty()) {
        throw EmptyString("definition");
    } else {
        setAttr("definition", definition);
        forceUpdatedAt();
    }
}


boost::optional<string> NamedEntityFS::definition() const {
    boost::optional<string> ret;
    string definition;
    if (hasAttr("definition")) {
        getAttr("definition", definition);
        ret = definition;
    }
    return ret;
}


void NamedEntityFS::definition(const nix::none_t t) {
    if (hasAttr("definition")) {
        removeAttr("definition");
    }
    forceUpdatedAt();
}


int NamedEntityFS::compare(const std::shared_ptr<INamedEntity> &other) const {
    int cmp = 0;
    if (!name().empty() && !other->name().empty()) {
        cmp = (name()).compare(other->name());
    }
    if (cmp == 0) {
        cmp = id().compare(other->id());
    }
    return cmp;
}


NamedEntityFS::~NamedEntityFS() {}

} // ns nix::file
} // ns nix