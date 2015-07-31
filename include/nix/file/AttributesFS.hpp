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

namespace nix {
namespace file {

class AttributesFS {

private:
    YAML::Node node;
    boost::filesystem::path loc;

    void open_or_create();

    void flush();

public:
    AttributesFS();

    AttributesFS(const std::string &file_path);
    // AttributesFS(const nix::file::AttributesFS &other);

    boost::filesystem::path location() const;

    bool has(const std::string &name);

    void remove(const std::string &name);

    template <typename T> void get(const std::string &name, T &value);

    template <typename T> void set(const std::string &name, const T &value);

    ndsize_t attributeCount();
};

template <typename T> void AttributesFS::get(const std::string &name, T &value) {
    this->open_or_create();
    if (has(name)) {
        value = this->node[name].as<T>();
    }
}

template <typename T> void AttributesFS::set(const std::string &name, const T &value) {
    this->open_or_create();
    if (this->node[name]) {
        this->node.remove(name);
    }
    this->node[name] = value;
    this->flush();
}

} // namespace file
} // namespace nix

#endif //NIX_ATTRIBUTE_HPP
