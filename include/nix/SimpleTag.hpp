// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_SIMPLE_TAG_H
#define NIX_SIMPLE_TAG_H

#include <algorithm>

#include <nix/base/EntityWithSources.hpp>
#include <nix/base/ISimpleTag.hpp>
#include <nix/Section.hpp>
#include <nix/DataArray.hpp>
#include <nix/Feature.hpp>

#include <nix/Platform.hpp>

namespace nix {


/**
 * @brief A tag class that defines a single point or region of interest.
 *
 * Besides the {@link nix::DataArray} the tag entities can be considered as the other
 * core entities of the data model.
 * They are meant to attach annotations directly to the data and to establish meaningful
 * links between different kinds of stored data.
 * Most importantly tags allow the definition of points or regions of interest in data
 * that is stored in other {@link nix::DataArray} entities. The data array entities the
 * tag applies to are defined by its property {@link references}.
 *
 * Further the referenced data is defined by an origin vector called {@link position}
 * and an optional {@link extent} vector that defines its size.
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
class NIXAPI SimpleTag : virtual public base::ISimpleTag, public base::EntityWithSources<base::ISimpleTag> {

public:

    /**
     * @brief Constructor that creates an uninitialized SimpleTag.
     *
     * Calling any method on an uninitialized tag will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a tag is initialized:
     *
     * ~~~
     * SimpleTag e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    SimpleTag()
        : EntityWithSources()
    {
    }

    /**
     * @brief Copy constructor.
     *
     * Copying of all NIX front facing objects like SimpleTag is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The tag to copy.
     */
    SimpleTag(const SimpleTag &other)
        : EntityWithSources(other.impl())
    {
    }

    /**
     * @brief Constructor that creates a new tag from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    SimpleTag(const std::shared_ptr<base::ISimpleTag> &p_impl)
        : EntityWithSources(p_impl)
    {
    }

    /**
     * @brief Constructor with move semantics that creates a new tag from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    SimpleTag(std::shared_ptr<base::ISimpleTag> &&ptr)
        : EntityWithSources(std::move(ptr))
    {
    }

    //--------------------------------------------------
    // Element getters and setters
    //--------------------------------------------------

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
                    throw InvalidUnit(msg, "SimpleTag::units(vector<string> &units)");
                }
                return unit;
            });
        backend()->units(sanitized);
    }

    void units(const boost::none_t t) {
        backend()->units(t);
    }

    std::vector<double> position() const {
        return backend()->position();
    }

    void position(const std::vector<double> &position) {
        backend()->position(position);
    }

    void position(const boost::none_t t) {
        backend()->position(t);
    }

    std::vector<double> extent() const {
        return backend()->extent();
    }

    void extent(const std::vector<double> &extent) {
        backend()->extent(extent);
    }

    void extent(const boost::none_t t) {
        backend()->extent(t);
    }

    //--------------------------------------------------
    // Methods concerning references.
    //--------------------------------------------------

    bool hasReference(const std::string &id) const {
        return backend()->hasReference(id);
    }

    /**
     * @brief Checks whether a DataArray is referenced by the tag.
     *
     * @param reference The DataArray to check.
     *
     * @return True if the data array is referenced, false otherwise.
     */
    bool hasReference(const DataArray &reference) const {
        if (reference == none) {
            throw std::runtime_error("SimpleTag::hasReference: Emty DataArray entity given!");
        }
        return backend()->hasReference(reference.id());
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

    /**
     * @brief Add a DataArray to the list of referenced data of the tag.
     *
     * @param reference The DataArray to add.
     */
    void addReference(const DataArray &reference) {
        if (reference == none) {
            throw std::runtime_error("SimpleTag::addReference: Empty DataArray entity given!");
        }
        backend()->addReference(reference.id());
    }

    void addReference(const std::string &id) {
        backend()->addReference(id);
    }

    /**
     * @brief Remove a DataArray from the list of referenced data of the tag.
     *
     * This method just removes the association between the data array and the
     * tag, the data array itself will not be removed from the file.
     *
     * @param reference The DataArray to remove.
     *
     * @returns True if the DataArray was removed, false otherwise.
     */
    bool removeReference(const DataArray &reference) {
        if(reference == none) {
            throw std::runtime_error("SimpleTag::removeReference: Empty DataArray entity given!");
        }
        return backend()->removeReference(reference.id());
    }

    bool removeReference(const std::string &id) {
        return backend()->removeReference(id);
    }

    /**
     * @brief Get referenced data arrays associated with this simple tag.
     *
     * The parameter filter can be used to filter data arrays by various
     * criteria.
     *
     * @param filter    A filter function.
     *
     * @return A vector containing the matching data arrays.
     */
    std::vector<DataArray> references(util::Filter<DataArray>::type filter) const
    {
        auto f = [this] (size_t i) { return getReference(i); };
        return getEntities<DataArray>(f,
                                      referenceCount(),
                                      filter);
    }
    /**
     * @brief Get all referenced data arrays associated with this simple 
     * tag.
     *
     * Always uses filter that accepts all sources.
     *
     * @return The filtered dimensions as a vector
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
     * @param feature   The Feature to check.
     *
     * @return True if the feature exists, false otherwise.
     */
    bool hasFeature(const Feature &feature) const {
        if(feature == none) {
            throw std::runtime_error("SimpleTag::hasFeature: Empty DataArray entity given!");
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
     * @brief Get all Features of this tag.
     *
     * The parameter filter can be used to filter features by various
     * criteria. By default a filter is used that accepts all features.
     *
     * @param filter    A filter function.
     *
     * @return A vector containing the matching features.
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
     * @param data      The data array of this feature.
     * @param link_type The link type of this feature.
     *
     * @return The created feature object.
     */
    Feature createFeature(const DataArray &data, LinkType link_type) {
        if(data == none) {
            throw std::runtime_error("SimpleTag::createFeature: Empty DataArray entity given!");
        }
        return backend()->createFeature(data.id(), link_type);
    }

    Feature createFeature(const std::string &data_array_id, LinkType link_type) {
        return backend()->createFeature(data_array_id, link_type);
    }

    bool deleteFeature(const std::string &id) {
        return backend()->deleteFeature(id);
    }

    /**
     * @brief Deletes a feature from the tag.
     *
     * @param feature   The feature to remove.
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

    /**
     * @brief Assignment operator to none.
     */
    virtual SimpleTag &operator=(none_t) {
        nullify();
        return *this;
    }

    /**
     * @brief Output operator
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
