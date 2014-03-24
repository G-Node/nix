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

// NOTE: though "Block" is an entity with sources it does not inherit "EntityWithSources" 
// since other than that it stores not just references to sources but creates them
class NIXAPI Block : virtual public base::IBlock, public base::EntityWithMetadata<base::IBlock> {

public:

    Block() {}

    Block(const Block &other)
        : EntityWithMetadata(other.impl())
    {
    }

    Block(const std::shared_ptr<base::IBlock> &p_impl)
        : EntityWithMetadata(p_impl)
    {
    }

    //--------------------------------------------------
    // Methods concerning sources
    //--------------------------------------------------

    /**
     * Checks if this block has a specific root source.
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
     * Checks if this block contains a specific source.
     *
     * @param source        The source.
     *
     * @return True if a given source exists at the root, false
     *         otherwise.
     */
    bool hasSource(const Source &source) const {
        if(source == none){
            throw std::runtime_error("Empty Source entity given");
        }
        return backend()->hasSource(source.id());
    }


    /**
     * Retrieves a specific root source.
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
     * Retrieves a specific root source by index.
     *
     * @param index     The index of the source.
     *
     * @return The source at the specified index.
     */
    Source getSource(size_t index) const {
        return backend()->getSource(index);
    }

    /**
     * Returns the number of root sources in this block.
     *
     * @return The number of root sources.
     */
    size_t sourceCount() const {
        return backend()->sourceCount();
    }

    /**
     * Get sources associated with this block.
     *
     * The parameter "filter" is defaulted to giving back all sources.
     * To use your own filter pass a lambda that accepts a "Source"
     * as parameter and returns a bool telling whether to get it or not.
     *
     * @param object filter function of type {@link nix::util::Filter::type}
     * @return object sources as a vector
     */
    std::vector<Source> sources(util::AcceptAll<Source>::type filter
                                = util::AcceptAll<Source>()) const
    {
        auto f = [this] (size_t i) { return getSource(i); };
        return getEntities<Source>(f,
                                   sourceCount(),
                                   filter);
    }

    /**
     * Go through the tree of sources originating from every source in this 
     * block until a max. level of "max_depth" and check for each source
     * whether to return it depending on predicate function "filter".
     * Return resulting vector of sources (which may contain duplicates)
     * or empty vector if none found.
     * 
     * @param object filter function of type std::function<bool(const Source &)>
     * @param int maximum depth to search tree
     * @return object vector of sources
     */
    std::vector<Source> findSources(std::function<bool(Source)> filter = util::AcceptAll<Source>(),
                                    size_t max_depth = std::numeric_limits<size_t>::max()) const;

    /**
     * Create a new root source.
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
     * Deletes a root source and all its child sources from
     * the block.
     *
     * @param id        The id of the source to remove.
     *
     * @return True if the source was removed, false otherwise.
     */
    bool deleteSource(const std::string &id) {
        return backend()->deleteSource(id);
    }

    /**
     * Deletes a root source and all its child sources from
     * the block.
     *
     * @param source        The source to remove.
     *
     * @return True if the source was removed, false otherwise.
     */
    bool deleteSource(const Source &source) {
        if (source == none){
            throw std::runtime_error("Empty Source entity given");
        }
        return backend()->deleteSource(source.id());
    }

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
    bool hasDataArray(const std::string &id) const {
        return backend()->hasDataArray(id);
    }

    /**
     * Checks if a specific data array exists in this block.
     *
     * @param data_array        The data array.
     *
     * @return True if the data array exists, false otherwise.
     */
    bool hasDataArray(const DataArray &data_array) const {
        if (data_array == none){
            throw std::runtime_error("Empty DataArray entity given!");
        }
        return backend()->hasDataArray(data_array.id());
    }

    /**
     * Retrieves a specific data array from the block.
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
     * Retrieves a data array by index.
     *
     * @param index     The index of the data array.
     *
     * @return The data array at the specified index.
     */
    DataArray getDataArray(size_t index) const {
        return backend()->getDataArray(index);
    }

    /**
     * Get data arrays associated with this block.
     *
     * The parameter "filter" is defaulted to giving back all arrays. To
     * use your own filter pass a lambda that accepts a "DataArray"
     * as parameter and returns a bool telling whether to get it or not.
     *
     * @param object filter function of type {@link nix::util::Filter::type}
     * @return object data arrays as a vector     
     */
    std::vector<DataArray> dataArrays(util::AcceptAll<DataArray>::type filter
                                      = util::AcceptAll<DataArray>()) const
    {
        auto f = [this] (size_t i) { return getDataArray(i); };
        return getEntities<DataArray>(f,
                                      dataArrayCount(),
                                      filter);
    }

    /**
     * Returns the number of all data arrays of the block.
     *
     * @return The number of data arrays of the block.
     */
    size_t dataArrayCount() const {
        return backend()->dataArrayCount();
    }

    /**
     * Create a new data array associated with this block.
     *
     * @param name      The name of the data array to create.
     *
     * @return The newly created data array.
     */
    DataArray createDataArray(const std::string &name, const std::string &type) {
        return backend()->createDataArray(name, type);
    }

    /**
     * Deletes a data array from this block.
     *
     * @param id        The id of the data array to remove.
     *
     * @return True if the data array was removed, false otherwise.
     */
    bool deleteDataArray(const std::string &id) {
        return backend()->deleteDataArray(id);
    }

    /**
     * Deletes a data array from this block.
     *
     * @param data_array        The data array to remove.
     *
     * @return True if the data array was removed, false otherwise.
     */
    bool deleteDataArray(const DataArray &data_array) {
        if (data_array == none){
            throw std::runtime_error("Empty DataArray entity given!");
        }
        return backend()->deleteDataArray(data_array.id());
    }
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
    bool hasSimpleTag(const std::string &id) const {
        return backend()->hasSimpleTag(id);
    }

    /**
     * Checks if a specific simple tag exists in the block.
     *
     * @param simple_tag        The simple tag.
     *
     * @return True if the simple tag exists, false otherwise.
     */
    bool hasSimpleTag(const SimpleTag &simple_tag) const {
        if(simple_tag == none){
            throw std::runtime_error("Empty SimpleTag entity given!");
        }
        return backend()->hasSimpleTag(simple_tag.id());
    }


    /**
     * Retrieves a specific simple tag from the block.
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
     * Retrieves a specific simple tag by index.
     *
     * @param index     The index of the tag.
     *
     * @return The simple tag at the specified index.
     */
    SimpleTag getSimpleTag(size_t index) const {
        return backend()->getSimpleTag(index);
    }

    /**
     * Get simple tags associated with this block.
     *
     * The parameter "filter" is defaulted to giving back all tags. To
     * use your own filter pass a lambda that accepts a "SimpleTag"
     * as parameter and returns a bool telling whether to get it or not.
     *
     * @param object filter function of type {@link nix::util::Filter::type}
     * @return object simple tags as a vector     
     */
    std::vector<SimpleTag> simpleTags(util::AcceptAll<SimpleTag>::type filter
                                      = util::AcceptAll<SimpleTag>()) const
    {
        auto f = [this] (size_t i) { return getSimpleTag(i); };
        return getEntities<SimpleTag>(f,
                                      simpleTagCount(),
                                      filter);
    }

    /**
     * Returns the number of simple tag associated with
     * this block.
     *
     * @return The number of simple tags.
     */
    size_t simpleTagCount() const {
        return backend()->simpleTagCount();
    }

    /**
     * Create a new simple tag associated with this block.
     *
     * @param name      The name of the simple tag to create.
     * @param type      The type of the tag.
     * @param vector<double> position
     *
     * @return The newly created tag.
     */
    SimpleTag createSimpleTag(const std::string &name, const std::string &type) {
        return backend()->createSimpleTag(name, type);
    }

    /**
     * Deletes a simple tag from the block.
     *
     * @param id        The id of the tag to remove.
     *
     * @return True if the tag was removed, false otherwise.
     */
    bool deleteSimpleTag(const std::string &id) {
        return backend()->deleteSimpleTag(id);
    }

    /**
     * Deletes a simple tag from the block.
     *
     * @param simple_tag The tag to remove.
     *
     * @return True if the tag was removed, false otherwise.
     */
    bool deleteSimpleTag(const SimpleTag &simple_tag) {
        if (simple_tag == none){
            throw std::runtime_error("Block::deleteSimpleTag: Empty SimpleTag entity given!");
        }
        return backend()->deleteSimpleTag(simple_tag.id());
    }

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
    bool hasDataTag(const std::string &id) const {
        return backend()->hasDataTag(id);
    }

    /**
     * Checks if a specific data tag exists in the block.
     *
     * @param data_tag        The data tag.
     *
     * @return True if the data tag exists, false otherwise.
     */
    bool hasDataTag(const DataTag &data_tag) const {
        if (data_tag == none){
            throw std::runtime_error("Block::hasDataTag: Empty DataTag entitiy given!");
        }
        return backend()->hasDataTag(data_tag.id());
    }

    /**
     * Retrieves a specific data tag from the block.
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
     * Retrieves a specific data tag by index.
     *
     * @param index     The index of the tag.
     *
     * @return The data tag at the specified index.
     */
    DataTag getDataTag(size_t index) const {
        return backend()->getDataTag(index);
    }

    /**
     * Get data tags associated with this block.
     *
     * The parameter "filter" is defaulted to giving back all tags. To
     * use your own filter pass a lambda that accepts a "DataTag"
     * as parameter and returns a bool telling whether to get it or not.
     *
     * @param object filter function of type {@link nix::util::Filter::type}
     * @return object data tags as a vector     
     */
    std::vector<DataTag> dataTags(util::AcceptAll<DataTag>::type filter
                                  = util::AcceptAll<DataTag>()) const
    {
        auto f = [this] (size_t i) { return getDataTag(i); };
        return getEntities<DataTag>(f,
                                    dataTagCount(),
                                    filter);
    }

    /**
     * Returns the number of data tag associated with
     * this block.
     *
     * @return The number of data tags.
     */
    size_t dataTagCount() const {
        return backend()->dataTagCount();
    }

    /**
     * Create a new data tag associated with this block.
     *
     * @param name      The name of the data tag to create.
     * @param type      The type of the tag.
     *
     * @return The newly created tag.
     */
    DataTag createDataTag(const std::string &name, const std::string &type, 
                          const DataArray position) {
        return backend()->createDataTag(name, type, position);
    }

    /**
     * Deletes a data tag from the block.
     *
     * @param id        The id of the tag to remove.
     *
     * @return True if the tag was deleted, false otherwise.
     */
    bool deleteDataTag(const std::string &id) {
        return backend()->deleteDataTag(id);
    }

    /**
     * Deletes a data tag from the block.
     *
     * @param data_tag        The tag to remove.
     *
     * @return True if the tag was deleted, false otherwise.
     */
    bool deleteDataTag(const DataTag &data_tag) {
        if (data_tag == none){
            throw std::runtime_error("Block::deleteDataTag: Empty DataTag entitiy given!");
        }
        return backend()->deleteDataTag(data_tag.id());
    }
    //------------------------------------------------------
    // Operators and other functions
    //------------------------------------------------------

    virtual Block &operator=(none_t) {
        nullify();
        return *this;
    }

    /**
     * Output operator
     */
    friend std::ostream& operator<<(std::ostream &out, const Block &ent) {
        out << "Block: {name = " << ent.name();
        out << ", type = " << ent.type();
        out << ", id = " << ent.id() << "}";
        return out;
    }

};

}


#endif // NIX_BLOCK_H
