// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BLOCK_HDF5_H
#define NIX_BLOCK_HDF5_H

#include <vector>
#include <string>

#include <nix.hpp>
#include <nix/hdf5/EntityWithMetadataHDF5.hpp>

namespace nix {
namespace hdf5 {

/**
 * Class that represents a pandora Block entity.
 */
class BlockHDF5 : virtual public base::IBlock, public EntityWithMetadataHDF5,
                  public std::enable_shared_from_this<BlockHDF5> {

private:

    Group source_group, data_array_group, simple_tag_group, data_tag_group;

public:

    /**
     * Copy constructor.
     *
     * @param block     The block to create the new copy from.
     */
    BlockHDF5(const BlockHDF5 &block);

    /**
     * Standard constructor for a Block.
     *
     * @param file      The file which contains this block.
     * @param group     The group that represents the block inside the file.
     * @param id        The id of this block.
     */
    BlockHDF5(File file, Group group, const std::string &id);

    /**
     * Standard constructor for a Block.
     *
     * @param file      The file which contains this block.
     * @param group     The group that represents the block inside the file.
     * @param id        The id of this block.
     */
    BlockHDF5(File file, Group group, const std::string &id, time_t time);


    //--------------------------------------------------
    // Methods concerning sources
    //--------------------------------------------------

    bool hasSource(const std::string &id) const;


    Source getSource(const std::string &id) const;


    Source getSource(size_t index) const;



    size_t sourceCount() const;


    Source createSource(const std::string &name, const std::string &type);


    bool deleteSource(const std::string &id);

    //--------------------------------------------------
    // Methods concerning data arrays
    //--------------------------------------------------

    bool hasDataArray(const std::string &id) const;


    DataArray getDataArray(const std::string &id) const;


    DataArray getDataArray(size_t index) const;


    size_t dataArrayCount() const;


    DataArray createDataArray(const std::string &name, const std::string &type);


    bool deleteDataArray(const std::string &id);

    //--------------------------------------------------
    // Methods concerning simple tags.
    //--------------------------------------------------


    bool hasSimpleTag(const std::string &id) const;


    SimpleTag getSimpleTag(const std::string &id) const;


    SimpleTag getSimpleTag(size_t index) const;


    size_t simpleTagCount() const;


    SimpleTag createSimpleTag(const std::string &name, const std::string &type);


    bool deleteSimpleTag(const std::string &id);

    //--------------------------------------------------
    // Methods concerning data tags.
    //--------------------------------------------------


    bool hasDataTag(const std::string &id) const;


    DataTag getDataTag(const std::string &id) const;


    DataTag getDataTag(size_t index) const;


    size_t dataTagCount() const;


    DataTag createDataTag(const std::string &name, const std::string &type);


    bool deleteDataTag(const std::string &id);

    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------

    void swap(BlockHDF5 &other);

    /**
     * Assignment operator
     */
    BlockHDF5& operator=(const BlockHDF5 &other);


    virtual ~BlockHDF5();

private:

    Block block() const;

};


} // namespace hdf5
} // namespace nix

#endif // NIX_BLOCK_HDF5_H
