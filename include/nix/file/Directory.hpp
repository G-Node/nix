#ifndef NIX_DIRECTORY_H
#define NIX_DIRECTORY_H

#include <boost/filesystem.hpp>
#include <nix/file/AttributesFS.hpp>

#include <string>
#include <vector>

namespace nix {
namespace file {

class NIXAPI Directory {
    private:
    AttributesFS attributes;

    public:
    Directory (const boost::filesystem::path &parent, const std::string &name);




};

}
}

#endif //NIX_DIRECTORY_H
