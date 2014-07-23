// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_DATA_TAG_H
#define NIX_DATA_TAG_H

#include <algorithm>

#include <nix/base/IDataTag.hpp>
#include <nix/base/EntityWithSources.hpp>
#include <nix/Feature.hpp>
#include <nix/Platform.hpp>

namespace nix {

/**
 * @brief A tag class that can be used to tag multiple positions or regions in data.
 *
 * Besides the {@link nix::DataArray} the tag entities can be considered as the other
 * core entities of the data model.
 * They are meant to attach annotations directly to the data and to establish meaningful
 * links between different kinds of stored data.
 * Most importantly tags allow the definition of points or regions of interest in data
 * that is stored in other {@link nix::DataArray} entities. The data array entities the
 * tag applies to are defined by its property {@link references}.
 *
 * Further the referenced data is defined by an origin vector called {@link positions}
 * and an optional {@link extents} vector that defines its size.
 * Therefore position and extent of a tag, together with the references field
 * defines a group of points or regions of interest collected from a subset of all
 * available {@link nix::DataArray} entities.
 *
 * Further tags have a field called {@link features} which makes it possible to associate
 * other data with the tag.  Semantically a feature of a tag is some additional data that
 * contains additional information about the points of hyperslabs defined by the tag.
 * This could be for example data that represents a stimulus (e.g. an image or a
 * signal) that was applied in a certain interval during the recording.
 */
class NIXAPI DataTag : virtual public base::IDataTag, public base::EntityWithSources<base::IDataTag> {

public:

    /**
     *  @brief Constructor that creates an uninitialized DataTag.
     *
     * Calling any method on an uninitialized tag will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a tag is initialized:
     *
     * ~~~
     * DataTag e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    DataTag()
        : EntityWithSources()
    {}

    /**
     * @brief Copy constructor.
     *
     * Copying of all NIX front facing objects like DataTag is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The tag to copy.
     */
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

    DataArray positions() const {
        return backend()->positions();
    }

    void positions(const std::string &id) {
        backend()->positions(id);
    }

    /**
     * @brief Setter for the positions of the tag.
     *
     * @param positions   The DataArray containing the positions of the tag.
     */
    void positions(const DataArray &positions) {
        if(positions == none) {
            throw std::runtime_error("Empty positions entity (DataArray) given");
        }
        else {
            backend()->positions(positions.id());
        }
    }

    bool hasPositions() const {
        return backend()->hasPositions();
    }

    DataArray extents() const {
        return backend()->extents();
    }

    /**
     * @brief Sets the extents DataArray of the tag.
     *
     * @param extents      The DataArray containing the extents of the tag.
     */
    void extents(const DataArray &extents) {
        if(extents == none) {
            backend()->extents(none);
        }
        else {
            backend()->extents(extents.id());
        }
    }

    void extents(const std::string &id) {
        backend()->extents(id);
    }

    void extents(const boost::none_t t) {
        backend()->extents(t);
    }

    std::vector<std::string> units() const {
        return backend()->units();
    }

    void units(const std::vector<std::string> &units) {
        std::vector<std::string> sanitized;
        sanitized.reserve(units.size());
        std::transform(begin(units), end(units), std::back_inserter(sanitized), [](const std::string &x) {
                std::string unit = util::unitSanitizer(x);
                if (unit.length() > 0 && (unit != "none" && !(util::isSIUnit(unit)))) {
                    std::string msg = "Unit " + unit +" is not a SI unit. Note: so far only atomic SI units are supported.";
                    throw InvalidUnit(msg, "DataTag::units(vector<string> &units)");
                }
                return unit;
            });
        backend()->units(sanitized);
    }

    void units(const boost::none_t t) {
        backend()->units(t);
    }

    //--------------------------------------------------
    // Methods concerning references.
    //--------------------------------------------------

    /**
     * @brief Checks if the specified DataArray is referenced by the tag.
     *
     * @param reference     The data array to check.
     *
     * @return True if the data array is referenced, false otherwise.
     */
    bool hasReference(const DataArray &reference) const {
        if (reference == none) {
            throw std::runtime_error("DataTag::hasReference: Empty DataArray entity given!");
        }
        return backend()->hasReference(reference.id());
    }

    bool hasReference(const std::string &id) const {
        return backend()->hasReference(id);
    }

    size_t referenceCount() const {
        return backend()->referenceCount();
    }

    DataArray getReference(const std::string &id) const {
        return backend()->getReference(id);
    }

    DataArray getReference(size_t index) const {
        return backend()->getReference(index);
    }

    void addReference(const std::string &id) {
        backend()->addReference(id);
    }

    /**
     * @brief Adds a new DataArray to the list of referenced data.
     *
     * @param reference The DataArray that should be referenced.
     */
    void addReference(const DataArray &reference) {
        if (reference == none) {
            throw std::runtime_error("DataTag::addReference: Empty DataArray entity given!");
        }
        backend()->addReference(reference.id());
    }

    bool removeReference(const std::string &id) {
        return backend()->removeReference(id);
    }

    /**
     * @brief Remove a DataArray from the list of referenced data.
     *
     * This function only removes the association between the tag and the data array,
     * but does not delete the data array itself.
     *
     * @param reference The DataArray to remove.
     *
     * @return True if the data array was removed, false otherwise.
     */
    bool removeReference(const DataArray &reference) {
        if (reference == none) {
            throw std::runtime_error("DataTag::removeReference: Empty DataArray reference given!");
        }
        return backend()->removeReference(reference.id());
    }

    /**
     * @brief Get all referenced data arrays associated with the tag.
     *
     * The parameter filter can be used to filter data arrays by various
     * criteria.
     *
     * @param filter       A filter function.
     *
     * @return A vector containing all filtered DataArray entities.
     */
    std::vector<DataArray> references(util::Filter<DataArray>::type filter) const
    {
        auto f = [this] (size_t i) { return getReference(i); };
        return getEntities<DataArray>(f,
                                      referenceCount(),
                                      filter);
    }
    /**
     * @brief Get all referenced data arrays associated with the tag.
     *
     * Always uses filter that accepts all sources.
     *
     * @return A vector containing all filtered DataArray entities.
     */
    std::vector<DataArray> references() const
    {
        return references(util::AcceptAll<DataArray>());
    }

    void references(const std::vector<DataArray> &references) {
        backend()->references(references);
    }

    //--------------------------------------------------
    // Methods concerning features.
    //--------------------------------------------------

    bool hasFeature(const std::string &id) const {
        return backend()->hasFeature(id);
    }

    /**
     * @brief Checks if a specific feature exists on the tag.
     *
     * @param feature        The Feature to check.
     *
     * @return True if the feature exists, false otherwise.
     */
    bool hasFeature(const Feature &feature) const {
        if (feature == none) {
            throw std::runtime_error("DataTag::hasFeature: Empty feature given!");
        }
        return backend()->hasFeature(feature.id());
    }

    size_t featureCount() const {
        return backend()->featureCount();
    }

    Feature getFeature(const std::string &id) const {
        return backend()->getFeature(id);
    }

    Feature getFeature(size_t index) const {
        return backend()->getFeature(index);
    }

    /**
     * @brief Get all Feature entities contained in the tag.
     *
     * The parameter filter can be used to filter features by various
     * criteria. By default a filter is used that accepts all features.
     *
     * @param filter       A filter function.
     *
     * @return A vector containing all filtered Feature entities.
     */
    std::vector<Feature> features(util::Filter<Feature>::type filter = util::AcceptAll<Feature>()) const
    {
        auto f = [this] (size_t i) { return getFeature(i); };
        return getEntities<Feature>(f,
                                    featureCount(),
                                    filter);
    }

    /**
     * @brief Create a new feature.
     *
     * @param data      The data array that is part of the new feature.
     * @param link_type The link type of this feature.
     *
     * @return The created feature object.
     */
    Feature createFeature(const DataArray &data, LinkType link_type) {
        return backend()->createFeature(data.id(), link_type);
    }

    Feature createFeature(const std::string &data_array_id, LinkType link_type) {
        return backend()->createFeature(data_array_id, link_type);
    }

    bool deleteFeature(const std::string &id) {
        return backend()->deleteFeature(id);
    }

    /**
     * @brief Delete a feature from the tag.
     *
     * @param feature   The feature to remove.
     *
     * @return True if the feature was removed, false otherwise.
     */
    bool deleteFeature(const Feature &feature) {
        if (feature == none) {
            throw std::runtime_error("DataTag::deleteFeature: Empty Feature entity given!");
        }
        return backend()->deleteFeature(feature.id());
    }

    //------------------------------------------------------
    // Operators and other functions
    //------------------------------------------------------

    /**
     * @brief Assign none operator.
     */
    virtual DataTag &operator=(none_t) {
        nullify();
        return *this;
    }

    /**
     * @brief Output operator
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
