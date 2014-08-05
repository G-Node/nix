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

#include <nix/base/IBlock.hpp>
#include <nix/hdf5/EntityWithMetadataHDF5.hpp>

namespace nix {
namespace hdf5 {

/**
 * Class that represents a NIX Block entity.
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
     * @param type      The type of this block.
     * @param name      The name of this block.
     */
    BlockHDF5(std::shared_ptr<base::IFile> file, Group group, const std::string &id, const std::string &type, const std::string &name);

    /**
     * Standard constructor for a Block.
     *
     * @param file      The file which contains this block.
     * @param group     The group that represents the block inside the file.
     * @param id        The id of this block.
     * @param type      The type of this block.
     * @param name      The name of this block.
     */
    BlockHDF5(std::shared_ptr<base::IFile> file, Group group, const std::string &id, const std::string &type, const std::string &name, time_t time);

    //--------------------------------------------------
    // Methods concerning sources
    //--------------------------------------------------

    bool hasSource(const std::string &id) const;


    std::shared_ptr<base::ISource> getSource(const std::string &id) const;


    std::shared_ptr<base::ISource> getSource(size_t index) const;


    size_t sourceCount() const;


    std::shared_ptr<base::ISource> createSource(const std::string &name, const std::string &type);


    bool deleteSource(const std::string &id);

    //--------------------------------------------------
    // Methods concerning data arrays
    //--------------------------------------------------

    bool hasDataArray(const std::string &id) const;


    std::shared_ptr<base::IDataArray> getDataArray(const std::string &id) const;


    std::shared_ptr<base::IDataArray> getDataArray(size_t index) const;


    size_t dataArrayCount() const;


    std::shared_ptr<base::IDataArray> createDataArray(const std::string &name, const std::string &type,
                                                      nix::DataType data_type, const NDSize &shape) override;


    bool deleteDataArray(const std::string &id);

    //--------------------------------------------------
    // Methods concerning simple tags.
    //--------------------------------------------------

    bool hasSimpleTag(const std::string &id) const;


    std::shared_ptr<base::ISimpleTag> getSimpleTag(const std::string &id) const;


    std::shared_ptr<base::ISimpleTag> getSimpleTag(size_t index) const;


    size_t simpleTagCount() const;


    std::shared_ptr<base::ISimpleTag> createSimpleTag(const std::string &name, const std::string &type,
                                                      const std::vector<DataArray> &refs);


    bool deleteSimpleTag(const std::string &id);

    //--------------------------------------------------
    // Methods concerning data tags.
    //--------------------------------------------------

    bool hasDataTag(const std::string &id) const;


    std::shared_ptr<base::IDataTag> getDataTag(const std::string &id) const;


    std::shared_ptr<base::IDataTag> getDataTag(size_t index) const;


    size_t dataTagCount() const;


    std::shared_ptr<base::IDataTag> createDataTag(const std::string &name, const std::string &type,
                                                  const DataArray &positions);


    bool deleteDataTag(const std::string &id);

    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------

    // TODO do we really need swap and operator=?
    void swap(BlockHDF5 &other);


    BlockHDF5& operator=(const BlockHDF5 &other);


    virtual ~BlockHDF5();


    std::shared_ptr<IBlock> block() const;

};


} // namespace hdf5
} // namespace nix

#endif // NIX_BLOCK_HDF5_H
