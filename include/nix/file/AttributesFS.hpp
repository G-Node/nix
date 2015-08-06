//
// Created by jan on 7/30/15.
//

#ifndef NIX_ATTRIBUTE_HPP
#define NIX_ATTRIBUTE_HPP

#include <yaml-cpp/yaml.h>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>

#include <nix/Platform.hpp>
#include <nix/NDSize.hpp>
#include <nix/base/IFile.hpp>

namespace nix {
namespace file {

class AttributesFS {

private:
    boost::filesystem::path loc;
    FileMode mode;
    YAML::Node node;

    void open_or_create();

    void flush();

public:
    AttributesFS();

    AttributesFS(const std::string &file_path, FileMode mode = FileMode::ReadOnly);

    AttributesFS(const boost::filesystem::path &file_path, FileMode mode = FileMode::ReadOnly);

    // AttributesFS(const nix::file::AttributesFS &other);

    boost::filesystem::path location() const;

    bool has(const std::string &name);

    void remove(const std::string &name);

    template <typename T> void get(const std::string &name, T &value);

    template <typename T> void set(const std::string &name, const T &value);

    ndsize_t attributeCount();
};

template <typename T> void AttributesFS::get(const std::string &name, T &value) {
    open_or_create();
    if (has(name)) {
        value = node[name].as<T>();
    }
}

template <typename T> void AttributesFS::set(const std::string &name, const T &value) {
    open_or_create();
    if (mode == FileMode::ReadOnly) {
        throw std::logic_error("Trying to set an attributes in ReadOnly mode!");
    }
    if (node[name]) {
        node.remove(name);
    }
    this->node[name] = value;
    this->flush();
}

} // namespace file
} // namespace nix

#endif //NIX_ATTRIBUTE_HPP
