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

#include <string>
#include <vector>
#include <functional>

namespace nix {

class Source;
class DataArray;
class SimpleTag;
class DataTag;

class IBlock : virtual public base::IEntityWithMetadata {

public:

    /**
     * Checks if this block has a specific root source.
     *
      * @param id        The id of the source.
     *
     * @return True if a source with the given id exists at the root, false
     *         otherwise.
     */
    virtual bool hasSource(const std::string &id) const = 0;

    /**
     * Retrieves a specific root source.
     *
     * @param id        The id of the source.
     *
     * @return The source with the given id. If it doesn't exist an exception
     *         will be thrown.
     */
    virtual Source getSource(const std::string &id) const = 0;

    /**
     * Retrieves a specific root source by index.
     *
     * @param index     The index of the source.
     *
     * @return The source at the specified index.
     */
    virtual Source getSource(size_t index) const = 0;


    /**
     * Returns the number of root sources in this block.
     *
     * @return The number of root sources.
     */
    virtual size_t sourceCount() const = 0;

    /**
     * Returns all root sources in this block as a vector.
     *
     * @return All root sources.
     */
    virtual std::vector<Source> sources() const = 0;


    virtual std::vector<Source> findSources(std::function<bool(const Source &)>) const = 0;


    /**
     * Create a new root source.
     *
     * @param name      The name of the source to create.
     * @param type      The type of the source.
     *
     * @return The created source object.
     */
    virtual Source createSource(const std::string &name, const std::string &type) = 0;

    /**
     * Remove a root source and all its child sources from
     * the block.
     *
     * @param id        The id of the source to remove.
     *
     * @return True if the source was removed, false otherwise.
     */
    virtual bool removeSource(const std::string &id) = 0;

    //--------------------------------------------------
    // Methods concerning data arrays
    //--------------------------------------------------

    /**
     * Checks if a specific data array exists in this block.
     *
     * @param id        The id of a data array.
     *
     * @return True if the data array exists, false otherwise.
     */
    virtual bool hasDataArray(const std::string &id) const = 0;

    /**
     * Retrieves a specific data array from the block.
     *
     * @param id        The id of an existing data array.
     *
     * @return The data array with the specified id. If this
     *         doesn't exist, an exception will be thrown.
     */
    virtual DataArray getDataArray(const std::string &id) const = 0;

    /**
     * Retrieves a data array by index.
     *
     * @param index     The index of the data array.
     *
     * @return The data array at the specified index.
     */
    virtual DataArray getDataArray(size_t index) const = 0;

    /**
     * Returns all data arrays of this block as a vector.
     *
     * @return All data arrays.
     */
    virtual std::vector<DataArray> dataArrays() const = 0;

    /**
     * Returns the number of all data arrays of the block.
     *
     * @return The number of data arrays of the block.
     */
    virtual size_t dataArrayCount() const = 0;

    /**
     * Create a new data array associated with this block.
     *
     * @param name      The name of the data array to create.
     *
     * @return The newly created data array.
     */
    virtual DataArray createDataArray(const std::string &name, const std::string &type) = 0;

    /**
     * Remove/delete a data array from this block.
     *
     * @param id        The id of the data array to remove.
     *
     * @return True if the data array was removed, false otherwise.
     */
    virtual bool removeDataArray(const std::string &id) = 0;

    //--------------------------------------------------
    // Methods concerning simple tags.
    //--------------------------------------------------

    /**
     * Checks if a specific simple tag exists in the block.
     *
     * @param id        The id of a simple tag.
     *
     * @return True if the simple tag exists, false otherwise.
     */
    virtual bool hasSimpleTag(const std::string &id) const = 0;

    /**
     * Retrieves a specific simple tag from the block.
     *
     * @param id        The id of the simple tag.
     *
     * @return The tag with the specified id. If this tag doesn't exist
     *         an exception will be thrown.
     */
    virtual SimpleTag getSimpleTag(const std::string &id) const = 0;

    /**
     * Retrieves a specific simple tag by index.
     *
     * @param index     The index of the tag.
     *
     * @return The simple tag at the specified index.
     */
    virtual SimpleTag getSimpleTag(size_t index) const = 0;

    /**
     * Get all simple tags associated with this block.
     *
     * @return All simple tags as a vector.
     */
    virtual std::vector<SimpleTag> simpleTags() const = 0;

    /**
     * Returns the number of simple tag associated with
     * this block.
     *
     * @return The number of simple tags.
     */
    virtual size_t simpleTagCount() const = 0;

    /**
     * Create a new simple tag associated with this block.
     *
     * @param name      The name of the simple tag to create.
     * @param type      The type of the tag.
     *
     * @return The newly created tag.
     */
    virtual SimpleTag createSimpleTag(const std::string &name, const std::string &type) = 0;

    /**
     * Remove a simple tag from the block.
     *
     * @param id        The id of the tag to remove.
     *
     * @return True if the tag was removed, false otherwise.
     */
    virtual bool removeSimpleTag(const std::string &id) = 0;

    //--------------------------------------------------
    // Methods concerning data tags.
    //--------------------------------------------------

    /**
     * Checks if a specific data tag exists in the block.
     *
     * @param id        The id of a data tag.
     *
     * @return True if the data tag exists, false otherwise.
     */
    virtual bool hasDataTag(const std::string &id) const = 0;

    /**
     * Retrieves a specific data tag from the block.
     *
     * @param id        The id of the data tag.
     *
     * @return The tag with the specified id. If this tag doesn't exist
     *         an exception will be thrown.
     */
    virtual DataTag getDataTag(const std::string &id) const = 0;

    /**
     * Retrieves a specific data tag by index.
     *
     * @param index     The index of the tag.
     *
     * @return The data tag at the specified index.
     */
    virtual DataTag getDataTag(size_t index) const = 0;

    /**
     * Get all simple data associated with this block.
     *
     * @return All simple tags as a vector.
     */
    virtual std::vector<DataTag> dataTags() const = 0;

    /**
     * Returns the number of data tag associated with
     * this block.
     *
     * @return The number of data tags.
     */
    virtual size_t dataTagCount() const = 0;

    /**
     * Create a new data tag associated with this block.
     *
     * @param name      The name of the data tag to create.
     * @param type      The type of the tag.
     *
     * @return The newly created tag.
     */
    virtual DataTag createDataTag(const std::string &name, const std::string &type) = 0;

    /**
     * Remove a data tag from the block.
     *
     * @param id        The id of the tag to remove.
     *
     * @return True if the tag was removed, false otherwise.
     */
    virtual bool removeDataTag(const std::string &id) = 0;

};


} // namespace nix


#endif // NIX_I_BLOCK_H
