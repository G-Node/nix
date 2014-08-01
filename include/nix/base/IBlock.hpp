// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_BLOCK_H
#define NIX_I_BLOCK_H

#include <nix/base/IEntityWithMetadata.hpp>
#include <nix/NDSize.hpp>

#include <string>
#include <vector>
#include <functional>

namespace nix {

class Source;
class DataArray;
class SimpleTag;
class DataTag;


namespace base {

/**
 * @brief Interface for implementations of the Block entity.
 *
 * See {@link nix::Block} for a more detailed description.
 */
class NIXAPI IBlock : virtual public base::IEntityWithMetadata {

public:

    /**
     * @brief Checks if this block has a specific root source.
     *
     * @param id        The id of the source.
     *
     * @return True if a source with the given id exists at the root, false
     *         otherwise.
     */
    virtual bool hasSource(const std::string &id) const = 0;

    /**
     * @brief Retrieves a specific root source by its id.
     *
     * @param id        The id of the source.
     *
     * @return The source with the given id. If it doesn't exist an exception
     *         will be thrown.
     */
    virtual Source getSource(const std::string &id) const = 0;

    /**
     * @brief Retrieves a specific root source by index.
     *
     * @param index     The index of the source.
     *
     * @return The source at the specified index.
     */
    virtual Source getSource(size_t index) const = 0;


    /**
     * @brief Returns the number of root sources in this block.
     *
     * @return The number of root sources.
     */
    virtual size_t sourceCount() const = 0;


    /**
     * @brief Create a new root source.
     *
     * @param name      The name of the source to create.
     * @param type      The type of the source.
     *
     * @return The created source object.
     */
    virtual Source createSource(const std::string &name, const std::string &type) = 0;

    /**
     * @brief Deletes a root source.
     *
     * This will also delete all child sources of this root source from the file.
     * The deletion of a source can't be undone.
     *
     * @param id        The id of the source to delete.
     *
     * @return True if the source was deleted, false otherwise.
     */
    virtual bool deleteSource(const std::string &id) = 0;

    //--------------------------------------------------
    // Methods concerning data arrays
    //--------------------------------------------------

    /**
     * @brief Checks if a specific data array exists in this block.
     *
     * @param id        The id of a data array.
     *
     * @return True if the data array exists, false otherwise.
     */
    virtual bool hasDataArray(const std::string &id) const = 0;

    /**
     * @brief Retrieves a specific data array from the block by id.
     *
     * @param id        The id of an existing data array.
     *
     * @return The data array with the specified id. If this
     *         doesn't exist, an exception will be thrown.
     */
    virtual DataArray getDataArray(const std::string &id) const = 0;

    /**
     * @brief Retrieves a data array by index.
     *
     * @param index     The index of the data array.
     *
     * @return The data array at the specified index.
     */
    virtual DataArray getDataArray(size_t index) const = 0;

    /**
     * @brief Returns the number of all data arrays of the block.
     *
     * @return The number of data arrays of the block.
     */
    virtual size_t dataArrayCount() const = 0;


    virtual DataArray createDataArray(const std::string &name,
                                      const std::string &type,
                                      nix::DataType      data_type,
                                      const NDSize      &shape) = 0;

    /**
     * @brief Deletes a data array from this block.
     *
     * This deletes a data array and all its dimensions from the block and the file.
     * The deletion can't be undone.
     *
     * @param id        The id of the data array to delete.
     *
     * @return True if the data array was deleted, false otherwise.
     */
    virtual bool deleteDataArray(const std::string &id) = 0;

    //--------------------------------------------------
    // Methods concerning simple tags.
    //--------------------------------------------------

    /**
     * @brief Checks if a specific simple tag exists in the block.
     *
     * @param id        The id of a simple tag.
     *
     * @return True if the simple tag exists, false otherwise.
     */
    virtual bool hasSimpleTag(const std::string &id) const = 0;

    /**
     * @brief Retrieves a specific simple tag from the block by its id.
     *
     * @param id        The id of the simple tag.
     *
     * @return The tag with the specified id. If this tag doesn't exist
     *         an exception will be thrown.
     */
    virtual SimpleTag getSimpleTag(const std::string &id) const = 0;

    /**
     * @brief Retrieves a specific simple tag by index.
     *
     * @param index     The index of the tag.
     *
     * @return The simple tag at the specified index.
     */
    virtual SimpleTag getSimpleTag(size_t index) const = 0;

    /**
     * @brief Returns the number of simple tags within this block.
     *
     * @return The number of simple tags.
     */
    virtual size_t simpleTagCount() const = 0;

    /**
     * @brief Create a new simple tag associated with this block.
     *
     * @param name      The name of the simple tag to create.
     * @param type      The type of the tag.
     * @param refs      A Vector with referenced data array entities.
     *
     * @return The newly created tag.
     */
    virtual SimpleTag createSimpleTag(const std::string &name, const std::string &type, 
                                      const std::vector<double> &position) = 0;

    /**
     * @brief Deletes a simple tag from the block.
     *
     * Deletes a simple tag with all its features from the block and the file.
     * The deletion can't be undone.
     *
     * @param id        The id of the tag to remove.
     *
     * @return True if the tag was removed, false otherwise.
     */
    virtual bool deleteSimpleTag(const std::string &id) = 0;

    //--------------------------------------------------
    // Methods concerning data tags.
    //--------------------------------------------------

    /**
     * @brief Checks if a specific data tag exists in the block.
     *
     * @param id        The id of a data tag.
     *
     * @return True if the data tag exists, false otherwise.
     */
    virtual bool hasDataTag(const std::string &id) const = 0;

    /**
     * @brief Retrieves a specific data tag from the block by its id.
     *
     * @param id        The id of the data tag.
     *
     * @return The tag with the specified id. If this tag doesn't exist
     *         an exception will be thrown.
     */
    virtual DataTag getDataTag(const std::string &id) const = 0;

    /**
     * @brief Retrieves a specific data tag by index.
     *
     * @param index     The index of the tag.
     *
     * @return The data tag at the specified index.
     */
    virtual DataTag getDataTag(size_t index) const = 0;

    /**
     * @brief Returns the number of data tags associated with this block.
     *
     * @return The number of data tags.
     */
    virtual size_t dataTagCount() const = 0;

    /**
     * @brief Create a new data tag associated with this block.
     *
     * @param name      The name of the data tag to create.
     * @param type      The type of the tag.
     * @param positions The positions of the tag.
     *
     * @return The newly created tag.
     */
    virtual DataTag createDataTag(const std::string &name, const std::string &type, 
                                  const DataArray positions) = 0;

    /**
     * @brief Deletes a data tag from the block.
     *
     * Deletes a data tag and all its features from the block and the file.
     * The deletion can't be undone.
     *
     * @param id        The id of the tag to remove.
     *
     * @return True if the tag was removed, false otherwise.
     */
    virtual bool deleteDataTag(const std::string &id) = 0;


    /**
     * @brief Destructor
     */
    virtual ~IBlock() {}

};

} // namespace base
} // namespace nix


#endif // NIX_I_BLOCK_H
