//
// Created by jan on 7/26/15.
//

#include <nix/filesys/FileFS.hpp>
#include <nix/util/util.hpp>
#include <iostream>
#include "../../include/nix/NDSize.hpp"

// #include <nix/filesys/BlockFS.hpp>
// #include <nix/filesys/SectionFS.hpp>
using namespace std;

namespace nix {
namespace filesys {

// Format definition
#define FILE_VERSION std::vector<int>{1, 0, 0}
#define FILE_FORMAT  std::string("nix")

static unsigned int map_file_mode(FileMode mode) {
    switch (mode) {
        case FileMode::ReadWrite:
            return ios::in | ios::out;

        case FileMode::ReadOnly:
            return ios::out;

        case FileMode::Overwrite:
            return ios::trunc;

        default:
            return ios::out;
    }

}


FileFS::FileFS(const string &name, FileMode mode)
{
    boost::filesystem::path p(name.c_str());
    if (!boost::filesystem::exists(p)) {
        mode = FileMode::Overwrite;
    }

    this->file_mode = map_file_mode(mode);
    this->root_path = p;

   /*
    if (h5mode & H5F_ACC_TRUNC) {
        hid = H5Fcreate(name.c_str(), h5mode, fcpl.h5id(), H5P_DEFAULT);
    } else {
        hid = H5Fopen(name.c_str(), h5mode, H5P_DEFAULT);
    }

    if (!H5Iis_valid(hid)) {
        throw H5Exception("Could not open/create file");
    }

    root = Group(H5Gopen2(hid, "/", H5P_DEFAULT));
    root.check("Could not root group");

    metadata = root.openGroup("metadata");
    data = root.openGroup("data");

    setCreatedAt();
    setUpdatedAt();

    if (!checkHeader()) {
        throw std::runtime_error("Invalid file header: either file format or file version not correct");
    }
    */
}

void FileFS::open_or_create() {
    this->root_path += "data";
    this->root_path += "metadata";
    if (!boost::filesystem::exists(this->root_path)) {
        boost::filesystem::create_directory(this->root_path);
        boost::filesystem::create_directory(this->data_path);
        boost::filesystem::create_directory(this->metadata_path);
    } else {
        if (!boost::filesystem::exists(this->data_path)) {
            boost::filesystem::create_directory(this->data_path);
        }
        if (!boost::filesystem::exists(this->metadata_path)) {
            boost::filesystem::create_directory(this->metadata_path);
        }
    }
}


ndsize_t FileFS::blockCount() const {
    ndsize_t count = 0;

    return count;
}


} // namspace filesys
} // namespace nix