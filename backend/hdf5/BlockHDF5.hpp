// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BLOCK_HDF5_H
#define NIX_BLOCK_HDF5_H

#include <nix/base/IBlock.hpp>
#include "EntityWithMetadataHDF5.hpp"

#include <vector>
#include <string>
#include <boost/optional.hpp>

namespace nix {
namespace hdf5 {

/**
 * Class that represents a NIX Block entity.
 */
class BlockHDF5 : virtual public base::IBlock, public EntityWithMetadataHDF5,
                  public std::enable_shared_from_this<BlockHDF5> {

private:

    optGroup data_array_group, tag_group, multi_tag_group, source_group, groups_group;

public:

    /**
     * Standard constructor for an existing Block.
     *
     * @param file      The file which contains this block.
     * @param group     The group that represents the block inside the file.
     */
    BlockHDF5(const std::shared_ptr<base::IFile> &file, const H5Group &group);

    /**
     * Standard constructor for a new Block.
     *
     * @param file      The file which contains this block.
     * @param group     The group that represents the block inside the file.
     * @param id        The id of this block.
     * @param type      The type of this block.
     * @param name      The name of this block.
     */
    BlockHDF5(const std::shared_ptr<base::IFile> &file, const H5Group &group, const std::string &id, const std::string &type, const std::string &name);

    /**
     * Standard constructor for a new Block.
     *
     * @param file      The file which contains this block.
     * @param group     The group that represents the block inside the file.
     * @param id        The id of this block.
     * @param type      The type of this block.
     * @param name      The name of this block.
     * @param time      The creation time of this block.
     */
    BlockHDF5(const std::shared_ptr<base::IFile> &file, const H5Group &group, const std::string &id, const std::string &type, const std::string &name, time_t time);


private:
    // Helper methods for generic entity related methods below
    boost::optional<H5Group> groupForObjectType(ObjectType ot, bool create = false) const;

    boost::optional<H5Group> findEntityGroup(const nix::Identity &ident) const;

public:
    //--------------------------------------------------
    // Generic entity methods
    //--------------------------------------------------
    std::string resolveEntityId(const nix::Identity &ident) const;

    bool hasEntity(const nix::Identity &ident) const;

    std::shared_ptr<base::IEntity> getEntity(const nix::Identity &ident) const;

    std::shared_ptr<base::IEntity> getEntity(ObjectType type, ndsize_t index) const;

    ndsize_t entityCount(ObjectType type) const;

    bool removeEntity(const nix::Identity &ident);


    //--------------------------------------------------
    // Methods concerning sources
    //--------------------------------------------------

    bool hasSource(const std::string &name_or_id) const;


    std::shared_ptr<base::ISource> getSource(const std::string &name_or_id) const;


    std::shared_ptr<base::ISource> getSource(ndsize_t index) const;


    ndsize_t sourceCount() const;


    std::shared_ptr<base::ISource> createSource(const std::string &name, const std::string &type);


    bool deleteSource(const std::string &name_or_id);

    //--------------------------------------------------
    // Methods concerning data arrays
    //--------------------------------------------------

    std::shared_ptr<base::IDataArray> createDataArray(const std::string &name, const std::string &type,
                                                      nix::DataType data_type, const NDSize &shape);


    //--------------------------------------------------
    // Methods concerning tags.
    //--------------------------------------------------

    std::shared_ptr<base::ITag> createTag(const std::string &name, const std::string &type,
                                                      const std::vector<double> &position);

    //--------------------------------------------------
    // Methods concerning multi tags.
    //--------------------------------------------------

    std::shared_ptr<base::IMultiTag> createMultiTag(const std::string &name, const std::string &type,
                                                  const DataArray &positions);

    //--------------------------------------------------
    // Methods concerning groups.
    //--------------------------------------------------

    std::shared_ptr<base::IGroup> createGroup(const std::string &name, const std::string &type);


    bool deleteGroup(const std::string &name_or_id);

    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------


    virtual ~BlockHDF5();


    std::shared_ptr<base::IBlock> block() const;

};


} // namespace hdf5
} // namespace nix

#endif // NIX_BLOCK_HDF5_H
