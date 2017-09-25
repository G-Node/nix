// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "FileHDF5.hpp"

#include <nix/util/util.hpp>
#include "BlockHDF5.hpp"
#include "SectionHDF5.hpp"
#include "h5x/H5Exception.hpp"


#include <fstream>
#include <vector>
#include <ctime>

using namespace std;

namespace nix {
namespace hdf5 {

static FormatVersion my_version = HDF5_FF_VERSION;

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


    FileHDF5::FileHDF5(const string &name, FileMode mode, Compression compression) {
    if (!fileExists(name)) {
        mode = FileMode::Overwrite;
    }
    this->mode = mode;
    this->compr = compression;
    //we want hdf5 to keep track of the order in which links were created so that
    //the order for indexed based accessors is stable cf. issue #387
    H5Object fcpl = H5Pcreate(H5P_FILE_CREATE);
    fcpl.check("Could not create file creation plist");
    HErr res = H5Pset_link_creation_order(fcpl.h5id(), H5P_CRT_ORDER_TRACKED|H5P_CRT_ORDER_INDEXED);
    res.check("Unable to create file (H5Pset_link_creation_order failed.)");
    unsigned int h5mode =  map_file_mode(mode);

    bool is_create = !fileExists(name) || h5mode == H5F_ACC_TRUNC;

    if (is_create) {
        hid = H5Fcreate(name.c_str(), h5mode, fcpl.h5id(), H5P_DEFAULT);
    } else {
        hid = H5Fopen(name.c_str(), h5mode, H5P_DEFAULT);
    }

    if (!H5Iis_valid(hid)) {
        throw H5Exception("Could not open/create file");
    }

    openRoot();

    if (is_create) {
        createHeader();
    } else if (!checkHeader(mode)) {
        throw nix::InvalidFile("FileHDF5::open_existing!");
    }

    metadata = root.openGroup("metadata");
    data = root.openGroup("data");

    setCreatedAt();
    setUpdatedAt();
}


bool FileHDF5::flush() {
    HErr err = H5Fflush(hid, H5F_SCOPE_GLOBAL);
    return !err.isError();
}

//--------------------------------------------------
// Methods concerning blocks
//--------------------------------------------------


bool FileHDF5::hasBlock(const std::string &name_or_id) const {
    return getBlock(name_or_id) != nullptr;
}


shared_ptr<base::IBlock> FileHDF5::getBlock(const std::string &name_or_id) const {
    shared_ptr<BlockHDF5> block;

    boost::optional<H5Group> group = data.findGroupByNameOrAttribute("entity_id", name_or_id);
    if (group)
        block = make_shared<BlockHDF5>(file(), *group);

    return block;
}


shared_ptr<base::IBlock> FileHDF5::getBlock(ndsize_t index) const {
    string name = data.objectName(index);
    return getBlock(name);
}


shared_ptr<base::IBlock> FileHDF5::createBlock(const string &name, const string &type) {
    string id = util::createId();
    H5Group group = data.openGroup(name, true);
    return make_shared<BlockHDF5>(file(), group, id, type, name, compr);
}


bool FileHDF5::deleteBlock(const std::string &name_or_id) {
    bool deleted = false;

    if (hasBlock(name_or_id)) {
        // we get first "entity" link by name, but delete all others whatever their name with it
        deleted = data.removeAllLinks(getBlock(name_or_id)->name());
    }

    return deleted;
}


ndsize_t FileHDF5::blockCount() const {
    return data.objectCount();
}


//--------------------------------------------------
// Methods concerning sections
//--------------------------------------------------


bool FileHDF5::hasSection(const std::string &name_or_id) const {
    return getSection(name_or_id) != nullptr;
}


shared_ptr<base::ISection> FileHDF5::getSection(const std::string &name_or_id) const {
    shared_ptr<SectionHDF5> sec;

    boost::optional<H5Group> group = metadata.findGroupByNameOrAttribute("entity_id", name_or_id);
    if (group)
        sec = make_shared<SectionHDF5>(file(), *group);

    return sec;
}


shared_ptr<base::ISection> FileHDF5::getSection(ndsize_t index) const{
    string name = metadata.objectName(index);
    return getSection(name);
}


shared_ptr<base::ISection> FileHDF5::createSection(const string &name, const  string &type) {
    string id = util::createId();

    H5Group group = metadata.openGroup(name, true);
    return make_shared<SectionHDF5>(file(), group, id, type, name);
}


bool FileHDF5::deleteSection(const std::string &name_or_id) {
    bool deleted = false;

    // call deleteSection on sections to trigger recursive call to all sub-sections
    if (hasSection(name_or_id)) {
        // get instance of section about to get deleted
        Section section = getSection(name_or_id);
        // loop through all child sections and call deleteSection on them
        for(auto &child : section.sections()) {
            section.deleteSection(child.id());
        }
        // if hasSection is true then section_group always exists
        deleted = metadata.removeAllLinks(section.name());
    }

    return deleted;
}


ndsize_t FileHDF5::sectionCount() const {
    return metadata.objectCount();
}


//--------------------------------------------------
// Local attributes
//--------------------------------------------------


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


vector<int> FileHDF5::version() const {
    vector<int> version;
    root.getAttr("version",version);
    return version;
}


string FileHDF5::format() const {
    string t;
    root.getAttr("format", t);
    return t;
}


string FileHDF5::location() const {
    ssize_t size = H5Fget_name(hid, nullptr, 0);

    if (size < 0) {
        throw H5Exception("H5Fget_name failed");
    }

    std::vector<char> buf(static_cast<size_t>(size + 1), 0);
    size = H5Fget_name(hid, buf.data(), buf.size());
    if (size < 0) {
        throw H5Exception("H5Fget_name failed");
    }

    return std::string(buf.data());
}


void FileHDF5::close() {

    if (!isOpen())
        return;

    data.close();
    metadata.close();
    root.close();

    unsigned types = H5F_OBJ_GROUP|H5F_OBJ_DATASET|H5F_OBJ_DATATYPE;

    ssize_t obj_count = H5Fget_obj_count(hid, types);

    if (obj_count < 0) {
        throw H5Exception("FileHDF5::close(): Could not get object count");
    }

    std::vector<hid_t> objs(static_cast<size_t>(obj_count));

    if (obj_count > 0) {
        obj_count = H5Fget_obj_ids(hid, types, objs.size(), objs.data());

        if (obj_count < 0) {
            throw H5Exception("FileHDF5::close(): Could not get objs");
        }
    }

    for (auto obj : objs) {
        int ref_count = H5Iget_ref(obj);

        for (int j = 0; j < ref_count; j++) {
            H5Oclose(obj);
        }
    }

    H5Object::close();
}


bool FileHDF5::isOpen() const {
    return isValid();
}


FileMode FileHDF5::fileMode() const {
    return mode;
}


Compression FileHDF5::compression() const {
     return compr;
}

shared_ptr<base::IFile> FileHDF5::file() const {
    return  const_pointer_cast<FileHDF5>(shared_from_this());
}


bool FileHDF5::checkHeader(FileMode mode) const {
    bool check = true;
    vector<int> vv;
    string str;
    if (root.hasAttr("format")) {
        if (!root.getAttr("format", str) || str != FILE_FORMAT) {
            check = false;
        }
    } else {
        check = false;
    }
    if (root.hasAttr("version")) {
        if (!root.getAttr("version", vv)) {
            check = false;
        } else {
            FormatVersion ver = FormatVersion(vv);

            if (mode == FileMode::ReadWrite) {
                check = my_version.canWrite(ver);
            } else {
                check = my_version.canRead(ver);
            }
        }
    } else {
        check = false;
    }
    return check;
}


void FileHDF5::createHeader() const {
    try {
        root.setAttr("format", FILE_FORMAT);
        root.setAttr("version", std::vector<int>{my_version.x(), my_version.y(), my_version.z()});
    } catch ( ... ) {
        throw H5Exception("Could not open/create file");
    }
}

void FileHDF5::openRoot() {
    root = H5Group(H5Gopen2(hid, "/", H5P_DEFAULT));
    root.check("Could not open root group");
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
    return location() == other.location();
}


bool FileHDF5::operator!=(const FileHDF5 &other) const {
    return !(*this == other);
}


FileHDF5::~FileHDF5() {
    close();
}

} // ns nix::hdf5
} // ns nix
