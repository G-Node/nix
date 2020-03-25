// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "FileFS.hpp"
#include "BlockFS.hpp"
#include "SectionFS.hpp"

namespace bfs = boost::filesystem;

namespace nix {
namespace file {


FileFS::FileFS(const std::string &name, FileMode mode, Compression compression)
    : DirectoryWithAttributes(name, mode, true) {
    this->mode = mode;
    this->compr = compression;
    if (mode == FileMode::Overwrite) {
        removeAll();
    }
    setCreatedAt();
    setUpdatedAt();
    create_subfolders(name);
    if (!checkHeader()) {
        throw std::runtime_error("Invalid file header: either file format or file version not correct");
    }
}

void FileFS::create_subfolders(const std::string &loc) {
    bfs::path data("data");
    bfs::path metadata("metadata");
    bfs::path p;
    p = bfs::canonical(loc);

    data_dir = Directory(p / data, mode);
    metadata_dir = Directory(p / metadata, mode);
}


ndsize_t FileFS::blockCount() const {
    return data_dir.subdirCount();
}

bool FileFS::hasBlock(const std::string &name_or_id) const  {
    boost::optional<bfs::path> path = data_dir.findByNameOrAttribute("entity_id", name_or_id);
    return (bool)path;
}


std::shared_ptr<base::IBlock> FileFS::getBlock(const std::string &name_or_id) const {
    std::shared_ptr<BlockFS> block;
    boost::optional<bfs::path> path = data_dir.findByNameOrAttribute("entity_id", name_or_id);
    if (path) {
        BlockFS b(file(), path->string());
        return std::make_shared<BlockFS>(b);
    }
    return block;
}


std::shared_ptr<base::IBlock> FileFS::getBlock(ndsize_t index) const {
    if (index >= blockCount()) {
        throw nix::OutOfBounds("Trying to access file.block with invalid index", index);
    }
    bfs::path p = data_dir.sub_dir_by_index(index);
    std::shared_ptr<BlockFS> b;
    b = std::make_shared<BlockFS>(file(), p.string());
    return b;
}


std::shared_ptr<base::IBlock> FileFS::createBlock(const std::string &name, const std::string &type) {
    if (name.empty()) {
        throw EmptyString("Trying to create Block with empty name!");
    }
    if (hasBlock(name)) {
        throw DuplicateName("Block with the given name already exists!");
    }
    std::string id = util::createId();
    BlockFS b(file(), data_dir.location(), id, type, name, this->compression());
    return std::make_shared<BlockFS>(b);
}


bool FileFS::deleteBlock(const std::string &name_or_id) {
    return data_dir.removeObjectByNameOrAttribute("entity_id", name_or_id);
}

//--------------------------------------------------
// Methods concerning sections
//--------------------------------------------------

bool FileFS::hasSection(const std::string &name_or_id) const {
    boost::optional<bfs::path> path = metadata_dir.findByNameOrAttribute("entity_id", name_or_id);
    return (bool)path;
}


std::shared_ptr<base::ISection> FileFS::getSection(const std::string &name_or_id) const {
    std::shared_ptr<base::ISection> sec;
    boost::optional<bfs::path> path = metadata_dir.findByNameOrAttribute("entity_id", name_or_id);
    if (path) {
        SectionFS s(file(), path->string());
        return std::make_shared<SectionFS>(s);
    }
    return sec;
}


std::shared_ptr<base::ISection> FileFS::getSection(ndsize_t index) const {
    if (index >= sectionCount()) {
        throw OutOfBounds("Trying to access file.section with invalid index.", index);
    }
    bfs::path p = metadata_dir.sub_dir_by_index(index);
    return std::make_shared<SectionFS>(file(), p.string());
}


ndsize_t FileFS::sectionCount() const {
    return metadata_dir.subdirCount();
}


std::shared_ptr<base::ISection> FileFS::createSection(const std::string &name, const std::string &type) {
    if (name.empty()) {
        throw EmptyString("Trying to create a Section with an empty name!");
    }
    if (hasSection(name)) {
        throw DuplicateName("Section with the specified name altready exists!");
    }
    std::string id = util::createId();
    SectionFS s(file(), metadata_dir.location(), id, type, name);
    return std::make_shared<SectionFS>(s);
}


bool FileFS::deleteSection(const std::string &name_or_id) {
    return metadata_dir.removeObjectByNameOrAttribute("entity_id", name_or_id);
}

//--------------------------------------------------
// Methods for file attribute access.
//--------------------------------------------------


std::vector<int> FileFS::version() const {
    std::vector<int> version;
    getAttr("version", version);
    return  version;
}


std::string FileFS::format() const {
    std::string format;
    getAttr("format", format);
    return format;
}


std::string FileFS::id() const {
    std::string id;
    getAttr("id", id);
    return  id;
}


void FileFS::forceId() {
    std::string id = util::createId();
    setAttr("id", id);
}


std::string FileFS::location() const {
    return boost::filesystem::canonical(Directory::location()).string();
}


time_t FileFS::createdAt() const {
    std::string temp_t;
    getAttr("created_at", temp_t);
    return util::strToTime(temp_t);
}


time_t FileFS::updatedAt() const {
    std::string temp_t;
    getAttr("updated_at", temp_t);
    return util::strToTime(temp_t);
}


void FileFS::setUpdatedAt(){
    if (!hasAttr("updated_at")) {
        time_t t = time(NULL);
        setAttr("updated_at", util::timeToStr(t));
    }
}


void FileFS::forceUpdatedAt() {
    time_t t = time(NULL);
    setAttr("updated_at", util::timeToStr(t));
}


void FileFS::setCreatedAt() {
    if (!hasAttr("created_at")) {
        time_t t = time(NULL);
        setAttr("created_at", util::timeToStr(t));
    }
}


void FileFS::forceCreatedAt(time_t t) {
    setAttr("created_at", util::timeToStr(t));
}


void FileFS::close() {} // FIXME not needed?

bool FileFS::isOpen() const { //FIXME not needed?
    return true;
}


bool FileFS::operator==(const FileFS &other) const {
    return location() == other.location();
}


bool FileFS::operator!=(const FileFS &other) const {
    return location() != other.location();
}


bool FileFS::checkHeader() {
    bool check = true;
    std::vector<int> version;
    std::string str;
    std::string id;
    // check format
    if (hasAttr("format")) {
        getAttr("format", str);
        if (str != FILE_FORMAT) {
            check = false;
        }
    } else {
        setAttr("format", FILE_FORMAT);
    }
    // check version
    if (hasAttr("version")) {
        getAttr("version", version);
        if (version != FILE_VERSION) {
            check = false;
        }
    } else {
        setAttr("version", FILE_VERSION);
    }
    // check id
    if (hasAttr("id")) {
        getAttr("id", id);
        if (!util::looksLikeUUID(id)) {
            check = false;
        }
    } else {
        setAttr("id", util::createId());
    }
    return check;
}

std::shared_ptr<base::IFile> FileFS::file() const {
    return std::const_pointer_cast<FileFS>(shared_from_this());
}

FileMode FileFS::fileMode() const {
    return mode;
}

Compression FileFS::compression() const {
    return compr;
}

FileFS::~FileFS() {}

} // namespace file
} // namespace nix
