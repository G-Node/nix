// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_SOURCE_HDF5_H
#define NIX_SOURCE_HDF5_H

#include <nix/base/ISource.hpp>
#include "EntityWithMetadataHDF5.hpp"

#include <vector>
#include <string>

namespace nix {
namespace hdf5 {

/**
 * An entity that implements ISource for the HDF5 back-end.
 */
class SourceHDF5 : virtual public base::ISource, public EntityWithMetadataHDF5  {

private:

    optGroup source_group;
    std::shared_ptr<base::IBlock> entity_block;

public:


    /**
     * Standard constructor for existing Source
     */
    SourceHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const H5Group &group);

    /**
     * Default constructor.
     */
    SourceHDF5(const std::shared_ptr<base::IFile> &file,  const std::shared_ptr<base::IBlock> &block,
               const H5Group &group, const std::string &id, const std::string &type, const std::string &name);

    /**
     * Default constructor that preserves the creation time.
     */
    SourceHDF5(const std::shared_ptr<base::IFile> &file,  const std::shared_ptr<base::IBlock> &block,
               const H5Group &group, const std::string &id, const std::string &type, const std::string &name, time_t time);

    //--------------------------------------------------
    // Methods concerning child sources
    //--------------------------------------------------


    bool hasSource(const std::string &name_or_id) const;

    
    std::shared_ptr<base::ISource> getSource(const std::string &name_or_id) const;


    std::shared_ptr<base::ISource> getSource(ndsize_t index) const;


    ndsize_t sourceCount() const;


    std::shared_ptr<base::ISource> createSource(const std::string &name, const std::string &type);


    bool deleteSource(const std::string &name_or_id);

    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------

    std::shared_ptr<base::IFile> parentFile() const;


    std::shared_ptr<base::IBlock> parentBlock() const;

    virtual ~SourceHDF5();
};


} // namestace hdf5
} // namespace nix

#endif // NIX_SOURCE_HDF5_H
