// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_FILE_HDF5_H
#define NIX_FILE_HDF5_H

#include <nix/base/IFile.hpp>
#include "h5x/H5Group.hpp"

#include <string>
#include <memory>

namespace nix {
namespace hdf5 {


/**
 * Class that represents a NIX file.
 */
class FileHDF5 : public H5Object, public base::IFile, public std::enable_shared_from_this<FileHDF5> {

private:

    /* groups representing different sections of the file */
    H5Group root, metadata, data;
    FileMode mode;

public:

    /**
     * Constructor that is used to open the file.
     *
     * @param name    The name of the file to open.
     * @param prefix  The prefix used for IDs.
     * @param mode    File open mode ReadOnly, ReadWrite or Overwrite.
     */
    FileHDF5(const std::string &name, const FileMode mode = FileMode::ReadWrite);

    //--------------------------------------------------
    // Methods concerning blocks
    //--------------------------------------------------


    ndsize_t blockCount() const;


    bool hasBlock(const std::string &name_or_id) const;


    std::shared_ptr<base::IBlock> getBlock(const std::string &name_or_id) const;


    std::shared_ptr<base::IBlock> getBlock(ndsize_t index) const;


    std::shared_ptr<base::IBlock> createBlock(const std::string &name, const std::string &type);


    bool deleteBlock(const std::string &name_or_id);

    //--------------------------------------------------
    // Methods concerning sections
    //--------------------------------------------------

    bool hasSection(const std::string &name_or_id) const;


    std::shared_ptr<base::ISection> getSection(const std::string &name_or_id) const;


    std::shared_ptr<base::ISection> getSection(ndsize_t index) const;


    ndsize_t sectionCount() const;


    std::shared_ptr<base::ISection> createSection(const std::string &name, const std::string &type);


    bool deleteSection(const std::string &name_or_id);

    //--------------------------------------------------
    // Methods for file attribute access.
    //--------------------------------------------------


    std::vector<int> version() const;


    std::string format() const;


    std::string location() const;


    time_t createdAt() const;


    time_t updatedAt() const;


    void setUpdatedAt();


    void forceUpdatedAt();


    void setCreatedAt();


    void forceCreatedAt(time_t t);


    void close();


    bool isOpen() const;


    FileMode fileMode() const;


    bool operator==(const FileHDF5 &other) const;


    bool operator!=(const FileHDF5 &other) const;


    virtual ~FileHDF5();

private:

    std::shared_ptr<base::IFile> file() const;

    // check for existence
    bool fileExists(const std::string &name) const;


    void createNew(const std::string &name, unsigned int h5mode, const H5Object &fcpl);


    void openExisting(const std::string &name, unsigned int h5mode, const H5Object &fcpl);


    void openRoot();


    bool checkHeader() const;


    void createHeader() const;
};


} // namespace hdf5
} // namespace nix

#endif // NIX_FILE_HDF5_H
