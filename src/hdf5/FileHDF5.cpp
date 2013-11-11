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
        /// TODO throw an exception here
    }
}


FileHDF5::FileHDF5(const FileHDF5 &file)
    : h5file(file.h5file), root(file.root), metadata(file.metadata), data(file.data)
{}


bool FileHDF5::hasBlock(const std::string &id) const {
    return data.hasGroup(id);
}


Block FileHDF5::getBlock(const std::string &id) const {
    shared_ptr<BlockHDF5> ptr(new BlockHDF5(file(), data.openGroup(id, false), id));
    return Block(ptr);
}


Block FileHDF5::getBlock(size_t index) const {
    string id = data.objectName(index);
    return getBlock(id);
}


vector<Block> FileHDF5::blocks() const {
    vector<Block>  block_obj;

    size_t block_count = data.objectCount();
    for (size_t i = 0; i < block_count; i++) {
        string id = data.objectName(i);
        shared_ptr<BlockHDF5> ptr(new BlockHDF5(file(), data.openGroup(id, false), id));
        block_obj.push_back(Block(ptr));
    }

    return block_obj;
}


Block FileHDF5::createBlock(const std::string &name, const string &type) {
    string id = util::createId("block");
    while(data.hasObject(id))
        id = util::createId("block");
    shared_ptr<BlockHDF5> ptr(new BlockHDF5(file(), data.openGroup(id, true), id));
    Block b(ptr);
    b.name(name);
    b.type(type);
    return b;
}


bool FileHDF5::removeBlock(const std::string &id) {
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


std::vector<Section> FileHDF5::sections()const{
    vector<Section>  section_obj;
    size_t section_count = metadata.objectCount();
    for (size_t i = 0; i < section_count; i++) {
        string id = metadata.objectName(i);
        shared_ptr<SectionHDF5> ptr(new SectionHDF5(file(), metadata.openGroup(id, false), id));
        Section section(ptr);
        section_obj.push_back(section);
    }
    return section_obj;
}


bool FileHDF5::hasSection(const std::string &id) const{
    return metadata.hasGroup(id);
}


Section FileHDF5::getSection(const std::string &id) const{
    shared_ptr<SectionHDF5> ptr(new SectionHDF5(file(), metadata.openGroup(id, false), id));
    return Section(ptr);
}


Section FileHDF5::getSection(size_t index) const{
    string id = data.objectName(index);
    return getSection(id);
}


std::vector<Section> FileHDF5::findSection(const std::string &id) const{
    vector<Section> s = sections();
    vector<Section> sects;
    for(size_t i = 0; i < s.size(); i++){
        if(s[i].id().compare(id)==0){
            sects.push_back(s[i]);
            return sects;
        }
    }
    // TODO uncomment end fix error
    /*
    for(size_t i = 0; i < s.size(); i++){
        sects = s[i].collectIf([&](const Section &section) {
            bool found = section.id() == id;
            return found;
        });
        if (sects.size() > 0){
            return sects;
        }
    }
    */
    return sects;
}


Section FileHDF5::createSection(const string &name, const  string &type) {
    string id = util::createId("section");
    while(metadata.hasObject(id))
        id = util::createId("section");
    shared_ptr<SectionHDF5> ptr(new SectionHDF5(file(), metadata.openGroup(id, true), id));
    Section section(ptr);
    section.name(name);
    section.type(type);
    return section;
}


bool FileHDF5::removeSection(const std::string &id){
    bool success = false;

    std::vector<Section> sects = findSection(id);
    if(!sects.empty()){
        metadata.removeGroup(id);
        success = true;
    }

    return success;
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
    setUpdatedAt();
    h5file.close();
}


} // namespace hdf5
} // namespace nix

