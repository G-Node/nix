#ifndef NIX_DIRECTORY_H
#define NIX_DIRECTORY_H

#include <boost/filesystem.hpp>
#include <nix/file/AttributesFS.hpp>
#include <nix/File.hpp>

#include <string>
#include <vector>

namespace nix {
namespace file {

class Directory {

private:
    boost::filesystem::path loc;
    FileMode mode;

    void open_or_create();

public:
    Directory () {};

    Directory (const boost::filesystem::path &location, FileMode mode = FileMode::ReadOnly);

    Directory (const std::string &location, FileMode mode = FileMode::ReadOnly);

    std::string location() const;

    ndsize_t subdir_count() const;
    FileMode fileMode() const;


    boost::filesystem::path sub_dir_by_index(ndsize_t index) const;

    bool hasObject(const std::string &name) const;

    boost::optional<boost::filesystem::path> findByNameOrAttribute(const std::string &attribute, const std::string &value) const;

    bool removeObjectByNameOrAttribute(const std::string &attribute, const std::string &name_or_id) const;

    void removeAll();
};

}
}

#endif //NIX_DIRECTORY_H
