// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BLOCK_H
#define NIX_BLOCK_H

#include <nix/base/EntityWithMetadata.hpp>
#include <nix/base/IBlock.hpp>
#include <nix/Source.hpp>
#include <nix/DataArray.hpp>
#include <nix/MultiTag.hpp>
#include <nix/Tag.hpp>
#include <nix/Group.hpp>
#include <nix/Platform.hpp>

#include <nix/util/util.hpp>

#include <string>
#include <memory>

namespace nix {

/**
 * @brief Class for grouping further data entities.
 *
 * The Block entity is a top-level, summarizing element that allows to
 * group the other data entities belonging for example to the same recording session.
 * All data entities such as {@link nix::Source}, {@link nix::DataArray}, {@link nix::Tag} and
 * {@link nix::MultiTag} have to be associated with one Block.
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
 * {@link nix::Source}, {@link nix::DataArray}, {@link nix::Tag} and {@link nix::MultiTag}
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
     * @param name_or_id        Name or id of the source.
     *
     * @return True if a source with the given id exists at the root, false
     *         otherwise.
     */
    bool hasSource(const std::string &name_or_id) const {
        return backend()->hasSource(name_or_id);
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
     * @param name_or_id        Name or id of the source.
     *
     * @return The source with the given id. If it doesn't exist an exception
     *         will be thrown.
     */
    Source getSource(const std::string &name_or_id) const {
        return backend()->getSource(name_or_id);
    }

    /**
     * @brief Retrieves a specific root source by index.
     *
     * @param index     The index of the source.
     *
     * @return The source at the specified index.
     */
    Source getSource(ndsize_t index) const;

    /**
     * @brief Returns the number of root sources in this block.
     *
     * @return The number of root sources.
     */
    ndsize_t sourceCount() const {
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
    std::vector<Source> sources(const util::Filter<Source>::type &filter = util::AcceptAll<Source>()) const;

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
    std::vector<Source> findSources(const util::Filter<Source>::type &filter = util::AcceptAll<Source>(),
                                    size_t max_depth = std::numeric_limits<size_t>::max()) const;

    /**
     * @brief Create a new root source.
     *
     * @param name      The name of the source to create.
     * @param type      The type of the source.
     *
     * @return The created source object.
     */
    Source createSource(const std::string &name, const std::string &type);

    /**
     * @brief Deletes a root source.
     *
     * This will also delete all child sources of this root source from the file.
     * The deletion of a source can't be undone.
     *
     * @param name_or_id        Name or id of the source to delete.
     *
     * @return True if the source was deleted, false otherwise.
     */
    bool deleteSource(const std::string &name_or_id) {
        return backend()->deleteSource(name_or_id);
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
     * @param name_or_id        Name or id of a data array.
     *
     * @return True if the data array exists, false otherwise.
     */
    bool hasDataArray(const std::string &name_or_id) const {
        return backend()->hasEntity({name_or_id, ObjectType::DataArray});
    }

    /**
    * @brief Checks if a specific data array exists in this block.
    *
    * @param data_array        The data array to check.
    *
    * @return True if the data array exists, false otherwise.
    */
    bool hasDataArray(const DataArray &data_array) const {
        if (!util::checkEntityInput(data_array, false)) {
            return false;
        }
        return backend()->hasEntity(data_array);
    }

    /**
     * @brief Retrieves a specific data array from the block by name or id.
     *
     * @param name_or_id        Name or id of an existing data array.
     *
     * @return The data array with the specified id. If this
     *         doesn't exist, an exception will be thrown.
     */
    DataArray getDataArray(const std::string &name_or_id) const {
        return backend()->getEntity<base::IDataArray>(name_or_id);
    }


    /**
     * @brief Retrieves a data array by index.
     *
     * @param index     The index of the data array.
     *
     * @return The data array at the specified index.
     */
    DataArray getDataArray(ndsize_t index) const {
        if (index >= dataArrayCount()) {
            throw OutOfBounds("Block::getDataArray: index is out of bounds!");
        }
        return backend()->getEntity<base::IDataArray>(index);
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
    std::vector<DataArray> dataArrays(const util::AcceptAll<DataArray>::type &filter
                                      = util::AcceptAll<DataArray>()) const;

    /**
     * @brief Returns the number of all data arrays of the block.
     *
     * @return The number of data arrays of the block.
     */
    ndsize_t dataArrayCount() const {
        return backend()->entityCount(ObjectType::DataArray);
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
                              const NDSize      &shape);

    /**
    * @brief Create a new data array associated with this block.
    *
    * @param name      The name of the data array to create.
    * @param type      The type of the data array.
    * @param data      Data to create array with.
    * @param data_type A optional nix::DataType indicating the format to store values.
    *
    * Create a data array with shape and type inferred from data. After
    * successful creation, the contents of data will be written to the
    * data array. If data_type has been specified the DataArray will be created
    * with the specified type instead of the type inferred from the data.
    *
    * @return The newly created data array.
    */
    template<typename T>
    DataArray createDataArray(const std::string &name,
                              const std::string &type,
                              const T &data,
                              DataType data_type = DataType::Nothing) {
         const Hydra<const T> hydra(data);

         if (data_type == DataType::Nothing) {
              data_type = hydra.element_data_type();
         }

         const NDSize shape = hydra.shape();
         DataArray da = createDataArray(name, type, data_type, shape);

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
     * @param name_or_id        Name or id of the data array to delete.
     *
     * @return True if the data array was deleted, false otherwise.
     */
    bool deleteDataArray(const std::string &name_or_id) {
        return backend()->removeEntity({name_or_id, ObjectType::DataArray});
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
    bool deleteDataArray(const DataArray &data_array) {
        if (!util::checkEntityInput(data_array, false)) {
            return false;
        }
        return backend()->removeEntity(data_array);
    }

    //--------------------------------------------------
    // Methods concerning tags.
    //--------------------------------------------------

    /**
     * @brief Checks if a specific tag exists in the block.
     *
     * @param name_or_id        Name or id of a tag.
     *
     * @return True if the tag exists, false otherwise.
     */
    bool hasTag(const std::string &name_or_id) const {
        return backend()->hasTag(name_or_id);
    }

    /**
    * @brief Checks if a specific tag exists in the block.
    *
    * @param tag        The tag to check.
    *
    * @return True if the tag exists, false otherwise.
    */
    bool hasTag(const Tag &tag) const;

    /**
     * @brief Retrieves a specific tag from the block by its id.
     *
     * @param name_or_id        Name or id of the tag.
     *
     * @return The tag with the specified id. If this tag doesn't exist
     *         an exception will be thrown.
     */
    Tag getTag(const std::string &name_or_id) const {
        return backend()->getTag(name_or_id);
    }

    /**
     * @brief Retrieves a specific tag by index.
     *
     * @param index     The index of the tag.
     *
     * @return The tag at the specified index.
     */
    Tag getTag(ndsize_t index) const;

    /**
     * @brief Get tags within this block.
     *
     * The parameter filter can be used to filter tags by various
     * criteria. By default a filter is used that accepts all tags.
     *
     * @param filter    A filter function.
     *
     * @return A vector that contains all filtered tags.
     */
    std::vector<Tag> tags(const util::Filter<Tag>::type &filter
                          = util::AcceptAll<Tag>()) const;

    /**
     * @brief Returns the number of tags within this block.
     *
     * @return The number of tags.
     */
    ndsize_t tagCount() const {
        return backend()->tagCount();
    }

    /**
     * @brief Create a new tag associated with this block.
     *
     * @param name      The name of the tag to create.
     * @param type      The type of the tag.
     * @param position  The position of the tag.
     *
     * @return The newly created tag.
     */
    Tag createTag(const std::string &name, const std::string &type,
                              const std::vector<double> &position);

    /**
     * @brief Deletes a tag from the block.
     *
     * Deletes a tag with all its features from the block and the file.
     * The deletion can't be undone.
     *
     * @param name_or_id        Name or id of the tag to remove.
     *
     * @return True if the tag was removed, false otherwise.
     */
    bool deleteTag(const std::string &name_or_id) {
        return backend()->deleteTag(name_or_id);
    }

    /**
    * @brief Deletes a tag from the block.
    *
    * Deletes a tag with all its features from the block and the file.
    * The deletion can't be undone.
    *
    * @param tag        The tag to remove.
    *
    * @return True if the tag was removed, false otherwise.
    */
    bool deleteTag(const Tag &tag);

    //--------------------------------------------------
    // Methods concerning multi tags.
    //--------------------------------------------------

    /**
     * @brief Checks if a specific multi tag exists in the block.
     *
     * @param name_or_id        Name or id of a multi tag.
     *
     * @return True if the multi tag exists, false otherwise.
     */
    bool hasMultiTag(const std::string &name_or_id) const {
        return backend()->hasMultiTag(name_or_id);
    }

    /**
    * @brief Checks if a specific multi tag exists in the block.
    *
    * @param multi_tag          The multi tag to check.
    *
    * @return True if the multi tag exists, false otherwise.
    */
    bool hasMultiTag(const MultiTag &multi_tag) const;

    /**
     * @brief Retrieves a specific multi tag from the block by its id.
     *
     * @param name_or_id        Name or id of the multi tag.
     *
     * @return The tag with the specified id. If this tag doesn't exist
     *         an exception will be thrown.
     */
    MultiTag getMultiTag(const std::string &name_or_id) const {
        return backend()->getMultiTag(name_or_id);
    }

    /**
     * @brief Retrieves a specific multi tag by index.
     *
     * @param index     The index of the tag.
     *
     * @return The multi tag at the specified index.
     */
    MultiTag getMultiTag(ndsize_t index) const;

    /**
     * @brief Get multi tags within this block.
     *
     * The parameter filter can be used to filter multi tags by various
     * criteria. By default a filter is used that accepts all tags.
     *
     * @param filter    A filter function.
     *
     * @return A vector that contains all filtered multi tags.
     */
    std::vector<MultiTag> multiTags(const util::AcceptAll<MultiTag>::type &filter
                                  = util::AcceptAll<MultiTag>()) const;

    /**
     * @brief Returns the number of multi tags associated with this block.
     *
     * @return The number of multi tags.
     */
    ndsize_t multiTagCount() const {
        return backend()->multiTagCount();
    }

    /**
     * @brief Create a new multi tag associated with this block.
     *
     * @param name       The name of the multi tag to create.
     * @param type       The type of the tag.
     * @param positions  The positions of the tag.
     *
     * @return The newly created tag.
     */
    MultiTag createMultiTag(const std::string &name, const std::string &type,
                          const DataArray &positions);

    /**
     * @brief Deletes a multi tag from the block.
     *
     * Deletes a multi tag and all its features from the block and the file.
     * The deletion can't be undone.
     *
     * @param name_or_id        Name or id of the tag to remove.
     *
     * @return True if the tag was removed, false otherwise.
     */
    bool deleteMultiTag(const std::string &name_or_id) {
        return backend()->deleteMultiTag(name_or_id);
    }

    /**
    * @brief Deletes a multi tag from the block.
    *
    * Deletes a multi tag and all its features from the block and the file.
    * The deletion can't be undone.
    *
    * @param multi_tag  The tag to remove.
    *
    * @return True if the tag was removed, false otherwise.
    */
    bool deleteMultiTag(const MultiTag &multi_tag);

    //--------------------------------------------------
    // Methods concerning groups
    //--------------------------------------------------
    /**
    * @brief Checks if a specific group exists in the block.
    *
    * @param name_or_id        Name or id of a group.
    *
    * @return True if the group exists, false otherwise.
    */
    bool hasGroup(const std::string &name_or_id) const {
        return backend()->hasGroup(name_or_id);
    }

    /**
    * @brief Checks if a specific group exists in the block.
    *
    * @param group          The group to check.
    *
    * @return True if the group exists, false otherwise.
    */
    bool hasGroup(const Group &group) const;

    /**
     * @brief Retrieves a specific group from the block by its id.
     *
     * @param name_or_id        Name or id of the group.
     *
     * @return The group with the specified id. If this tag doesn't exist
     *         an exception will be thrown.
     */
    Group getGroup(const std::string &name_or_id) const {
        return backend()->getGroup(name_or_id);
    }

    /**
     * @brief Retrieves a specific group by index.
     *
     * @param index     The index of the group.
     *
     * @return The group at the specified index.
     */
    Group getGroup(ndsize_t index) const {
        return backend()->getGroup(index);
    }

    /**
     * @brief Get groups within this block.
     *
     * The parameter filter can be used to filter groups by various
     * criteria. By default a filter is used that accepts all groups.
     *
     * @param filter    A filter function.
     *
     * @return A vector that contains all filtered groups.
     */
    std::vector<Group> groups(const util::AcceptAll<Group>::type &filter
    = util::AcceptAll<Group>()) const;

    /**
     * @brief Returns the number of groups associated with this block.
     *
     * @return The number of groups.
     */
    ndsize_t groupCount() const {
        return backend()->groupCount();
    }

    /**
     * @brief Create a new group associated with this block.
     *
     * @param name       The name of the group to create.
     * @param type       The type of the tag.
     *
     * @return The newly created group.
     */
    Group createGroup(const std::string &name, const std::string &type);

    /**
     * @brief Deletes a Group from the block.
     *
     * Deletes a group and all its features from the block and the file.
     * The deletion can't be undone.
     *
     * @param name_or_id        Name or id of the group to remove.
     *
     * @return True if the group was removed, false otherwise.
     */
    bool deleteGroup(const std::string &name_or_id) {
        return backend()->deleteGroup(name_or_id);
    }

    /**
    * @brief Deletes a group from the block.
    *
    * Deletes a group and all its features from the block and the file.
    * The deletion can't be undone.
    *
    * @param Group  The group to remove.
    *
    * @return True if the group was removed, false otherwise.
    */
    bool deleteGroup(const Group &multi_tag);

    //------------------------------------------------------
    // Operators and other functions
    //------------------------------------------------------

    /**
     * @brief Assignment operator for none.
     */
    Block &operator=(const none_t &t) {
        ImplContainer::operator=(t);
        return *this;
    }

    /**
     * @brief Output operator
     */
    NIXAPI friend std::ostream &operator<<(std::ostream &out, const Block &ent);
};

template<>
struct objectToType<Block> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::Block;
};

}


#endif // NIX_BLOCK_H
