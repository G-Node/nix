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
        : ImplContainer(other.impl_ptr)
    {
    }

    File(const std::shared_ptr<base::IFile> &p_impl)
        : ImplContainer(p_impl)
    {
    }

    static File open(const std::string name, FileMode mode=FileMode::ReadWrite,
                     Implementation impl=Implementation::Hdf5);

    size_t blockCount() const {
        return impl_ptr->blockCount();
    }


    bool hasBlock(const std::string &id) const {
        return impl_ptr->hasBlock(id);
    }


    Block getBlock(const std::string &id) const {
        return impl_ptr->getBlock(id);
    }


    Block getBlock(size_t index) const {
        return impl_ptr->getBlock(index);
    }


    Block createBlock(const std::string &name, const std::string &type) {
        return impl_ptr->createBlock(name, type);
    }


    bool removeBlock(const std::string &id) {
        return impl_ptr->removeBlock(id);
    }


    std::vector<Block> blocks() const {
        return impl_ptr->blocks();
    }

    //--------------------------------------------------
    // Methods concerning sections
    //--------------------------------------------------


    bool hasSection(const std::string &id) const {
        return impl_ptr->hasSection(id);
    }


    Section getSection(const std::string &id) const {
        return impl_ptr->getSection(id);
    }


    Section getSection(size_t index) const {
        return impl_ptr->getSection(index);
    }


    size_t sectionCount() const {
        return impl_ptr->sectionCount();
    }


    std::vector<Section> sections() const {
        return impl_ptr->sections();
    }


    std::vector<Section> findSections(std::function<bool(const Section&)> filter = util::acceptAllFilter<Section>,
                                      size_t max_depth = std::numeric_limits<size_t>::max()) const;


    Section createSection(const std::string &name, const std::string &type) {
        return impl_ptr->createSection(name, type);
    }


    bool removeSection(const std::string &id) {
        return impl_ptr->removeSection(id);
    }

    //--------------------------------------------------
    // Methods for file attribute access.
    //--------------------------------------------------


    std::string version() const {
        return impl_ptr->version();
    }


    std::string format() const {
        return impl_ptr->format();
    }


    time_t createdAt() const {
        return impl_ptr->createdAt();
    }


    time_t updatedAt() const {
        return impl_ptr->updatedAt();
    }


    void setUpdatedAt() {
        impl_ptr->setUpdatedAt();
    }


    void forceUpdatedAt() {
        impl_ptr->forceUpdatedAt();
    }


    void setCreatedAt() {
        impl_ptr->setCreatedAt();
    }


    void forceCreatedAt(time_t t) {
        impl_ptr->forceCreatedAt(t);
    }

    //------------------------------------------------------
    // Operators and other functions
    //------------------------------------------------------


    void close() {
        if (impl_ptr != nullptr)
            impl_ptr->close();
    }


    bool isOpen() const {
        if (impl_ptr == nullptr)
            return false;
        else
            return impl_ptr->isOpen();
    }


    virtual File &operator=(std::nullptr_t nullp) {
        impl_ptr = nullp;
        return *this;
    }

};


} // namespace nix

#endif
