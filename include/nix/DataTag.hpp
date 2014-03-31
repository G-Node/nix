// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_DATA_TAG_H
#define NIX_DATA_TAG_H

#include <nix/base/IDataTag.hpp>
#include <nix/base/EntityWithSources.hpp>
#include <nix/Representation.hpp>

#include <nix/Platform.hpp>

namespace nix {


class NIXAPI DataTag : virtual public base::IDataTag, public base::EntityWithSources<base::IDataTag> {

public:

    DataTag()
: EntityWithSources()
{}

    DataTag(const DataTag &other)
    : EntityWithSources(other.impl())
    {
    }

    DataTag(const std::shared_ptr<base::IDataTag> &p_impl)
    : EntityWithSources(p_impl)
    {
    }

    //--------------------------------------------------
    // Positions and extents
    //--------------------------------------------------

    /**
     * Getter for the positions of a tag. The positions are strored in a DataArray.
     *
     *
     * @return The positions DataArray.
     */
    DataArray positions() const {
        return backend()->positions();
    }

    /**
     * Setter for the positions of a tag.
     *
     * @param id   string.
     */
    void positions(const std::string &id) {
        backend()->positions(id);
    }

    /**
     * Setter for the positions of a tag.
     *
     * @param position   DataArray.
     */
    void positions(const DataArray &positions) {
        if(positions == none) {
            throw std::runtime_error("Empty positions entity (DataArray) given");
        }
        else {
            backend()->positions(positions.id());
        }
    }
    
    /**
     * Returns whether this DataArray contains positions.
     *
     * @return bool
     */
    bool hasPositions() const {
        return backend()->hasPositions();
    }

    /**
     * Getter for the extents of the tag which are stored in a#
     * DataArray
     *
     * @return The extents of the tag.
     */
    DataArray extents() const {
        return backend()->extents();
    }

    /**
     * Sets the extents DataArray of the tag.
     *
     * @param extent      The extent vector.
     */
    void extents(const DataArray &extents) {
        if(extents == none) {
            backend()->extents(none);
        }
        else {
            backend()->extents(extents.id());
        }
    }

    /**
     * Set the reference to the extents DataArray.
     *
     * @param the id.
     */
    void extents(const std::string &id) {
        backend()->extents(id);
    }

    /**
     * Deleter for the reference to the extents DataArray.
     * Note: This function does not delete the referenced DataArray!
     *
     * @param boost::none_t.
     */
    void extents(const boost::none_t t) {
        backend()->extents(t);
    }


    //--------------------------------------------------
    // Methods concerning references.
    // TODO implement when done with the DataArray class.
    //--------------------------------------------------

    /**
     * Checks if the specified DataArray is referenced in
     * this DataTag.
     *
     * @param DataArray     the dataArray
     *
     * @return bool
     */
    bool hasReference(const DataArray &reference) const {
        if (reference == none) {
            throw std::runtime_error("DataTag::hasReference: Empty DataArray entity given!");
        }
        return backend()->hasReference(reference.id());
    }

    /**
     * Checks if a DataArray with the specified id is referenced in
     * this DataTag.
     *
     * @param std::string the id
     *
     * @return bool
     */
    bool hasReference(const std::string &id) const {
        return backend()->hasReference(id);
    }

    /**
     * Returns the count of references.
     *
     * @return size_t the count
     */
    size_t referenceCount() const {
        return backend()->referenceCount();
    }

    /**
     * Returns the specified DataArray.
     *
     * @param std::string the id
     *
     * @return DataArray object may be false if not found.
     *
     */
    DataArray getReference(const std::string &id) const {
        return backend()->getReference(id);
    }

    /**
     * Returns a certain reference that is specified by its index.
     *
     * @param size_t the index
     *
     * @return DataArray result may be false if not found
     *
     */
    DataArray getReference(size_t index) const {
        return backend()->getReference(index);
    }

    /**
     * Adds a reference to a DataArray to the list of References.
     *
     * @param std::string the id of a DataArray.
     *
     */
    void addReference(const std::string &id) {
        backend()->addReference(id);
    }

    /**
     * Adds a reference to a DataArray to the list of References.
     *
     * @param DataArray the DataArray that should be referenced.
     *
     */
    void addReference(const DataArray &reference) {
        if (reference == none) {
            throw std::runtime_error("DataTag::addReference: Empty DataArray entity given!");
        }
        backend()->addReference(reference.id());
    }

    /**
     * Removes a certain DataArray from the list of References.
     *
     * @param  id      The id of the DataArray
     *
     * @return bool    whether the operation succeeded.
     *
     */
    bool removeReference(const std::string &id) {
        return backend()->removeReference(id);
    }

    /**
     * Removes a certain DataArray from the list of References.
     *
     * @param reference   The DataArray reference
     *
     * @return bool         whether the operation succeeded.
     *
     */
    bool removeReference(const DataArray &reference) {
        if (reference == none) {
            throw std::runtime_error("DataTag::removeReference: Empty DataArray reference given!");
        }
        return backend()->removeReference(reference.id());
    }

    /**
     * Get referenced data arrays associated with this data tag.
     *
     * The parameter "filter" is defaulted to giving back all arrays. To
     * use your own filter pass a lambda that accepts a "DataArray"
     * as parameter and returns a bool telling whether to get it or not.
     *
     * @param object filter function of type {@link nix::util::Filter::type}
     * @return object referenced data arrays as a vector     
     */
    std::vector<DataArray> references(util::AcceptAll<DataArray>::type filter
                                      = util::AcceptAll<DataArray>()) const
    {
        auto f = [this] (size_t i) { return getReference(i); };
        return getEntities<DataArray>(f,
                                      referenceCount(),
                                      filter);
    }

    /**
     * Setter for all referenced DataArrays. Previously referenced
     * DataArrays, that are not in the references vector will be
     * removed.
     *
     * @param references    All referenced arrays.
     */
    void references(const std::vector<DataArray> &references) {
        backend()->references(references);
    }

    //TODO to be implemented later
    /*
    template<typename T, size_t dims>
    void getReferencedData(std::vector<boost::multi_array<T, dims>> &data, size_t index) const{

        if (referenceCount() == 0) {
            throw std::runtime_error("DataTagHDF5::getReferencedData: There is no reference attached to this tag!");
        }
        if(!hasPositions()) {
            throw std::runtime_error("DataTagHDF5::getReferencedData: There is no positions array attached to this tag!");
        }

        DataArray pa = positions();
        boost::multi_array<double,1> posData, extData;
        pa.getData(posData);

        if(index >= posData.shape()[0]) {
            throw std::runtime_error("DataTagHDF5::getReferencedData: index exeeds matrix dimensions in positions data!");
        }

        if(hasExtents()) {
            DataArray ea = extents();
            ea.getData(extData);
        }

        //TODO convert position and extent to respective units
        //TODO get the data slice from the referenced DataArrays
        std::vector<DataArray> refs = references();
        for (size_t i = 0; i < refs.size();i++) {

        }
    }
    */
    //--------------------------------------------------
    // Methods concerning representations.
    //--------------------------------------------------

    /**
     * Checks if a specific representation exists on the tag.
     *
     * @param id        The id of a representation.
     *
     * @return True if the representation exists, false otherwise.
     */
    bool hasRepresentation(const std::string &id) const {
        return backend()->hasRepresentation(id);
    }

    /**
     * Checks if a specific representation exists on the tag.
     *
     * @param representation        The Representation.
     *
     * @return bool                 True if the representation exists, false otherwise.
     */
    bool hasRepresentation(const Representation &representation) const {
        if (representation == none) {
            throw std::runtime_error("DataTag::hasRepresentation: Empty representation given!");
        }
        return backend()->hasRepresentation(representation.id());
    }

    /**
     * Returns the number of representations in this block.
     *
     * @return The number of representations.
     */
    size_t representationCount() const {
        return backend()->referenceCount();
    }

    /**
     * Retrieves a specific representation from the tag.
     *
     * @param id        The id of the representation.
     *
     * @return The representation with the specified id. If it doesn't exist
     *         an exception will be thrown.
     */
    Representation getRepresentation(const std::string &id) const {
        return backend()->getRepresentation(id);
    }

    /**
     * Retrieves a specific representation from the tag.
     *
     * @param index        The index of the representation.
     *
     * @return The representation with the specified index.
     */
    Representation getRepresentation(size_t index) const {
        return backend()->getRepresentation(index);
    }

    /**
     * Get all representations of this data tag.
     *
     * The parameter "filter" is defaulted to giving back all 
     * representations. To use your own filter pass a lambda 
     * that accepts a "Representation" as parameter and returns a bool 
     * telling whether to get it or not.
     *
     * @param object filter function of type {@link nix::util::Filter::type}
     * @return object representations as a vector     
     */
    std::vector<Representation> representations(
                                  util::AcceptAll<Representation>::type filter
                                  = util::AcceptAll<Representation>()) const
    {
        auto f = [this] (size_t i) { return getRepresentation(i); };
        return getEntities<Representation>(f,
                                    representationCount(),
                                    filter);
    }

    /**
     * Create a new representation.
     *
     * @param data      The data array of this representation.
     * @param type      The link type of this representation.
     *
     * @return The created representation object.
     */
    Representation createRepresentation(const DataArray &data, LinkType link_type) {
        return backend()->createRepresentation(data.id(), link_type);
    }

    /**
     * Create a new representation.
     *
     * @param data      The data array of this representation.
     * @param type      The link type of this representation.
     *
     * @return The created representation object.
     */
    Representation createRepresentation(const std::string &data_array_id, LinkType link_type) {
        return backend()->createRepresentation(data_array_id, link_type);
    }

    /**
     * Delete a representation from the tag.
     *
     * @param id        The id of the representation to delete.
     *
     * @return True if the representation was removed, false otherwise.
     */
    bool deleteRepresentation(const std::string &id) {
        return backend()->deleteRepresentation(id);
    }

    /**
     * Delete a representation from the tag.
     *
     * @param representation        The representation to delete.
     *
     * @return bool True if the representation was removed, false otherwise.
     */
    bool deleteRepresentation(const Representation &representation) {
        if (representation == none) {
            throw std::runtime_error("DataTag::deleteRepresentation: Empty Representation entity given!");
        }
        return backend()->deleteRepresentation(representation.id());
    }

    virtual DataTag &operator=(none_t) {
        nullify();
        return *this;
    }

    /**
     * Output operator
     */
    friend std::ostream& operator<<(std::ostream &out, const DataTag &ent) {
        out << "DataTag: {name = " << ent.name();
        out << ", type = " << ent.type();
        out << ", id = " << ent.id() << "}";
        return out;
    }

};


} // namespace nix

#endif // NIX_DATA_TAG_H
