// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#ifndef NIX_FILTER_H
#define NIX_FILTER_H

#include <functional>
#include <vector>
#include <unordered_set>
#include <string>

namespace nix {
namespace util {

/**
 * Base struct to be inherited by all filter implementations.
 * Child classes will have to implement ()-operator and will
 * all inherit typedef "type" which corresponds to the type of "()".
 */
template<typename T>
struct Filter : public std::unary_function<T, bool> {

    virtual bool operator()(const T&) = 0;

    typedef std::function<bool(const T&)> type;

};


/**
 * One Filter struct to that filters nothing but always returns true.
 * Use "AcceptAll<T>()" to pass it on as filter and "::type" to define
 * its' type.
 */
template<typename T>
struct AcceptAll : public Filter<T> {

    virtual bool operator()(const T &e) {
        return true;
    }

};


template<typename T>
struct IdFilter : public Filter<T> {

    const std::string id;


    IdFilter(const std::string &str)
        : id(str)
    {}


    virtual bool operator()(const T &e) {
        return e.id() == id;
    }

};


template<typename T>
struct IdsFilter : public Filter<T> {

    std::unordered_set<std::string> ids;


    IdsFilter(const std::vector<std::string> &ids_vect)
        : ids(ids_vect.begin(), ids_vect.end())
    {}


    virtual bool operator()(const T &e) {
        // std::unordered_set.count is a fast by-hash-getter that
        // returns 1 (if val found) or 0 (if not found).
        return ids.count(e.id()) > 0;
    }

};


template<typename T>
struct TypeFilter : public Filter<T> {

    const std::string type;


    TypeFilter(const std::string &str)
        : type(str)
    {}


    virtual bool operator()(const T &e) {
        return e.type() == type;
    }

};


template<typename T>
struct NameFilter : public Filter<T> {

    const std::string name;


    NameFilter(const std::string &str)
        : name(str)
    {}


    virtual bool operator()(const T &e) {
        return e.name() == name;
    }

};


template<typename T>
struct MetadataFilter : public Filter<T> {

    const std::string sec_id;


    MetadataFilter(const std::string &section_id)
        : sec_id(section_id)
    {}


    virtual bool operator()(const T &e) {
        if (e.metadata()) {
            return e.metadata().id() == sec_id;
        } else {
            return false;
        }

    }
};


template<typename T>
struct SourceFilter : public Filter<T> {

    const std::string src_id;


    SourceFilter(const std::string &src_id)
        : src_id(src_id)
    {}


    virtual bool operator() (const T &ent) {
        if (ent.source()) {
            return ent.source().id() == src_id;
        } else {
            return false;
        }
    }

};

} // namespace util
} // namespace nix

#endif // include guard
