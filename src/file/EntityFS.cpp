// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
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
using namespace boost::filesystem;

namespace nix {
namespace file {


EntityFS::EntityFS(const shared_ptr<IFile> &file, const path &loc)
    : DirectoryWithAttributes(loc, file->fileMode()), entity_file(file)
{
    setUpdatedAt();
    setCreatedAt();
}


EntityFS::EntityFS(const shared_ptr<IFile> &file, const path &loc, const string &id, time_t time)
    : EntityFS(file, loc)
{
    setAttr("entity_id", id);
    forceCreatedAt(time);
}


string EntityFS::id() const {
    string t;

    if (hasAttr("entity_id")) {
        getAttr("entity_id", t);
    }
    else {
        throw runtime_error("Entity has no id!");
    }
    return t;
}


time_t EntityFS::updatedAt() const {
    string t;
    getAttr("updated_at", t);
    return util::strToTime(t);
}


void EntityFS::setUpdatedAt() {
    if (!hasAttr("updated_at")) {
        time_t t = util::getTime();
        setAttr("updated_at", util::timeToStr(t));
    }
}


void EntityFS::forceUpdatedAt() {
    time_t t = util::getTime();
    setAttr("updated_at", util::timeToStr(t));
}


time_t EntityFS::createdAt() const {
    string t;
    getAttr("created_at", t);
    return util::strToTime(t);
}


void EntityFS::setCreatedAt() {
    if (!hasAttr("created_at")) {
        time_t t = util::getTime();
        setAttr("created_at", util::timeToStr(t));
    }
}


void EntityFS::forceCreatedAt(time_t t) {
    setAttr("created_at", util::timeToStr(t));
}

/*
string EntityFS::location() const {
    return location();
}
*/

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