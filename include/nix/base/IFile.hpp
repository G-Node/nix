// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef I_FILE_H
#define I_FILE_H

#include <nix/base/ISection.hpp>
#include <nix/base/IBlock.hpp>
#include <nix/Platform.hpp>
#include <nix/ObjectType.hpp>

#include <string>
#include <vector>
#include <ctime>

namespace nix {

/**
 * @brief File open modes
 */
enum class FileMode {
    ReadOnly = 0,
    ReadWrite,
    Overwrite
};


#define FILE_VERSION std::vector<int>{1, 0, 0}
#define FILE_FORMAT  std::string("nix")

namespace base {


/**
 * @brief Interface that represents a NIX file.
 *
 * See {@link nix::File} for a more detailed description.
 */
class NIXAPI IFile {

public:

    virtual bool flush() = 0;


    virtual ndsize_t blockCount() const = 0;


    virtual bool hasBlock(const std::string &name_or_id) const = 0;


    virtual std::shared_ptr<IBlock> getBlock(const std::string &name_or_id) const = 0;


    virtual std::shared_ptr<IBlock> getBlock(ndsize_t index) const = 0;


    virtual std::shared_ptr<IBlock> createBlock(const std::string &name, const std::string &type) = 0;


    virtual bool deleteBlock(const std::string &name_or_id) = 0;

    //--------------------------------------------------
    // Methods concerning sections
    //--------------------------------------------------

    virtual bool hasSection(const std::string &name_or_id) const = 0;


    virtual std::shared_ptr<ISection> getSection(const std::string &name_or_id) const = 0;


    virtual std::shared_ptr<ISection> getSection(ndsize_t index) const = 0;


    virtual ndsize_t sectionCount() const = 0;


    virtual std::shared_ptr<ISection> createSection(const std::string &name, const std::string &type) = 0;


    virtual bool deleteSection(const std::string &name_or_id) = 0;

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


    virtual FileMode fileMode() const = 0;


    virtual ~IFile() {}

};


} // namespace base

template<>
struct objectToType<nix::base::IFile> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::File;
};

} // namespace nix

#endif
