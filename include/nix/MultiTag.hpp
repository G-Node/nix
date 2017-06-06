// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_MULTI_TAG_H
#define NIX_MULTI_TAG_H

#include <nix/base/IMultiTag.hpp>
#include <nix/base/EntityWithSources.hpp>
#include <nix/Feature.hpp>
#include <nix/Platform.hpp>
#include <nix/DataView.hpp>
#include <nix/ObjectType.hpp>

#include <algorithm>
#include <memory>
#include <vector>
#include <string>

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
class NIXAPI MultiTag : public base::EntityWithSources<base::IMultiTag> {

public:

    /**
     *  @brief Constructor that creates an uninitialized MultiTag.
     *
     * Calling any method on an uninitialized tag will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a tag is initialized:
     *
     * ~~~
     * MultiTag e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    MultiTag()
        : EntityWithSources()
    {}

    /**
     * @brief Copy constructor.
     *
     * Copying of all NIX front facing objects like MultiTag is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The tag to copy.
     */
    MultiTag(const MultiTag &other)
        : EntityWithSources(other.impl())
    {
    }

    MultiTag(const std::shared_ptr<base::IMultiTag> &p_impl)
        : EntityWithSources(p_impl)
    {
    }

    MultiTag(std::shared_ptr<base::IMultiTag> &&ptr)
        : EntityWithSources(std::move(ptr))
    {
    }

    //--------------------------------------------------
    // Positions and extents
    //--------------------------------------------------

    /**
     * @brief Getter for the positions of a tag.
     *
     * The positions of a multi tag are defined in a DataArray. This array has to define a set of
     * origin vectors, each defining a point inside the referenced data or the beginning of a
     * region of interest.
     *
     * @return The DataArray defining the positions of the tag.
     */
    DataArray positions() const {
        return backend()->positions();
    }

    /**
     * @brief Setter for the positions of the tag.
     *
     * @param name_or_id      Name or id of the DataArray that defines the positions for this tag.
     */
    void positions(const std::string &name_or_id);

    /**
     * @brief Setter for the positions of the tag.
     *
     * @param positions   The DataArray containing the positions of the tag.
     */
    void positions(const DataArray &positions);

    /**
     * @brief Determine whether this DataArray contains positions.
     *
     * @return True if the DataArray has positions, false otherwise.
     */
    bool hasPositions() const {
        return backend()->hasPositions();
    }

    /**
     * @brief Getter for the extents of a tag.
     *
     * The extents of a multi tag are defined in an associated DataArray. This array has to define a set of
     * extent vectors, each defining the size of the corresponding region of interest.
     *
     * @return The DataArray defining the extents of the tag.
     */
    DataArray extents() const {
        return backend()->extents();
    }

    /**
     * @brief Sets the extents DataArray of the tag.
     *
     * @param extents      The DataArray containing the extents of the tag.
     */
    void extents(const DataArray &extents);

    /**
     * @brief Setter for the positions of the tag.
     *
     * @param name_or_id        Name or id of the DataArray that defines the extents of the tag.
     */
    void extents(const std::string &name_or_id);

    /**
     * @brief Deleter for the reference to the extents DataArray.
     *
     * This function only removes the association between the tag and the data array,
     * but does not delete the data array itself.
     *
     * @param t         None
     */
    void extents(const boost::none_t t) {
        backend()->extents(t);
    }

    /**
     * @brief Gets for the units of the tag.
     *
     * The units are applied to all values for position and extent in order to calculate the right
     * position vectors in referenced data arrays.
     *
     * @return All units of the tag as a vector.
     */
    std::vector<std::string> units() const {
        return backend()->units();
    }

    /**
     * @brief Setter for the units of a tag.
     *
     * All previously defined units will be replaced by the ones passed
     * to the units parameter.
     *
     * @param units     All units as a vector.
     */
    void units(const std::vector<std::string> &units);

    /**
     * @brief Deleter for the units of a tag.
     *
     * Removes all units from this tag.
     *
     * @param t         None
     */
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
    bool hasReference(const DataArray &reference) const;

    /**
     * @brief Checks if the specified DataArray is referenced in this tag.
     *
     * @param name_or_id        Name or id of the data array to check.
     *
     * @return True if a data array with the given id is referenced, false otherwise.
     */
    bool hasReference(const std::string &name_or_id) const {
        return backend()->hasReference(name_or_id);
    }

    /**
     * @brief Get the number of DataArray entities that are referenced by the tag.
     *
     * @return The number of referenced data arrays.
     */
    ndsize_t referenceCount() const {
        return backend()->referenceCount();
    }

    /**
     * @brief Get a referenced DataArray by its name or id.
     *
     * @param name_or_id        Name or id of the data array.
     *
     * @return The referenced data array.
     */
    DataArray getReference(const std::string &name_or_id) const {
        return backend()->getReference(name_or_id);
    }

    /**
     * @brief Get a referenced DataArray by its index.
     *
     * @param index     The index of the data array.
     *
     * @return The referenced data array.
     */
    DataArray getReference(size_t index) const;

    /**
     * @brief Add a new DataArray to the list of referenced data.
     *
     * @param name_or_id        Name or id of the data array.
     */
    void addReference(const std::string &name_or_id) {
        backend()->addReference(name_or_id);
    }

    /**
     * @brief Adds a new DataArray to the list of referenced data.
     *
     * @param reference The DataArray that should be referenced.
     */
    void addReference(const DataArray &reference);

    /**
     * @brief Remove a DataArray from the list of referenced data.
     *
     * This function only removes the association between the tag and the data array,
     * but does not delete the data array itself.
     *
     * @param name_or_id        Name or id of the data array.
     *
     * @return True if the data array was removed, false otherwise.
     */
    bool removeReference(const std::string &name_or_id) {
        return backend()->removeReference(name_or_id);
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
    bool removeReference(const DataArray &reference);

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
    std::vector<DataArray> references(const util::Filter<DataArray>::type &filter) const;

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

    /**
     * @brief Setter for all referenced DataArrays.
     *
     * Previously referenced data will be replaced.
     * removed.
     *
     * @param references    All referenced arrays.
     */
    void references(const std::vector<DataArray> &references) {
        backend()->references(references);
    }

    /**
     * @brief Retrieves the data slice tagged by a certain position and extent
     *        of a certain reference.  
     *
     * @param position_index the index of the requested position.
     * @param reference_index the index of the requested reference.
     *
     * @return the requested data.
     */
    DataView retrieveData(size_t position_index, size_t reference_index) const;

     /**
     * @brief Retrieves the data slice tagged by a certain position and extent
     *        of a certain reference.  
     *
     * @param position_index      The index of the requested position.
     * @param name_or_id          The name or id of the requested DataArray.
     *
     * @return the requested data.
     */
    DataView retrieveData(size_t position_index, const std::string &name_or_id) const;

    //--------------------------------------------------
    // Methods concerning features.
    //--------------------------------------------------

    /**
     * @brief Checks if a specific feature exists on the tag.
     *
     * @param name_or_id        Name or id of a feature.
     *
     * @return True if the feature exists, false otherwise.
     */
    bool hasFeature(const std::string &name_or_id) const {
        return backend()->hasFeature(name_or_id);
    }

    /**
     * @brief Checks if a specific feature exists on the tag.
     *
     * @param feature        The Feature to check.
     *
     * @return True if the feature exists, false otherwise.
     */
    bool hasFeature(const Feature &feature) const;

    /**
     * @brief Returns the number of features in this block.
     *
     * @return The number of features.
     */
    ndsize_t featureCount() const {
        return backend()->featureCount();
    }

    /**
     * @brief Retrieves a specific feature from the tag.
     *
     * @param name_or_id        Name or id of the feature.
     *
     * @return The feature with the specified name or id. If it doesn't exist
     *         an exception will be thrown.
     */
    Feature getFeature(const std::string &name_or_id) const {
        return backend()->getFeature(name_or_id);
    }

    /**
     * @brief Retrieves a specific feature from the tag.
     *
     * @param index        The index of the feature.
     *
     * @return The feature with the specified index.
     */
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
    std::vector<Feature> features(const util::Filter<Feature>::type &filter = util::AcceptAll<Feature>()) const;

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

    /**
     * @brief Create a new feature.
     *
     * @param name_or_id        Name or id of the data array that is part of the new feature.
     * @param link_type         The link type of this feature.
     *
     * @return The created feature object.
     */
    Feature createFeature(const std::string &name_or_id, LinkType link_type) {
        return backend()->createFeature(name_or_id, link_type);
    }

    /**
     * @brief Delete a feature from the tag.
     *
     * @param name_or_id        Name or id of the feature to remove.
     *
     * @return True if the feature was removed, false otherwise.
     */
    bool deleteFeature(const std::string &name_or_id) {
        return backend()->deleteFeature(name_or_id);
    }

    /**
     * @brief Delete a feature from the tag.
     *
     * @param feature   The feature to remove.
     *
     * @return True if the feature was removed, false otherwise.
     */
    bool deleteFeature(const Feature &feature);

    /**
     * @brief Retrieves the data stored in a feature related to a certain
     *        position of this tag.
     *
     * @param position_index The index of the requested position
     * @param feature_index The index of the selected feature
     *
     * @return The data
     */
    DataView retrieveFeatureData(size_t position_index, size_t feature_index) const;

    /**
     * @brief Retrieves the data stored in a feature related to a certain
     *        position of this tag.
     *
     * @param position_index The index of the requested position.
     * @param name_or_id     The name or id of the feature that is requested.
     *
     * @return The data
     */
    DataView retrieveFeatureData(size_t position_index, const std::string &name_or_id) const;

    //------------------------------------------------------
    // Operators and other functions
    //------------------------------------------------------

    /**
     * @brief Assignment operator for none.
     */
    MultiTag &operator=(const none_t &t) {
        ImplContainer::operator=(t);
        return *this;
    }

    /**
     * @brief Output operator
     */
    NIXAPI friend std::ostream& operator<<(std::ostream &out, const MultiTag &ent);

};

template<>
struct objectToType<nix::MultiTag> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::MultiTag;
    typedef nix::base::IMultiTag backendType;
};

} // namespace nix

#endif // NIX_MULTI_TAG_H
