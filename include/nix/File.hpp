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

    File() {}

    File(const File &other)
        : ImplContainer(other.impl())
    {
    }

    File(const std::shared_ptr<base::IFile> &p_impl)
        : ImplContainer(p_impl)
    {
    }

    static File open(const std::string name, FileMode mode=FileMode::ReadWrite,
                     Implementation impl=Implementation::Hdf5);

    size_t blockCount() const {
        return backend()->blockCount();
    }


    bool hasBlock(const std::string &id) const {
        return backend()->hasBlock(id);
    }

    bool hasBlock(const Block &block) const {
        if (block == none){
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


    bool deleteBlock(const Block &block) {
        if (block == none){
            throw std::runtime_error("File::deleteBlock: Empty Block entity given!");
        }
        return backend()->deleteBlock(block.id());
    }

    /**
     * Get blocks associated with this file.
     *
     * The parameter "filter" is defaulted to giving back all blocks. To
     * use your own filter pass a lambda that accepts a "Block"
     * as parameter and returns a bool telling whether to get it or not.
     *
     * @param object filter function of type {@link nix::util::Filter::type}
     * @return object blocks as a vector     
     */
    std::vector<Block> blocks(util::AcceptAll<Block>::type filter
                                  = util::AcceptAll<Block>()) const
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
        if(section == none){
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
     * Get sections associated with this file.
     *
     * The parameter "filter" is defaulted to giving back all sections. 
     * To use your own filter pass a lambda that accepts a "Section"
     * as parameter and returns a bool telling whether to get it or not.
     *
     * @param object filter function of type {@link nix::util::Filter::type}
     * @return object sections as a vector     
     */
    std::vector<Section> sections(util::AcceptAll<Section>::type filter
                                  = util::AcceptAll<Section>()) const
    {
        auto f = [this] (size_t i) { return getSection(i); };
        return getEntities<Section>(f,
                                    sectionCount(),
                                    filter);
    }


    std::vector<Section> findSections(std::function<bool(const Section&)> filter = util::AcceptAll<Section>(),
                                      size_t max_depth = std::numeric_limits<size_t>::max()) const;


    Section createSection(const std::string &name, const std::string &type) {
        return backend()->createSection(name, type);
    }


    bool deleteSection(const std::string &id) {
        return backend()->deleteSection(id);
    }


    bool deleteSection(const Section &section){
        if(section == none){
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
        }
    }


    bool isOpen() const {
        return !isNone() && backend()->isOpen();
    }


    virtual File &operator=(none_t) {
        nullify();
        return *this;
    }

};


} // namespace nix

#endif
