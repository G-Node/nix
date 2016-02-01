// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_TAG_FS_HPP
#define NIX_TAG_FS_HPP

#include "BaseTagFS.hpp"
#include <nix/base/ITag.hpp>

namespace nix {
namespace file {


/**
 * Class that represents a NIX tag.
 */
class TagFS : public BaseTagFS, virtual public base::ITag {

private:

    static const NDSize MIN_CHUNK_SIZE;
    static const NDSize MAX_SIZE_1D;

public:

    /**
     * Standard constructor for existing Tag
     */
    TagFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc);

    /**
     * Standard constructor for new Tag
     */
    TagFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc, const std::string &id,
          const std::string &type, const std::string &name, const std::vector<double> &position);

    /**
     * Standard constructor for new Tag that preserves the creation time.
     */
    TagFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc, const std::string &id,
          const std::string &type, const std::string &name, const std::vector<double> &_position, const time_t time);


    std::vector<std::string> units() const;


    void units(const std::vector<std::string> &units);


    void units(const none_t t);


    std::vector<double> position() const;


    void position(const std::vector<double> &position);


    std::vector<double> extent() const;


    void extent(const std::vector<double> &extent);


    void extent(const none_t t);

    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------

    /**
     * Destructor.
     */
    virtual ~TagFS();

};


} // namespace file
} // namespace nix

#endif //NIX_TAG_FS_HPP
