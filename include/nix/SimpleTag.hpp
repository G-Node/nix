// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_SIMPLE_TAG_H
#define NIX_SIMPLE_TAG_H

#include <nix/base/EntityWithSources.hpp>
#include <nix/base/ISimpleTag.hpp>
#include <nix/Section.hpp>
#include <nix/DataArray.hpp>
#include <nix/Feature.hpp>

#include <nix/Platform.hpp>

namespace nix {


// TODO
class NIXAPI SimpleTag : virtual public base::ISimpleTag, public base::EntityWithSources<base::ISimpleTag> {

public:

    SimpleTag()
        : EntityWithSources()
    {
    }


    SimpleTag(const SimpleTag &other)
        : EntityWithSources(other.impl())
    {
    }


    SimpleTag(const std::shared_ptr<base::ISimpleTag> &p_impl)
        : EntityWithSources(p_impl)
    {
    }


    SimpleTag(std::shared_ptr<base::ISimpleTag> &&ptr)
        : EntityWithSources(std::move(ptr))
    {
    }

    //--------------------------------------------------
    // Element getters and setters
    //--------------------------------------------------

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
                throw InvalidUnit(msg, "SimpleTag::units(vector<string> &units)");
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
    
    /**
     * Getter for the position of a tag. The position is a vector that
     * points into referenced DataArrays.
     *
     * @return The position vector.
     */
    std::vector<double> position() const {
        return backend()->position();
    }

    /**
     * Setter for the position of a tag.
     *
     * @param position    The position vector.
     */
    void position(const std::vector<double> &position) {
        backend()->position(position);
    }
    
    /**
     * Deleter for the position of a tag.
     *
     * @param boost::none_t.
     */
    void position(const boost::none_t t) {
        backend()->position(t);
    }
    
    /**
     * Getter for the extent of a tag. Given a specified position
     * vector, the extent vector defined the size of a region of
     * interest in the referenced DataArrays.
     *
     * @return The extent of the tag.
     */
    std::vector<double> extent() const {
        return backend()->extent();
    }

    /**
     * Setter for the extent of a tag.
     *
     * @param extent      The extent vector.
     */
    void extent(const std::vector<double> &extent) {
        backend()->extent(extent);
    }
    
    /**
     * Deleter for the extent of a tag.
     *
     * @param boost::none_t.
     */
    void extent(const boost::none_t t) {
        backend()->extent(t);
    }

    //--------------------------------------------------
    // Methods concerning references.
    //--------------------------------------------------
   
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
     * Checks if the specified DataArray is referenced in
     * this SimpleTag.
     *
     * @param DataArray     the dataArray
     *
     * @return bool
     */
    bool hasReference(const DataArray &reference) const {
        if (reference == none) {
            throw std::runtime_error("SimpleTag::hasReference: Emty DataArray entity given!");
        }
        return backend()->hasReference(reference.id());
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
     * @param DataArray the DataArray tha should be referenced.
     *
     */
    void addReference(const DataArray &reference) {
        if (reference == none) {
            throw std::runtime_error("SimpleTag::addReference: Empty DataArray entity given!");
        }
        backend()->addReference(reference.id());
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
     * Removes a certain DataArray from the list of References.
     *
     * @param DataArray the DataArray
     *
     * @return bool whether the operation succeeded.
     *
     */
    bool removeReference(const DataArray &reference) {
        if(reference == none) {
            throw std::runtime_error("SimpleTag::removeReference: Empty DataArray entity given!");
        }
        return backend()->removeReference(reference.id());
    }
     /**
     * Removes a certain DataArray from the list of References.
     *
     * @param string the id of the DataArray
     *
     * @return bool whether the operation succeeded.
     *
     */
    bool removeReference(const std::string &id) {
        return backend()->removeReference(id);
    }

    /**
     * Get referenced data arrays associated with this simple tag.
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
     * @return True if the feature exists, false otherwise.
     */
    bool hasFeature(const Feature &feature) const {
        if(feature == none) {
            throw std::runtime_error("SimpleTag::hasFeature: Empty DataArray entity given!");
        }
        return backend()->hasFeature(feature.id());
    }

    /**
     * Returns the number of features in this block.
     *
     * @return The number of features.
     */
    size_t featureCount() const {
        return backend()->featureCount();
    }

    /**
     * Retrieves a specific feature from the tag.
     *
     * @param id        The id of the feature.
     *
     * @return The feature with the specified id. If it 
     *         doesn't exist an exception will be thrown.
     */
    Feature getFeature(const std::string &id) const {
        return backend()->getFeature(id);
    }

    /**
     * Retrieves a specific feature from the tag.
     *
     * @param index        The index of the feature.
     *
     * @return The feature with the specified index. If it 
     *         doesn't exist an exception will be thrown.
     */
    Feature getFeature(size_t index) const {
        return backend()->getFeature(index);
    }

    /**
     * Get all features of this simple tag.
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
        if(data == none) {
            throw std::runtime_error("SimpleTag::createFeature: Empty DataArray entity given!");
        }
        return backend()->createFeature(data.id(), link_type);
    }

    /**
     * Create a new feature.
     *
     * @param data_array_id      The id of data array of this feature.
     * @param type      The link type of this feature.
     *
     * @return The created feature object.
     */
    Feature createFeature(const std::string &data_array_id, LinkType link_type) {
        return backend()->createFeature(data_array_id, link_type);
    }

    /**
     * Deletes a feature.
     *
     * @param id        The id of the feature to remove.
     *
     * @return True if the feature was removed, false otherwise.
     */
    bool deleteFeature(const std::string &id) {
        return backend()->deleteFeature(id);
    }

    /**
     * Deletes a feature.
     *
     * @param feature        The feature to remove.
     *
     * @return True if the feature was removed, false otherwise.
     */
    bool deleteFeature(const Feature &feature) {
        if (feature == none) {
            throw std::runtime_error("SimpleTag::deleteFeature: Empty Feature entity given!");
        }
        return backend()->deleteFeature(feature.id());
    }

    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------

    virtual SimpleTag &operator=(none_t) {
        nullify();
        return *this;
    }

    /**
     * Output operator
     */
    friend std::ostream& operator<<(std::ostream &out, const SimpleTag &ent) {
        out << "SimpleTag: {name = " << ent.name();
        out << ", type = " << ent.type();
        out << ", id = " << ent.id() << "}";
        return out;
    }
};


} // namespace nix

#endif // NIX_SIMPLE_TAG_H
