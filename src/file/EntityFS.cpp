// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/file/EntityFS.hpp>
#include <nix/util/util.hpp>

#include <ctime>

using namespace std;
using namespace nix::base;

namespace nix {
namespace file {


EntityFS::EntityFS(const shared_ptr<IFile> &file, const string &loc)
    : entity_file(file), attributes(loc), loc(loc)
{
    setUpdatedAt();
    setCreatedAt();
}


EntityFS::EntityFS(const shared_ptr<IFile> &file, const string &loc, const string &id, time_t time)
    : entity_file(file), attributes(loc), loc(loc)
{
    attributes.set("entity_id", id);
    setUpdatedAt();
    forceCreatedAt(time);
}


string EntityFS::id() const {
    string t;

    if (attributes.has("entity_id")) {
        attributes.get("entity_id", t);
    }
    else {
        throw runtime_error("Entity has no id!");
    }
    return t;
}


time_t EntityFS::updatedAt() const {
    string t;
    attributes.get("updated_at", t);
    return util::strToTime(t);
}


void EntityFS::setUpdatedAt() {
    if (!attributes.has("updated_at")) {
        time_t t = util::getTime();
        attributes.set("updated_at", util::timeToStr(t));
    }
}


void EntityFS::forceUpdatedAt() {
    time_t t = util::getTime();
    attributes.set("updated_at", util::timeToStr(t));
}


time_t EntityFS::createdAt() const {
    string t;
    attributes.get("created_at", t);
    return util::strToTime(t);
}


void EntityFS::setCreatedAt() {
    if (!attributes.has("created_at")) {
        time_t t = util::getTime();
        attributes.set("created_at", util::timeToStr(t));
    }
}


void EntityFS::forceCreatedAt(time_t t) {
    attributes.set("created_at", util::timeToStr(t));
}


string EntityFS::location() const {
    return loc;
}


std::shared_ptr<base::IFile> EntityFS::file() const {
    return entity_file;
}


bool EntityFS::operator==(const EntityFS &other) const {
    return location() == other.location() && id() == other.id();
}


bool EntityFS::operator!=(const EntityFS &other) const {
    return location() != other.location() || id() != other.id();
}


EntityFS::~EntityFS() {}

} // ns nix::file
} // ns nix