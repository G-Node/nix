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

namespace nix {


class NIXAPI File : public base::IFile, public base::ImplContainer<base::IFile> {

public:

    /**
     * @brief Constructor that creates a null file.
     */
    File() {}

    /**
     * @brief Copy constructor.
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
    static File open(const std::string name, FileMode mode=FileMode::ReadWrite,
                     Implementation impl=Implementation::Hdf5);

    size_t blockCount() const {
        return backend()->blockCount();
    }


    bool hasBlock(const std::string &id) const {
        return backend()->hasBlock(id);
    }

    /**
     * @brief Check if a block exists in the file.
     *
     * @param block     The block to check.
     *
     * @return True if the block exists, false otherwise.
     */
    bool hasBlock(const Block &block) const {
        if (block == none) {
            throw std::runtime_error("File::hasBlock: Empty Block entity given!");
        }
        return backend()->hasBlock(block.id());
    }

    Block getBlock(const std::string &id) const {
        return backend()->getBlock(id);
    }


    Block getBlock(size_t index) const {
        return backend()->getBlock(index);
    }


    Block createBlock(const std::string &name, const std::string &type) {
        return backend()->createBlock(name, type);
    }


    bool deleteBlock(const std::string &id) {
        return backend()->deleteBlock(id);
    }

    /**
     * @brief Deletes a block from the file.
     *
     * @param block    The block to delete.
     *
     * @return True if the block has been removed, false otherwise.
     */
    bool deleteBlock(const Block &block) {
        if (block == none) {
            throw std::runtime_error("File::deleteBlock: Empty Block entity given!");
        }
        return backend()->deleteBlock(block.id());
    }

    /**
     * @brief Get all blocks within this file.
     *
     * The parameter filter can be used to filter block by various
     * criteria. By default a filter is used that accepts every block.
     *
     * @param filter    A filter function.
     *
     * @return A vector of filtered Block entities.
     */
    std::vector<Block> blocks(util::Filter<Block>::type filter = util::AcceptAll<Block>()) const
    {
        auto f = [this] (size_t i) { return getBlock(i); };
        return getEntities<Block>(f,
                                  blockCount(),
                                  filter);
    }

    //--------------------------------------------------
    // Methods concerning sections
    //--------------------------------------------------

    bool hasSection(const std::string &id) const {
        return backend()->hasSection(id);
    }


    bool hasSection(const Section &section) const {
        if(section == none) {
            throw std::runtime_error("File::hasSection: Empty Section entity given!");
        }
        return backend()->hasSection(section.id());
    }


    Section getSection(const std::string &id) const {
        return backend()->getSection(id);
    }


    Section getSection(size_t index) const {
        return backend()->getSection(index);
    }


    size_t sectionCount() const {
        return backend()->sectionCount();
    }


    /**
     * @brief Get all root sections within this file.
     *
     * The parameter filter can be used to filter sections by various
     * criteria. By default a filter is used that accepts every section.
     *
     * @param filter    A filter function.
     *
     * @return A vector of filtered Section entities.
     */
    std::vector<Section> sections(util::Filter<Section>::type filter = util::AcceptAll<Section>()) const
    {
        auto f = [this] (size_t i) { return getSection(i); };
        return getEntities<Section>(f,
                                    sectionCount(),
                                    filter);
    }

    /**
     * @brief Get all sections in this file recursively.
     *
     * This method traverses the trees of all section in the file. The traversal
     * is accomplished via breadth first and can be limited by depth. On each node or
     * section a filter is applied. If the filter returns true the respective section
     * will be added to the result list.
     * By default a filter is used that accepts every section.
     *
     * @param filter       A filter function.
     * @param max_depth    The maximum depth of traversal.
     *
     * @return A vector containing the filtered sections.
     */
    std::vector<Section> findSections(util::Filter<Section>::type filter = util::AcceptAll<Section>(),
                                      size_t max_depth = std::numeric_limits<size_t>::max()) const;


    Section createSection(const std::string &name, const std::string &type) {
        return backend()->createSection(name, type);
    }


    bool deleteSection(const std::string &id) {
        return backend()->deleteSection(id);
    }

    /**
     * @brief Deletes the Section.
     *
     * @param section   The section to delete.
     *
     * @return True if the section was deleted, false otherwise.
     */
    bool deleteSection(const Section &section) {
        if(section == none) {
            throw std::runtime_error("File::hasSection: Empty Section entity given!");
        }
        return deleteSection(section.id());
    }

    //--------------------------------------------------
    // Methods for file attribute access.
    //--------------------------------------------------

    std::string version() const {
        return backend()->version();
    }


    std::string format() const {
        return backend()->format();
    }


    time_t createdAt() const {
        return backend()->createdAt();
    }


    time_t updatedAt() const {
        return backend()->updatedAt();
    }


    void setUpdatedAt() {
        backend()->setUpdatedAt();
    }


    void forceUpdatedAt() {
        backend()->forceUpdatedAt();
    }


    void setCreatedAt() {
        backend()->setCreatedAt();
    }


    void forceCreatedAt(time_t t) {
        backend()->forceCreatedAt(t);
    }

    //------------------------------------------------------
    // Operators and other functions
    //------------------------------------------------------


    void close() {
        if (!isNone()) {
            backend()->close();
            nullify();
        }
    }


    bool isOpen() const {
        return !isNone() && backend()->isOpen();
    }

    /**
     * @brief Assignment operator for none.
     */
    virtual File &operator=(none_t) {
        nullify();
        return *this;
    }

};


} // namespace nix

#endif
