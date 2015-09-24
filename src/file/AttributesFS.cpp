// Copyright (c) 2013-2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "nix/file/AttributesFS.hpp"

namespace bfs = boost::filesystem;
namespace y = YAML;

namespace nix {
namespace file {

#define ATTRIBUTES_FILE std::string("attributes")

AttributesFS::AttributesFS() { }


AttributesFS::AttributesFS(const std::string &file_path, FileMode mode)
    : AttributesFS(bfs::path(file_path.c_str()), mode)
{ }

AttributesFS::AttributesFS(const bfs::path &file_path, FileMode mode)
    : loc(file_path), mode(mode) {
    if (bfs::exists(file_path)) {
        open_or_create();
    }
}


void AttributesFS::open_or_create() {
    bfs::path attr(ATTRIBUTES_FILE);
    bfs::path temp = location() / attr;
    if (!bfs::exists(temp)) {
        if (mode > FileMode::ReadOnly) {
            std::ofstream ofs;
            ofs.open(temp.string(), std::ofstream::out | std::ofstream::app);
            ofs.close();
        } else {
            throw std::logic_error("Trying to create new attributes in ReadOnly mode!");
        }
    }
    node = y::LoadFile(temp.string());
}


bool AttributesFS::has(const std::string &name) {
    open_or_create();
    return (node.size() > 0) && (node[name]);
}


void AttributesFS::flush() {
    std::ofstream ofs;
    bfs::path temp = location() / bfs::path(ATTRIBUTES_FILE);
    ofs.open(temp.string(), std::ofstream::trunc);
    if (ofs.is_open()) {
        ofs << node << std::endl;
    }
    else {
        throw std::runtime_error("Could not write to attributes file!");
    }
    ofs.close();
}

bfs::path AttributesFS::location() const {
    return loc;
}

nix::ndsize_t AttributesFS::attributeCount() {
    open_or_create();
    return node.size();
}

void AttributesFS::remove(const std::string &name) {
    open_or_create();
    if (mode == FileMode::ReadOnly) {
        throw std::logic_error("Trying to remove an attributes in ReadOnly mode!");
    }
    if (node[name]) {
        node.remove(name);
    }
    flush();
}

} //namespace file
} //namespace nix