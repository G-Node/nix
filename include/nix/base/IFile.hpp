// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef I_FILE_H
#define I_FILE_H

#include <string>
#include <vector>

#include <nix/Platform.hpp>

namespace nix {

/**
 * @brief File open modes
 */
NIXAPI enum class FileMode {
    ReadOnly = 0,
    ReadWrite,
    Overwrite
};

/**
 * @brief NIX back-end implementations.
 */
NIXAPI enum class Implementation {
    Hdf5 = 0
};


class Block;
class Section;


namespace base {


/**
 * @brief Interface that represents a NIX file.
 *
 * See {@link nix::File} for a more detailed description.
 */
class NIXAPI IFile {

public:

    virtual size_t blockCount() const = 0;


    virtual bool hasBlock(const std::string &id) const = 0;


    virtual Block getBlock(const std::string &id) const = 0;


    virtual Block getBlock(size_t index) const = 0;


    virtual Block createBlock(const std::string &name, const std::string &type) = 0;


    virtual bool deleteBlock(const std::string &id) = 0;

    //--------------------------------------------------
    // Methods concerning sections
    //--------------------------------------------------

    virtual bool hasSection(const std::string &id) const = 0;


    virtual Section getSection(const std::string &id) const = 0;


    virtual Section getSection(size_t index) const = 0;


    virtual size_t sectionCount() const = 0;


    virtual Section createSection(const std::string &name, const std::string &type) = 0;


    virtual bool deleteSection(const std::string &id) = 0;

    //--------------------------------------------------
    // Methods for file attribute access.
    //--------------------------------------------------

    virtual std::vector<int> version() const = 0;


    virtual std::string format() const = 0;


    virtual std::string location() const = 0;


    virtual time_t createdAt() const = 0;


    virtual time_t updatedAt() const = 0;


    virtual void setUpdatedAt() = 0;


    virtual void forceUpdatedAt() = 0;


    virtual void setCreatedAt() = 0;


    virtual void forceCreatedAt(time_t time) = 0;


    virtual void close() = 0;


    virtual bool isOpen() const = 0;


    virtual ~IFile() {}

};


} // namespace base
} // namespace nix

#endif
