//
// Created by jan on 7/30/15.
//

#include <nix/file/FilesystemAttribute.hpp>

using namespace nix::file;
using namespace boost::filesystem;
using namespace YAML;

FilesystemAttribute::FilesystemAttribute() { }


FilesystemAttribute::FilesystemAttribute(const std::string &file_path) {
    path temp(file_path.c_str());
    if (exists(temp)) {
        this->loc = temp;
        this->open_or_create();
   }
}


void FilesystemAttribute::open_or_create() {
    path attr(".attributes");
    path temp = this->location() /  attr;
    if (!exists(temp)) {
        std::ofstream ofs;
        ofs.open(this->location().string() + "/.attributes", std::ofstream::out | std::ofstream::app);
        ofs.close();
    }
    this->node = Load(this->location().string() + "/.attributes");
}


path FilesystemAttribute::location() const {
    return this->loc;
}
