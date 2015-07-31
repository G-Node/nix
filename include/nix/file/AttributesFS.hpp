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

namespace nix {
namespace file {

class AttributesFS {

private:
    YAML::Node node;
    boost::filesystem::path loc;

    void open_or_create();

    void write_file();

public:
    AttributesFS();

    AttributesFS(const std::string &file_path);
    // AttributesFS(const nix::file::AttributesFS &other);

    boost::filesystem::path location() const;

    bool hasField(const std::string &name);

    void remove(const std::string &name);

    template <typename T> void read(const std::string &name, T &value);

    template <typename T> void write(const std::string &name, const T &value);

    ndsize_t attributeCount();
};

template <typename T> void AttributesFS::read(const std::string &name, T &value) {
    T result;
    this->open_or_create();
    if (hasField(name)) {
        return this->node[name].as<T>();
    }
    return 0;
}

template <typename T> void AttributesFS::write(const std::string &name, const T &value) {
    this->open_or_create();
    this->node[name] = value;
    this->write_file();
    std::cerr << this->node << std::endl;
}

} // namespace file
} // namespace nix

#endif //NIX_ATTRIBUTE_HPP
