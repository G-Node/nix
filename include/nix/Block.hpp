// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BLOCK_H
#define NIX_BLOCK_H

#include <string>

#include <nix/base/EntityWithMetadata.hpp>
#include <nix/base/IBlock.hpp>
#include <nix/Source.hpp>
#include <nix/DataArray.hpp>
#include <nix/SimpleTag.hpp>
#include <nix/DataTag.hpp>

#include <nix/Platform.hpp>


namespace nix {

/**
 * @brief Class for grouping further data entities.
 *
 * The Block entity is a top-level, summarizing element that allows to
 * group the other data entities belonging for example to the same recording session.
 * All data entities such as {@link nix::Source}, {@link nix::DataArray}, {@link nix::SimpleTag} and
 * {@link nix::DataTag} have to be associated with one Block.
 *
 * ### Create a new Block
 * A block can only be created on an existing file object. Do not use the blocks constructors for this
 * purpose.
 *
 * ~~~
 * File f = ...;
 * Block b = f.createBlock("session one", "recording_session");
 * ~~~
 *
 * ### Working with blocks
 * After a block was created it can be used to create further entities. See the documentation of
 * {@link nix::Source}, {@link nix::DataArray}, {@link nix::SimpleTag} and {@link nix::DataTag}
 * for more information.
 * The next example shows how some properties of a block can be accessed.
 *
 * ~~~
 * File f = ...;
 * Block b = f.getBlock(some_id);
 *
 * // set the blocks name
 * std::string name = b.name("session two")
 *
 * // add metadata to a block
 * Section s = f.getSection(sec_id);
 * b.metadata(s);
 *
 * // get associated metadata from a block
 * b.metadata();
 *
 * // remove associated metadata from a block
 * b.metadata(boost::none);
 * ~~~
 *
 * ### Deleting a block
 * When a block is deleted from a file all contained data e.g. sources, data arrays and tags will
 * be removed too.
 *
 * ~~~
 * File f = ...;
 * bool deleted = f.deleteBlock(some_id);
 * cout << "The block was " << deleted ? "" : "not " << "deleted" << endl;
 * ~~~
 */
class NIXAPI Block : public base::EntityWithMetadata<base::IBlock> {

public:

    /**
     * @brief Constructor that creates an uninitialized Block.
     *
     * Calling any method on an uninitialized block will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a block is initialized:
     *
     * ~~~
     * Block e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    Block() {}

    /**
     * @brief Copy constructor.
     *
     * Copying of all NIX front facing objects like block is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The block to copy.
     */
    Block(const Block &other)
        : EntityWithMetadata(other.impl())
    {
    }

    /**
     * @brief Constructor that creates a new entity from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    Block(const std::shared_ptr<base::IBlock> &p_impl)
        : EntityWithMetadata(p_impl)
    {
    }

    /**
     * @brief Constructor with move semantics that creates a new entity from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    Block(std::shared_ptr<base::IBlock> &&ptr)
        : EntityWithMetadata(std::move(ptr))
    {
    }

    //--------------------------------------------------
    // Methods concerning sources
    //--------------------------------------------------

    /**
     * @brief Checks if this block has a specific root source.
     *
     * @param id        The id of the source.
     *
     * @return True if a source with the given id exists at the root, false
     *         otherwise.
     */
    bool hasSource(const std::string &id) const {
        return backend()->hasSource(id);
    }

    /**
     * @brief Checks if this block has a specific root source.
     *
     * @param source    The source to check.
     *
     * @return True if the source exists at the root, false
     *         otherwise.
     */
    bool hasSource(const Source &source) const;

    /**
     * @brief Retrieves a specific root source by its id.
     *
     * @param id        The id of the source.
     *
     * @return The source with the given id. If it doesn't exist an exception
     *         will be thrown.
     */
    Source getSource(const std::string &id) const {
        return backend()->getSource(id);
    }

    /**
     * @brief Retrieves a specific root source by index.
     *
     * @param index     The index of the source.
     *
     * @return The source at the specified index.
     */
    Source getSource(size_t index) const {
        return backend()->getSource(index);
    }

    /**
     * @brief Returns the number of root sources in this block.
     *
     * @return The number of root sources.
     */
    size_t sourceCount() const {
        return backend()->sourceCount();
    }

    /**
     * @brief Get all root sources associated with this block.
     *
     * The parameter filter can be used to filter sources by various
     * criteria. By default a filter is used that accepts all sources.
     *
     * @param filter    A filter function.
     *
     * @return A vector containing the matching root sources.
     */
    std::vector<Source> sources(util::Filter<Source>::type filter = util::AcceptAll<Source>()) const;

    /**
     * @brief Get all sources in this block recursively.
     *
     * This method traverses the tree of all sources in the block. The traversal
     * is accomplished via breadth first and can be limited in depth. On each node or
     * source a filter is applied. If the filter returns true the respective source
     * will be added to the result list.
     * By default a filter is used that accepts all sources.
     *
     * @param filter       A filter function.
     * @param max_depth    The maximum depth of traversal.
     *
     * @return A vector containing the matching sources.
     */
    std::vector<Source> findSources(util::Filter<Source>::type filter = util::AcceptAll<Source>(),
                                    size_t max_depth = std::numeric_limits<size_t>::max()) const;

    /**
     * @brief Create a new root source.
     *
     * @param name      The name of the source to create.
     * @param type      The type of the source.
     *
     * @return The created source object.
     */
    Source createSource(const std::string &name, const std::string &type) {
        return backend()->createSource(name, type);
    }

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
    bool deleteSource(const std::string &id) {
        return backend()->deleteSource(id);
    }

    /**
    * @brief Deletes a root source.
    *
    * This will also delete all child sources of this root source from the file.
    * The deletion of a source can't be undone.
    *
    * @param source    The source to delete.
    *
    * @return True if the source was deleted, false otherwise.
    */
    bool deleteSource(const Source &source);

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
    bool hasDataArray(const std::string &id) const {
        return backend()->hasDataArray(id);
    }

    /**
    * @brief Checks if a specific data array exists in this block.
    *
    * @param data_array        The data array to check.
    *
    * @return True if the data array exists, false otherwise.
    */
    bool hasDataArray(const DataArray &data_array) const;

    /**
     * @brief Retrieves a specific data array from the block by id.
     *
     * @param id        The id of an existing data array.
     *
     * @return The data array with the specified id. If this
     *         doesn't exist, an exception will be thrown.
     */
    DataArray getDataArray(const std::string &id) const {
        return backend()->getDataArray(id);
    }

    /**
     * @brief Retrieves a data array by index.
     *
     * @param index     The index of the data array.
     *
     * @return The data array at the specified index.
     */
    DataArray getDataArray(size_t index) const {
        return backend()->getDataArray(index);
    }

    /**
     * @brief Get data arrays within this block.
     *
     * The parameter filter can be used to filter data arrays by various
     * criteria. By default a filter is used that accepts all data arrays.
     *
     * @param filter    A filter function.
     *
     * @return A vector that contains all filtered data arrays.
     */
    std::vector<DataArray> dataArrays(util::AcceptAll<DataArray>::type filter
                                      = util::AcceptAll<DataArray>()) const;

    /**
     * @brief Returns the number of all data arrays of the block.
     *
     * @return The number of data arrays of the block.
     */
    size_t dataArrayCount() const {
        return backend()->dataArrayCount();
    }

    /**
    * @brief Create a new data array associated with this block.
    *
    * @param name      The name of the data array to create.
    * @param type      The type of the data array.
    * @param data_type A nix::DataType indicating the format to store values.
    * @param shape     A NDSize holding the extent of the array to create.
    *
    * @return The newly created data array.
    */
    DataArray createDataArray(const std::string &name,
                              const std::string &type,
                              nix::DataType      data_type,
                              const NDSize      &shape) {
        return backend()->createDataArray(name, type, data_type, shape);
    }

    /**
    * @brief Create a new data array associated with this block.
    *
    * @param name      The name of the data array to create.
    * @param type      The type of the data array.
    * @param data      Data to create array with.
    *
    * Create a data array with shape and type inferred from data. After
    * successful creation, the contents of data will be written to the
    * data array.
    *
    * @return The newly created data array.
    */
    template<typename T>
    DataArray createDataArray(const std::string &name,
                              const std::string &type,
                              const T &data) {
         const Hydra<const T> hydra(data);
         DataType dtype = hydra.element_data_type();

         const NDSize shape = hydra.shape();
         DataArray da = createDataArray(name, type, dtype, shape);

         const NDSize offset(shape.size(), 0);
         da.setData(data, offset);

         return da;
    }

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
    bool deleteDataArray(const std::string &id) {
        return backend()->deleteDataArray(id);
    }

    /**
    * @brief Deletes a data array from this block.
    *
    * This deletes a data array and all its dimensions from the block and the file.
    * The deletion can't be undone.
    *
    * @param data_array        The data array to delete.
    *
    * @return True if the data array was deleted, false otherwise.
    */
    bool deleteDataArray(const DataArray &data_array);

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
    bool hasSimpleTag(const std::string &id) const {
        return backend()->hasSimpleTag(id);
    }

    /**
    * @brief Checks if a specific simple tag exists in the block.
    *
    * @param simple_tag        The simple tag to check.
    *
    * @return True if the simple tag exists, false otherwise.
    */
    bool hasSimpleTag(const SimpleTag &simple_tag) const;

    /**
     * @brief Retrieves a specific simple tag from the block by its id.
     *
     * @param id        The id of the simple tag.
     *
     * @return The tag with the specified id. If this tag doesn't exist
     *         an exception will be thrown.
     */
    SimpleTag getSimpleTag(const std::string &id) const {
        return backend()->getSimpleTag(id);
    }

    /**
     * @brief Retrieves a specific simple tag by index.
     *
     * @param index     The index of the tag.
     *
     * @return The simple tag at the specified index.
     */
    SimpleTag getSimpleTag(size_t index) const {
        return backend()->getSimpleTag(index);
    }

    /**
     * @brief Get simple tags within this block.
     *
     * The parameter filter can be used to filter simple tags by various
     * criteria. By default a filter is used that accepts all tags.
     *
     * @param filter    A filter function.
     *
     * @return A vector that contains all filtered simple tags.
     */
    std::vector<SimpleTag> simpleTags(util::Filter<SimpleTag>::type filter
                                      = util::AcceptAll<SimpleTag>()) const;

    /**
     * @brief Returns the number of simple tags within this block.
     *
     * @return The number of simple tags.
     */
    size_t simpleTagCount() const {
        return backend()->simpleTagCount();
    }

    /**
     * @brief Create a new simple tag associated with this block.
     *
     * @param name      The name of the simple tag to create.
     * @param type      The type of the tag.
     * @param refs      A Vector with referenced data array entities.
     *
     * @return The newly created tag.
     */
    SimpleTag createSimpleTag(const std::string &name, const std::string &type,
                              const std::vector<DataArray> &refs) {
        return backend()->createSimpleTag(name, type, refs);
    }

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
    bool deleteSimpleTag(const std::string &id) {
        return backend()->deleteSimpleTag(id);
    }

    /**
    * @brief Deletes a simple tag from the block.
    *
    * Deletes a simple tag with all its features from the block and the file.
    * The deletion can't be undone.
    *
    * @param simple_tag        The tag to remove.
    *
    * @return True if the tag was removed, false otherwise.
    */
    bool deleteSimpleTag(const SimpleTag &simple_tag);

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
    bool hasDataTag(const std::string &id) const {
        return backend()->hasDataTag(id);
    }

    /**
    * @brief Checks if a specific data tag exists in the block.
    *
    * @param data_tag          The data tag to check.
    *
    * @return True if the data tag exists, false otherwise.
    */
    bool hasDataTag(const DataTag &data_tag) const;

    /**
     * @brief Retrieves a specific data tag from the block by its id.
     *
     * @param id        The id of the data tag.
     *
     * @return The tag with the specified id. If this tag doesn't exist
     *         an exception will be thrown.
     */
    DataTag getDataTag(const std::string &id) const {
        return backend()->getDataTag(id);
    }

    /**
     * @brief Retrieves a specific data tag by index.
     *
     * @param index     The index of the tag.
     *
     * @return The data tag at the specified index.
     */
    DataTag getDataTag(size_t index) const {
        return backend()->getDataTag(index);
    }

    /**
     * @brief Get data tags within this block.
     *
     * The parameter filter can be used to filter data tags by various
     * criteria. By default a filter is used that accepts all tags.
     *
     * @param filter    A filter function.
     *
     * @return A vector that contains all filtered data tags.
     */
    std::vector<DataTag> dataTags(util::AcceptAll<DataTag>::type filter
                                  = util::AcceptAll<DataTag>()) const;

    /**
     * @brief Returns the number of data tags associated with this block.
     *
     * @return The number of data tags.
     */
    size_t dataTagCount() const {
        return backend()->dataTagCount();
    }

    /**
     * @brief Create a new data tag associated with this block.
     *
     * @param name      The name of the data tag to create.
     * @param type      The type of the tag.
     * @param positions The positions of the tag.
     *
     * @return The newly created tag.
     */
    DataTag createDataTag(const std::string &name, const std::string &type,
                          const DataArray position) {
        return backend()->createDataTag(name, type, position);
    }

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
    bool deleteDataTag(const std::string &id) {
        return backend()->deleteDataTag(id);
    }

    /**
    * @brief Deletes a data tag from the block.
    *
    * Deletes a data tag and all its features from the block and the file.
    * The deletion can't be undone.
    *
    * @param data_tag  The tag to remove.
    *
    * @return True if the tag was removed, false otherwise.
    */
    bool deleteDataTag(const DataTag &data_tag);

    //------------------------------------------------------
    // Operators and other functions
    //------------------------------------------------------

    /**
     * @brief Assignment operator for none.
     */
    virtual Block &operator=(none_t) {
        nullify();
        return *this;
    }

    /**
     * @brief Output operator
     */
    friend std::ostream & operator<<(std::ostream &out, const Block &ent);

};

}


#endif // NIX_BLOCK_H
