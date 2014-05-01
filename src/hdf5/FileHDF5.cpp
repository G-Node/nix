// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <fstream>
#include <vector>
#include <ctime>

#include <nix/util/util.hpp>
#include <nix/hdf5/FileHDF5.hpp>
#include <nix/hdf5/BlockHDF5.hpp>
#include <nix/hdf5/SectionHDF5.hpp>

using namespace std;

namespace nix {
namespace hdf5 {


// Format definition
#define FILE_VERSION std::string("1.0")
#define FILE_FORMAT  std::string("pandora")


static unsigned int map_file_mode(FileMode mode) {
    switch (mode) {
        case FileMode::ReadWrite:
            return H5F_ACC_RDWR;

        case FileMode::ReadOnly:
            return H5F_ACC_RDONLY;

        case FileMode::Overwrite:
            return H5F_ACC_TRUNC;

        default:
            return H5F_ACC_DEFAULT;
    }

}


FileHDF5::FileHDF5(const string &name, FileMode mode)
{
    if (!fileExists(name)) {
        mode = FileMode::Overwrite;
    }

    unsigned int h5mode =  map_file_mode(mode);
    h5file = H5::H5File(name.c_str(), h5mode);

    root = Group(h5file.openGroup("/"));
    metadata = root.openGroup("metadata");
    data = root.openGroup("data");

    setCreatedAt();
    setUpdatedAt();

    if(!checkHeader()) {
        throw std::runtime_error("Invalid file header: either file format or file version not correct");
    }
}


FileHDF5::FileHDF5(const FileHDF5 &file)
    : h5file(file.h5file), root(file.root), metadata(file.metadata), data(file.data)
{}


bool FileHDF5::hasBlock(const std::string &id) const {
    return data.hasGroup(id);
}


Block FileHDF5::getBlock(const std::string &id) const {
    if(hasBlock(id)) {
        Group group = data.openGroup(id, false);
        std::string type;
        std::string name;
        group.getAttr("type", type);
        group.getAttr("name", name);
        shared_ptr<BlockHDF5> ptr(new BlockHDF5(file(), group, id, type, name));
        return Block(ptr);
    } else {
        return Block();
    }
}


Block FileHDF5::getBlock(size_t index) const {
    string id = data.objectName(index);
    return getBlock(id);
}


Block FileHDF5::createBlock(const std::string &name, const string &type) {
    string id = util::createId("block");
    
    while(data.hasObject(id)) {
        id = util::createId("block");
    }
        
    Group group = data.openGroup(id, true);
    shared_ptr<BlockHDF5> ptr(new BlockHDF5(file(), group, id, type, name));
    Block b(ptr);
    
    return b;
}


bool FileHDF5::deleteBlock(const std::string &id) {
    if (data.hasGroup(id)) {
        data.removeGroup(id);
        return true;
    } else {
        return false;
    }
}


size_t FileHDF5::blockCount() const {
    return data.objectCount();
}


bool FileHDF5::hasSection(const std::string &id) const {
    return metadata.hasGroup(id);
}


Section FileHDF5::getSection(const std::string &id) const {
    if(hasSection(id)) {
        Group group = metadata.openGroup(id, false);
        std::string type;
        std::string name;
        group.getAttr("type", type);
        group.getAttr("name", name);
        shared_ptr<SectionHDF5> ptr(new SectionHDF5(file(), group, id, type, name));
        return Section(ptr);
    } else {
        return Section();
    }
}


Section FileHDF5::getSection(size_t index) const{
    string id = metadata.objectName(index);    
    return getSection(id);
}


Section FileHDF5::createSection(const string &name, const  string &type) {
    string id = util::createId("section");
    while(metadata.hasObject(id))
        id = util::createId("section");
    shared_ptr<SectionHDF5> ptr(new SectionHDF5(file(), metadata.openGroup(id, true), id, type, name));
    Section section(ptr);
    return section;
}


bool FileHDF5::deleteSection(const std::string &id) {
    if (metadata.hasGroup(id)) {
        metadata.removeGroup(id);
        return true;
    } else {
        return false;
    }
}


size_t FileHDF5::sectionCount() const {
    return metadata.objectCount();
}


time_t FileHDF5::updatedAt() const {
    string t;
    root.getAttr("updated_at", t);
    return util::strToTime(t);
}


void FileHDF5::setUpdatedAt() {
    if (!root.hasAttr("updated_at")) {
        time_t t = time(NULL);
        root.setAttr("updated_at", util::timeToStr(t));
    }
}


void FileHDF5::forceUpdatedAt() {
    time_t t = time(NULL);
    root.setAttr("updated_at", util::timeToStr(t));
}


time_t FileHDF5::createdAt() const {
    string t;
    root.getAttr("created_at", t);
    return util::strToTime(t);
}


void FileHDF5::setCreatedAt() {
    if (!root.hasAttr("created_at")) {
        time_t t = time(NULL);
        root.setAttr("created_at", util::timeToStr(t));
    }
}


void FileHDF5::forceCreatedAt(time_t t) {
    root.setAttr("created_at", util::timeToStr(t));
}


string FileHDF5::version() const {
    string t;
    root.getAttr<std::string>("version", t);
    return t;
}


string FileHDF5::format() const {
    string t;
    root.getAttr("format", t);
    return t;
}


void FileHDF5::close() {

    if (!isOpen())
        return;

    unsigned types = H5F_OBJ_GROUP|H5F_OBJ_DATASET|H5F_OBJ_DATATYPE;

    hsize_t obj_count = h5file.getObjCount(types);
    hid_t*  objs = new hid_t[obj_count];
    h5file.getObjIDs(types, obj_count, objs);


    for (hsize_t i = 0; i < obj_count; i++) {
        hid_t   obj = objs[i];
        hsize_t ref_count = H5Iget_ref(obj);

        for (hsize_t j = 0; j < ref_count; j++) {
            H5Oclose(obj);
        }
    }

    delete[] objs;

    h5file.close();
}


bool FileHDF5::isOpen() const {
    hid_t file_id = h5file.getId();

    if (file_id == 0)
        return false;

    H5I_type_t id_type = H5Iget_type(file_id);

    if (id_type <= H5I_BADID || id_type >= H5I_NTYPES)
        return false;
    else
        return true;
}


File FileHDF5::file() const {
    shared_ptr<FileHDF5> ptr = const_pointer_cast<FileHDF5>(shared_from_this());
    return File(ptr);
}


bool FileHDF5::checkHeader() const {
    bool check = true;
    string str;
    // check format
    if (root.hasAttr("format")) {
        if (!root.getAttr("format", str) || str != FILE_FORMAT) {
            check = false;
        }
    } else {
        root.setAttr("format", FILE_FORMAT);
    }
    // check version
    if (root.hasAttr("version")) {
        if (!root.getAttr("version", str) || str != FILE_VERSION) {
            check = false;
        }
    } else {
        root.setAttr("version", FILE_VERSION);
    }
    return check;
}


bool FileHDF5::fileExists(const string &name) const {
    ifstream f(name.c_str());
    if (f) {
        f.close();
        return true;
    } else {
        return false;
    }
}


bool FileHDF5::operator==(const FileHDF5 &other) const {
    return h5file.getFileName() == other.h5file.getFileName();
}


bool FileHDF5::operator!=(const FileHDF5 &other) const {
    return h5file.getFileName() != other.h5file.getFileName();
}


FileHDF5& FileHDF5::operator=(const FileHDF5 &other) {
    if (*this != other) {
        this->h5file = other.h5file;
        this->root = other.root;
        this->metadata = other.metadata;
        this->data = other.data;
    }
    return *this;
}


FileHDF5::~FileHDF5() {
    close();
}


} // namespace hdf5
} // namespace nix

