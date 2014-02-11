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


NIXAPI enum class FileMode {
    ReadOnly = 0,
    ReadWrite,
    Overwrite
};


NIXAPI enum class Implementation {
    Hdf5 = 0
};


class Block;
class Section;


namespace base {


/**
 * Class that represents a pandora file.
 */
class IFile {

public:

    /**
     * Get the number of blocks in in the file.
     *
     * @return The number of blocks.
     */
    virtual size_t blockCount() const = 0;

    /**
     * Check if a block exists in the file.
     *
     * @param id    The ID of the block.
     *
     * @return True if the block exists, false otherwise.
     */
    virtual bool hasBlock(const std::string &id) const = 0;

    /**
     * Read an existing block from the file.
     *
     * @param id    The ID of the block.
     *
     * @return The block with the given id.
     */
    virtual Block getBlock(const std::string &id) const = 0;

    /**
     * Read an existing with block from the file, addressed by index.
     *
     * @param index   The index of the block to read.
     *
     * @return The block at the given index.
     */
    virtual Block getBlock(size_t index) const = 0;

    /**
     * Create an new block, that is immediately persisted in the file.
     *
     * @param name    The name of the block.
     * @param type    The type of the block.
     *
     * @return The created block.
     */
    virtual Block createBlock(const std::string &name, const std::string &type) = 0;

    /**
     * Deletes a block from the file.
     *
     * @param id    The id of the block to delete.
     *
     * @return True if the block has been removed, false otherwise.
     */
    virtual bool deleteBlock(const std::string &id) = 0;

    /**
     * Returns all blocks in this file.
     *
     * @return All blocks in this file as a vector.
     */
    virtual std::vector<Block> blocks() const = 0;

    //--------------------------------------------------
    // Methods concerning sections
    //--------------------------------------------------

    /**
     * Check if a specific root section exists in the file.
     *
     * @param id      The ID of the section.
     *
     * @return True if the section exists, false otherwise.
     */
    virtual bool hasSection(const std::string &id) const = 0;

    /**
     * Get root section with a given id.
     *
     * @param id      The id of the section.
     *
     * @return The section with the specified id.
     */
    virtual Section getSection(const std::string &id) const = 0;

    /**
     * Get root section with a given index/position.
     *
     * @param index      The index of the section.
     *
     * @return The section with the specified index.
     */
    virtual Section getSection(size_t index) const = 0;

    /**
     * Returns the number of Sections stored in the File.
     *
     * @return size_t   The number of sections.
     */
    virtual size_t sectionCount() const = 0;

    /**
     * Returns all root sections in this file.
     *
     * @returns All root sections as vector.
     */
    virtual std::vector<Section> sections() const = 0;

    /**
     * Creates a new Section with a given name and type. Both must not be empty.
     *
     * @param name    The given name of the section.
     * @param type    The type of the section.
     *
     * @return   the created Section.
     */
    virtual Section createSection(const std::string &name, const std::string &type) = 0;

    /**
     * Deletes the Section that is specified with the id.
     */
    virtual bool deleteSection(const std::string &id) = 0;

    //--------------------------------------------------
    // Methods for file attribute access.
    //--------------------------------------------------

    /**
     * Read the pandora version from the file.
     *
     * @return The version of the pandora file.
     */
    virtual std::string version() const = 0;

    /**
     * Read the format hint from the file.
     *
     * @return The format hint.
     */
    virtual std::string format() const = 0;

    /**
     * Get the creation date of the file.
     *
     * @return The creation date of the file.
     */
    virtual time_t createdAt() const = 0;

    /**
     * Get the date of the last update.
     *
     * @return The date of the last update.
     */
    virtual time_t updatedAt() const = 0;

    /**
     * Sets the time of the last update to the current time if
     * the field is not set.
     */
    virtual void setUpdatedAt() = 0;

    /**
     * Sets the time of the last update to the current time.
     */
    virtual void forceUpdatedAt() = 0;

    /**
     * Sets the creation time to the current time if the attribute
     * created_at is not set.
     */
    virtual void setCreatedAt() = 0;

    /**
     * Sets the creation time to the provided value even if the
     * attribute created_at is set.
     *
     * @param time The creation time to set.
     */
    virtual void forceCreatedAt(time_t t) = 0;

    /**
     * Close the file.
     */
    virtual void close() = 0;


    virtual bool isOpen() const = 0;


    virtual ~IFile() {}

};


} // namespace base
} // namespace nix

#endif
