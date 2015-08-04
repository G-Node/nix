#ifndef NIX_DIRECTORY_H
#define NIX_DIRECTORY_H

#include <boost/filesystem.hpp>
#include <nix/file/AttributesFS.hpp>

#include <string>
#include <vector>

namespace nix {
namespace file {

class Directory {

private:
    boost::filesystem::path loc;
    mutable AttributesFS attributes;

    void open_or_create();

public:
    Directory (const boost::filesystem::path &parent, const std::string &name);

    template <typename T> void setAttr(const std::string &name, const T &value);

    template <typename T> void getAttr(const std::string &name, T &value) const;

    bool hasAttr(const std::string &name) const;

    std::string location() const;
};


template <typename T>
void Directory::setAttr(const std::string &name, const T &value) {
    attributes.set(name, value);
}

template <typename T>
void Directory::getAttr(const std::string &name, T &value) const {
    attributes.get(name, value);
}

}
}

#endif //NIX_DIRECTORY_H
