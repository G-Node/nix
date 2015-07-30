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

class FilesystemAttribute {

private:
    YAML::Node node;
    boost::filesystem::path loc;

    void open_or_create();

public:
    FilesystemAttribute();

    FilesystemAttribute(const std::string &file_path);
    // FilesystemAttribute(const nix::file::FilesystemAttribute &other);

    boost::filesystem::path location() const;

    template <typename T> void read(const std::string &name, T &value);

    template <typename T> bool write(const std::string &name, const T &value);

};

template <typename T> void FilesystemAttribute::read(const std::string &name, T &value) {
    open_or_create();

    return 0;
}

template <typename T> bool FilesystemAttribute::write(const std::string &name, const T &value) {
    this->open_or_create();

    return true;
}

} // namespace file
} // namespace nix

#endif //NIX_ATTRIBUTE_HPP
