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
#include <nix/Feature.hpp>

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

    DataTag(std::shared_ptr<base::IDataTag> &&ptr)
    : EntityWithSources(std::move(ptr))
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

    /**
     * Getter for the units of the tag. The units are applied to all values for position
     * and extent in order to calculate the right position vectors in referenced data arrays.
     *
     * @return All units of the tag as a vector.
     */
    std::vector<std::string> units() const {
        return backend()->units();
    }

    /**
     * Setter for the units of a tag.
     *
     * @param units     All units as a vector.
     */
    void units(std::vector<std::string> &units) {
        for (std::vector<std::string>::iterator iter = units.begin(); iter != units.end(); ++iter) {
            //if (!(util::isSIUnit(*iter) || util::isCompoundSIUnit(*iter))) { TODO
            if ((*iter).length() > 0 && !(util::isSIUnit(*iter))) {
                std::string msg = "Unit " + *iter +"is not a SI unit. Note: so far only atomic SI units are supported.";
                throw InvalidUnit(msg, "DataTag::units(vector<string> &units)");
            }
        }
        backend()->units(units);
    }

    /**
     * Deleter for the units of a tag.
     *
     * @param boost::none_t.
     */
    void units(const boost::none_t t) {
        backend()->units(t);
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
    // Methods concerning features.
    //--------------------------------------------------

    /**
     * Checks if a specific feature exists on the tag.
     *
     * @param id        The id of a feature.
     *
     * @return True if the feature exists, false otherwise.
     */
    bool hasFeature(const std::string &id) const {
        return backend()->hasFeature(id);
    }

    /**
     * Checks if a specific feature exists on the tag.
     *
     * @param feature        The Feature.
     *
     * @return bool                 True if the feature exists, false otherwise.
     */
    bool hasFeature(const Feature &feature) const {
        if (feature == none) {
            throw std::runtime_error("DataTag::hasFeature: Empty feature given!");
        }
        return backend()->hasFeature(feature.id());
    }

    /**
     * Returns the number of features in this block.
     *
     * @return The number of features.
     */
    size_t featureCount() const {
        return backend()->referenceCount();
    }

    /**
     * Retrieves a specific feature from the tag.
     *
     * @param id        The id of the feature.
     *
     * @return The feature with the specified id. If it doesn't exist
     *         an exception will be thrown.
     */
    Feature getFeature(const std::string &id) const {
        return backend()->getFeature(id);
    }

    /**
     * Retrieves a specific feature from the tag.
     *
     * @param index        The index of the feature.
     *
     * @return The feature with the specified index.
     */
    Feature getFeature(size_t index) const {
        return backend()->getFeature(index);
    }

    /**
     * Get all features of this data tag.
     *
     * The parameter "filter" is defaulted to giving back all 
     * features. To use your own filter pass a lambda 
     * that accepts a "Feature" as parameter and returns a bool 
     * telling whether to get it or not.
     *
     * @param object filter function of type {@link nix::util::Filter::type}
     * @return object features as a vector     
     */
    std::vector<Feature> features(
                                  util::AcceptAll<Feature>::type filter
                                  = util::AcceptAll<Feature>()) const
    {
        auto f = [this] (size_t i) { return getFeature(i); };
        return getEntities<Feature>(f,
                                    featureCount(),
                                    filter);
    }

    /**
     * Create a new feature.
     *
     * @param data      The data array of this feature.
     * @param type      The link type of this feature.
     *
     * @return The created feature object.
     */
    Feature createFeature(const DataArray &data, LinkType link_type) {
        return backend()->createFeature(data.id(), link_type);
    }

    /**
     * Create a new feature.
     *
     * @param data      The data array of this feature.
     * @param type      The link type of this feature.
     *
     * @return The created feature object.
     */
    Feature createFeature(const std::string &data_array_id, LinkType link_type) {
        return backend()->createFeature(data_array_id, link_type);
    }

    /**
     * Delete a feature from the tag.
     *
     * @param id        The id of the feature to delete.
     *
     * @return True if the feature was removed, false otherwise.
     */
    bool deleteFeature(const std::string &id) {
        return backend()->deleteFeature(id);
    }

    /**
     * Delete a feature from the tag.
     *
     * @param feature        The feature to delete.
     *
     * @return bool True if the feature was removed, false otherwise.
     */
    bool deleteFeature(const Feature &feature) {
        if (feature == none) {
            throw std::runtime_error("DataTag::deleteFeature: Empty Feature entity given!");
        }
        return backend()->deleteFeature(feature.id());
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
