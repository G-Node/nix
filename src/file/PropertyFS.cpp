// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/file/PropertyFS.hpp>

namespace bfs = boost::filesystem;

namespace nix {
namespace file {

PropertyFS::PropertyFS(const std::shared_ptr<base::IFile> &file, const bfs::path &loc)
    : DirectoryWithAttributes(loc, file->fileMode())
{
}


PropertyFS::PropertyFS(const std::shared_ptr<base::IFile> &file, const bfs::path &loc, const std::string &id,
                       const std::string &name, const DataType &dataType)
    : DirectoryWithAttributes(loc / bfs::path(name), file->fileMode())
{
    if (name.empty()) {
        throw EmptyString("name");
    } else {
        setAttr("name", name);
        forceUpdatedAt();
    }

    setAttr("entity_id", id);
    setAttr("data_type", nix::data_type_to_string(dataType));
    time_t t = util::getTime();
    setUpdatedAt();
    forceCreatedAt(t);
}


std::string PropertyFS::id() const {
    std::string t;
    if (hasAttr("entity_id")) {
        getAttr("entity_id", t);
    }
    else {
        throw std::runtime_error("Entity has no id!");
    }
    return t;
}


time_t PropertyFS::updatedAt() const {
    std::string t;
    getAttr("updated_at", t);
    return util::strToTime(t);
}


void PropertyFS::setUpdatedAt() {
    if (!hasAttr("updated_at")) {
        time_t t = util::getTime();
        setAttr("updated_at", util::timeToStr(t));
    }
}


void PropertyFS::forceUpdatedAt() {
    time_t t = util::getTime();
    setAttr("updated_at", util::timeToStr(t));
}


time_t PropertyFS::createdAt() const {
    std::string t;
    getAttr("created_at", t);
    return util::strToTime(t);
}


void PropertyFS::setCreatedAt() {
    if (!hasAttr("created_at")) {
        time_t t = util::getTime();
        setAttr("created_at", util::timeToStr(t));
    }
}


void PropertyFS::forceCreatedAt(time_t t) {
    setAttr("created_at", util::timeToStr(t));
}


std::string PropertyFS::name() const {
    std::string name;
    if (hasAttr("name")) {
        getAttr("name", name);
        return name;
    } else {
        throw MissingAttr("name");
    }
}


void PropertyFS::definition(const std::string &definition) {
    if (definition.empty()) {
        throw EmptyString("definition");
    } else {
        setAttr("definition", definition);
        forceUpdatedAt();
    }
}


boost::optional<std::string> PropertyFS::definition() const {
    boost::optional<std::string> ret;
    std::string definition;
    if (hasAttr("definition")) {
        getAttr("definition", definition);
        ret = definition;
    }
    return ret;
}


void PropertyFS::definition(const nix::none_t t) {
    if (hasAttr("definition")) {
        removeAttr("definition");
    }
    forceUpdatedAt();
}


DataType PropertyFS::dataType() const {
    std::string dtype;
    getAttr("data_type", dtype);
    return nix::string_to_data_type(dtype);
}


void PropertyFS::mapping(const std::string &mapping) {
    if (mapping.empty()) {
        throw EmptyString("mapping");
    }
    setAttr("mapping", mapping);
    forceUpdatedAt();
}


boost::optional<std::string> PropertyFS::mapping() const {
    boost::optional<std::string> ret;
    std::string mapping;
    if (hasAttr("mapping")) {
        getAttr("mapping", mapping);
        ret = mapping;
    }
    return ret;
}


void PropertyFS::mapping(const nix::none_t t) {
    if (hasAttr("mapping")) {
        removeAttr("mapping");
        forceUpdatedAt();
    }
}


void PropertyFS::unit(const std::string &unit) {
    if (unit.empty()) {
        throw EmptyString("unit");
    }
    setAttr("unit", unit);
    forceUpdatedAt();
}


boost::optional<std::string> PropertyFS::unit() const {
    boost::optional<std::string> ret;
    std::string unit;
    if (hasAttr("unit")) {
        getAttr("unit", unit);
        ret = unit;
    }
    return ret;
}


void PropertyFS::unit(const nix::none_t t) {
    if (hasAttr("unit")) {
        removeAttr("unit");
    }
    forceUpdatedAt();
}


void PropertyFS::deleteValues() {
    // FIXME
}


ndsize_t PropertyFS::valueCount() const {
    // FIXME
    return 0;
}


void PropertyFS::values(const std::vector<Value> &values) {
    // FIXME
}


std::vector<Value> PropertyFS::values(void) const {
    std::vector<Value> values;
    // FIXME
    return values;
}


void PropertyFS::values(const nix::none_t t) {
    // TODO: rethink if we want two methods for same thing
    deleteValues();
}


bool PropertyFS::isValidEntity() const {
    return isValid();
}


int PropertyFS::compare(const std::shared_ptr<base::IProperty> &other) const {
    int cmp = 0;
    if (!name().empty() && !other->name().empty()) {
        cmp = (name()).compare(other->name());
    }
    if (cmp == 0) {
        cmp = id().compare(other->id());
    }
    return cmp;
}


PropertyFS::~PropertyFS() {}

} // ns nix::file
} // ns nix
