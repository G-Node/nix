// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "NamedEntityFS.hpp"

namespace bfs = boost::filesystem;

namespace nix {
namespace file {


NamedEntityFS::NamedEntityFS(const std::shared_ptr<base::IFile> &file, const bfs::path &loc)
    : EntityFS(file, loc)
{
}

NamedEntityFS::NamedEntityFS(const std::shared_ptr<base::IFile> &file, const std::string &loc)
	: NamedEntityFS(file, bfs::path(loc))
{
}


NamedEntityFS::NamedEntityFS(const std::shared_ptr<base::IFile> &file, const bfs::path &loc, const std::string &id,
                             const std::string &type, const std::string &name)
    : NamedEntityFS(file, loc, id, type, name, util::getTime())
{
}

NamedEntityFS::NamedEntityFS(const std::shared_ptr<base::IFile> &file, const std::string &loc, const std::string &id,
	const std::string &type, const std::string &name)
	: NamedEntityFS(file, bfs::path(loc), id, type, name)
{
}


NamedEntityFS::NamedEntityFS(const std::shared_ptr<base::IFile> &file, const bfs::path &loc, const std::string &id,
                             const std::string &type, const std::string &name, time_t time)
    : EntityFS(file, (bfs::path(loc.c_str()) / bfs::path(name.c_str())), id, time)
{
    this->type(type);
    if (name.empty()) {
        throw EmptyString("name");
    } else {
        setAttr("name", name);
        forceUpdatedAt();
    }
}


NamedEntityFS::NamedEntityFS(const std::shared_ptr<base::IFile> &file, const std::string &loc, const std::string &id,
	const std::string &type, const std::string &name, time_t time)
	: NamedEntityFS(file, bfs::path(loc), id, type, name, time)
{
}


void NamedEntityFS::type(const std::string &type) {
    if (type.empty()) {
        throw EmptyString("type");
    } else {
        setAttr("type", type);
        forceUpdatedAt();
    }
}


std::string NamedEntityFS::type() const {
    std::string type;
    if (hasAttr("type")) {
        getAttr("type", type);
        return type;
    } else {
        throw MissingAttr("type");
    }
}


std::string NamedEntityFS::name() const {
    std::string name;
    if (hasAttr("name")) {
        getAttr("name", name);
        return name;
    } else {
        throw MissingAttr("name");
    }
}


void NamedEntityFS::definition(const std::string &definition) {
    if (definition.empty()) {
        throw EmptyString("definition");
    } else {
        setAttr("definition", definition);
        forceUpdatedAt();
    }
}


boost::optional<std::string> NamedEntityFS::definition() const {
    boost::optional<std::string> ret;
    std::string definition;
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


int NamedEntityFS::compare(const std::shared_ptr<base::INamedEntity> &other) const {
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