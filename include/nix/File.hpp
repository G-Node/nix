// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_FILE_H
#define NIX_FILE_H

#include <nix/base/ImplContainer.hpp>
#include <nix/base/IFile.hpp>
#include <nix/Block.hpp>
#include <nix/Section.hpp>
#include <nix/Platform.hpp>
#include <nix/ObjectType.hpp>

#include <nix/valid/validate.hpp>

namespace nix {


class NIXAPI File : public base::ImplContainer<base::IFile> {

public:

    /**
     * @brief Constructor that creates an uninitialized File.
     *
     * Calling any method on an uninitialized file will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a file is initialized:
     *
     * ~~~
     * File e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    File() {}

    /**
     * @brief Copy constructor.
     *
     * Copying of all NIX front facing objects like File is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The file to copy.
     */
    File(const File &other)
        : ImplContainer(other.impl())
    {
    }

    /**
     * @brief Constructor that creates a new file from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    File(const std::shared_ptr<base::IFile> &p_impl)
        : ImplContainer(p_impl)
    {
    }

    /**
     * @brief Constructor with move semantics that creates a new entity from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    File(std::shared_ptr<base::IFile> &&ptr)
        : ImplContainer(std::move(ptr))
    {
    }

    /**
     * @brief Opens a file.
     *
     * @param name      The name/path of the file.
     * @param mode      The open mode.
     * @param impl      The back-end implementation the should be used to open the file.
     *                  (currently only hdf5)
     *
     * @return The opened file.
     */
    static File open(const std::string &name, FileMode mode=FileMode::ReadWrite,
                     const std::string &impl="hdf5");

    /**
     * @brief Persists all cached changes to the backend.
     *
     */
    bool flush();

    
    /**
     * @brief Get the number of blocks in in the file.
     *
     * @return The number of blocks.
     */
    ndsize_t blockCount() const {
        return backend()->blockCount();
    }

    /**
     * @brief Check if a block exists in the file.
     *
     * @param name_or_id    Name or ID of the block.
     *
     * @return True if the block exists, false otherwise.
     */
    bool hasBlock(const std::string &name_or_id) const {
        return backend()->hasBlock(name_or_id);
    }

    /**
     * @brief Check if a block exists in the file.
     *
     * @param block     The block to check.
     *
     * @return True if the block exists, false otherwise.
     */
    bool hasBlock(const Block &block) const;

    /**
     * @brief Read an existing block from the file.
     *
     * @param name_or_id    Name or ID of the block.
     *
     * @return The block with the given name or id.
     */
    Block getBlock(const std::string &name_or_id) const {
        return backend()->getBlock(name_or_id);
    }

    /**
     * @brief Read an existing with block from the file, addressed by index.
     *
     * @param index   The index of the block to read.
     *
     * @return The block at the given index.
     */
    Block getBlock(ndsize_t index) const {
        if (index >= blockCount()) {
            throw nix::OutOfBounds("Index is out of bounds when calling File::getBlock(index)!");
        }
        return backend()->getBlock(index);
    }

    /**
     * @brief Create an new block, that is immediately persisted in the file.
     *
     * @param name    The name of the block.
     * @param type    The type of the block.
     *
     * @return The created block.
     */
    Block createBlock(const std::string &name, const std::string &type);

    /**
     * @brief Deletes a block from the file.
     *
     * @param name_or_id    Name or id of the block to delete.
     *
     * @return True if the block has been removed, false otherwise.
     */
    bool deleteBlock(const std::string &name_or_id) {
        return backend()->deleteBlock(name_or_id);
    }

    /**
     * @brief Deletes a block from the file.
     *
     * @param block    The block to delete.
     *
     * @return True if the block has been removed, false otherwise.
     */
    bool deleteBlock(const Block &block);

    /**
     * @brief Get all blocks within this file.
     *
     * The parameter filter can be used to filter block by various
     * criteria.
     *
     * @param filter    A filter function.
     *
     * @return A vector of filtered Block entities.
     */
    std::vector<Block> blocks(const util::Filter<Block>::type &filter) const;

    /**
     * @brief Get all blocks within this file.
     *
     * The parameter filter can be used to filter block by various
     * criteria. By default a filter is used that accepts all blocks.
     *
     * @return A vector of filtered Block entities.
     */
    std::vector<Block> blocks() const
    {
        return blocks(util::AcceptAll<Block>());
    }

    //--------------------------------------------------
    // Methods concerning sections
    //--------------------------------------------------

    /**
     * @brief Check if a specific root section exists in the file.
     *
     * @param name_or_id      Name or ID of the section.
     *
     * @return True if the section exists, false otherwise.
     */
    bool hasSection(const std::string &name_or_id) const {
        return backend()->hasSection(name_or_id);
    }

    /**
     * @brief Check if a specific root section exists in the file.
     *
     * @param section The section to check.
     *
     * @return True if the section exists, false otherwise.
     */
    bool hasSection(const Section &section) const;

    /**
     * @brief Get a root section with the given name/id.
     *
     * @param name_or_id      Name or id of the section.
     *
     * @return The section with the specified name/id.
     */
    Section getSection(const std::string &name_or_id) const {
        return backend()->getSection(name_or_id);
    }

    /**
     * @brief Get root section with a given index/position.
     *
     * @param index      The index of the section.
     *
     * @return The section with the specified index.
     */
    Section getSection(ndsize_t index) const {
        if (index >= sectionCount()) {
            throw nix::OutOfBounds("Index is out of bounds when calling File::getSection(index)!");
        }
        return backend()->getSection(index);
    }

    /**
     * @brief Returns the number of root sections stored in the File.
     *
     * @return size_t   The number of sections.
     */
    ndsize_t sectionCount() const {
        return backend()->sectionCount();
    }


    /**
     * @brief Get all root sections within this file.
     *
     * The parameter filter can be used to filter sections by various
     * criteria.
     *
     * @param filter    A filter function.
     *
     * @return A vector of filtered Section entities.
     */
    std::vector<Section> sections(const util::Filter<Section>::type &filter) const;
    

    /**
     * @brief Get all root sections within this file.
     *
     * The parameter filter can be used to filter sections by various
     * criteria. By default a filter is used that accepts all sections.
     *
     * @return A vector of filtered Section entities.
     */
    std::vector<Section> sections() const
    {
        return sections(util::AcceptAll<Section>());
    }
    

    /**
     * @brief Get all sections in this file recursively.
     *
     * This method traverses the trees of all section in the file. The traversal
     * is accomplished via breadth first and can be limited in depth. On each node or
     * section a filter is applied. If the filter returns true the respective section
     * will be added to the result list.
     *
     * @param filter       A filter function.
     * @param max_depth    The maximum depth of traversal.
     *
     * @return A vector containing the matching sections.
     */
    std::vector<Section> findSections(const util::Filter<Section>::type &filter,
                                      size_t max_depth = std::numeric_limits<size_t>::max()) const;


    /**
     * @brief Get all sections in this file recursively.
     *
     * This method traverses the trees of all section in the file. The traversal
     * is accomplished via breadth first and can be limited in depth. On each node or
     * section a filter is applied. If the filter returns true the respective section
     * will be added to the result list.
     * By default a filter is used that accepts all sections.
     *
     * @param max_depth    The maximum depth of traversal.
     *
     * @return A vector containing the matching sections.
     */
    std::vector<Section> findSections(size_t max_depth = std::numeric_limits<size_t>::max()) const
    {
        return findSections(util::AcceptAll<Section>());
    }


    /**
     * @brief Creates a new Section with a given name and type. Both must not be empty.
     *
     * @param name    The name of the section.
     * @param type    The type of the section.
     *
     * @return The created Section.
     */
    Section createSection(const std::string &name, const std::string &type);

    /**
     * @brief Deletes the Section that is specified with the id.
     *
     * @param name_or_id        Name or id of the section to delete.
     *
     * @return True if the section was deleted, false otherwise.
     */
    bool deleteSection(const std::string &name_or_id) {
        return backend()->deleteSection(name_or_id);
    }

    /**
     * @brief Deletes the Section.
     *
     * @param section   The section to delete.
     *
     * @return True if the section was deleted, false otherwise.
     */
    bool deleteSection(const Section &section);

    //--------------------------------------------------
    // Methods for file attribute access.
    //--------------------------------------------------

    /**
     * @brief Read the NIX format version from the file.
     *
     * The version consist of three integers standing for the major,
     * minor and patch version of the nix format.
     *
     * @return The format version of the NIX file.
     */
    std::vector<int> version() const {
        return backend()->version();
    }

    /**
     * @brief Read the format hint from the file.
     *
     * @return The format hint.
     */
    std::string format() const {
        return backend()->format();
    }

    /**
     * @brief Return the location / uri.
     *
     * @return The uri string.
     */
    std::string location() const {
        return backend()->location();
    }

    /**
     * @brief Get the creation date of the file.
     *
     * @return The creation date of the file.
     */
    time_t createdAt() const {
        return backend()->createdAt();
    }

    /**
     * @brief Get the date of the last update.
     *
     * @return The date of the last update.
     */
    time_t updatedAt() const {
        return backend()->updatedAt();
    }

    /**
     * @brief Sets the time of the last update to the current time if
     * the field is not set.
     */
    void setUpdatedAt() {
        backend()->setUpdatedAt();
    }

    /**
     * @brief Sets the time of the last update to the current time.
     */
    void forceUpdatedAt() {
        backend()->forceUpdatedAt();
    }

    /**
     * @brief Sets the creation time to the current time if the field is not set.
     */
    void setCreatedAt() {
        backend()->setCreatedAt();
    }

    /**
     * @brief Sets the creation time to the provided value even if the
     * attribute is set.
     *
     * @param t        The creation time to set.
     */
    void forceCreatedAt(time_t t) {
        backend()->forceCreatedAt(t);
    }

    //------------------------------------------------------
    // Operators and other functions
    //------------------------------------------------------

    /**
     * @brief Close the file.
     */
    void close();

    /**
     * @brief Check if the file is currently open.
     *
     * @return True if the file is open, false otherwise.
     */
    bool isOpen() const {
        return !isNone() && backend()->isOpen();
    }

    /*
     * @brief Returns the mode in which the file has been opened.
     *
     * @return the FileMode
     */
    FileMode fileMode() {
        return backend()->fileMode();
    }
    /**
     * @brief Assignment operator for none.
     */
    File &operator=(const none_t &t) {
        ImplContainer::operator=(t);
        return *this;
    }

    //------------------------------------------------------
    // Validate
    //------------------------------------------------------

    valid::Result validate() const;

};

template<>
struct objectToType<nix::File> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::File;
    typedef nix::base::IFile backendType;
};


} // namespace nix

#endif
