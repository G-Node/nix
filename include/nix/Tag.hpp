// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_TAG_H
#define NIX_TAG_H

#include <nix/base/EntityWithSources.hpp>
#include <nix/base/ITag.hpp>
#include <nix/DataArray.hpp>
#include <nix/Feature.hpp>
#include <nix/DataView.hpp>
#include <nix/Platform.hpp>
#include <nix/ObjectType.hpp>

#include <algorithm>


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
class NIXAPI Tag : public base::EntityWithSources<base::ITag> {

public:

    /**
     * @brief Constructor that creates an uninitialized Tag.
     *
     * Calling any method on an uninitialized tag will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a tag is initialized:
     *
     * ~~~
     * Tag e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    Tag()
        : EntityWithSources()
    {
    }

    /**
     * @brief Copy constructor.
     *
     * Copying of all NIX front facing objects like Tag is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The tag to copy.
     */
    Tag(const Tag &other)
        : EntityWithSources(other.impl())
    {
    }

    /**
     * @brief Constructor that creates a new tag from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    Tag(const std::shared_ptr<base::ITag> &p_impl)
        : EntityWithSources(p_impl)
    {
    }

    /**
     * @brief Constructor with move semantics that creates a new tag from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    Tag(std::shared_ptr<base::ITag> &&ptr)
        : EntityWithSources(std::move(ptr))
    {
    }

    //--------------------------------------------------
    // Element getters and setters
    //--------------------------------------------------

    /**
     * @brief Gets the units of the tag.
     *
     * The units are applied to all values for position and extent in order to calculate the
     * right position vectors in referenced data arrays.
     *
     * @return All units of the tag as a vector.
     */
    std::vector<std::string> units() const {
        return backend()->units();
    }

    /**
     * @brief Sets the units of a tag.
     *
     * @param units     All units as a vector.
     */
    void units(const std::vector<std::string> &units);

    /**
     * @brief Deleter for the units of a tag.
     *
     * @param t         None
     */
    void units(const boost::none_t t) {
        backend()->units(t);
    }

    /**
     * @brief Gets the position of a tag.
     *
     * The position is a vector that points into referenced DataArrays.
     *
     * @return The position vector.
     */
    std::vector<double> position() const {
        return backend()->position();
    }

    /**
     * @brief Sets the position of a tag.
     *
     * @param position  The position vector.
     */
    void position(const std::vector<double> &position) {
        backend()->position(position);
    }

    /**
     * @brief Gets the extent of a tag.
     *
     * Given a specified position vector, the extent vector defined the size
     * of a region of interest in the referenced DataArray entities.
     *
     * @return The extent of the tag.
     */
    std::vector<double> extent() const {
        return backend()->extent();
    }

    /**
     * @brief Sets the extent of a tag.
     *
     * @param extent    The extent vector.
     */
    void extent(const std::vector<double> &extent) {
        backend()->extent(extent);
    }

    /**
     * @brief Deleter for the extent of a tag.
     *
     * @param t         None
     */
    void extent(const boost::none_t t) {
        backend()->extent(t);
    }

    //--------------------------------------------------
    // Methods concerning references.
    //--------------------------------------------------

    /**
     * @brief Checks whether a DataArray is referenced by the tag.
     *
     * @param id        The id of the DataArray to check.
     *
     * @return True if the data array is referenced, false otherwise.
     */
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
    bool hasReference(const DataArray &reference) const;

    /**
     * @brief Gets the number of referenced DataArray entities of the tag.
     *
     * @return The number of referenced data arrays.
     */
    ndsize_t referenceCount() const {
        return backend()->referenceCount();
    }

    /**
     * @brief Gets a specific referenced DataArray from the tag.
     *
     * @param id        The id of the referenced DataArray.
     *
     * @return The referenced data array.
     */
    DataArray getReference(const std::string &id) const {
        return backend()->getReference(id);
    }

    /**
     * @brief Gets a referenced DataArray by its index.
     *
     * @param index     The index of the DataArray.
     *
     * @return The referenced data array.
     */
    DataArray getReference(size_t index) const;

    /**
     * @brief Add a DataArray to the list of referenced data of the tag.
     *
     * @param reference The DataArray to add.
     */
    void addReference(const DataArray &reference);

    /**
     * @brief Add a DataArray to the list of referenced data of the tag.
     *
     * @param id        The id of the DataArray to add.
     */
    void addReference(const std::string &id);

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
    bool removeReference(const DataArray &reference);

    /**
     * @brief Remove a DataArray from the list of referenced data of the tag.
     *
     * This method just removes the association between the data array and the
     * tag, the data array itself will not be removed from the file.
     *
     * @param id        The id of the DataArray to remove.
     *
     * @returns True if the DataArray was removed, false otherwise.
     */
    bool removeReference(const std::string &id) {
        return backend()->removeReference(id);
    }

    /**
     * @brief Get referenced data arrays associated with this tag.
     *
     * The parameter filter can be used to filter data arrays by various
     * criteria.
     *
     * @param filter    A filter function.
     *
     * @return A vector containing the matching data arrays.
     */
    std::vector<DataArray> references(const util::Filter<DataArray>::type &filter) const;

    /**
     * @brief Get all referenced data arrays associated with this tag.
     *
     * Always uses filter that accepts all sources.
     *
     * @return The filtered dimensions as a vector
     */
    std::vector<DataArray> references() const
    {
        return references(util::AcceptAll<DataArray>());
    }

    /**
     * @brief Sets all referenced DataArray entities.
     *
     * Previously referenced data arrays, that are not in the references vector
     * will be removed.
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
     * @brief Checks if a specific feature exists on the tag.
     *
     * @param id        The id of a feature.
     *
     * @return True if the feature exists, false otherwise.
     */
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
    bool hasFeature(const Feature &feature) const;

    /**
     * @brief Gets the number of features in this block.
     *
     * @return The number of features.
     */
    ndsize_t featureCount() const {
        return backend()->featureCount();
    }

    /**
     * @brief Retrieves a specific feature from the tag.
     *
     * @param id        The id of the feature.
     *
     * @return The feature with the specified id. If it doesn't exist
     *         an exception will be thrown.
     */
    Feature getFeature(const std::string &id) const;

    /**
     * @brief Retrieves a specific feature from the tag.
     *
     * @param index        The index of the feature.
     *
     * @return The feature with the specified index.
     */
    Feature getFeature(ndsize_t index) const;

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
    std::vector<Feature> features(const util::Filter<Feature>::type &filter = util::AcceptAll<Feature>()) const;

    /**
     * @brief Create a new feature.
     *
     * @param data      The data array of this feature.
     * @param link_type The link type of this feature.
     *
     * @return The created feature object.
     */
    Feature createFeature(const DataArray &data, LinkType link_type);

    /**
     * @brief Create a new feature.
     *
     * @param data_array_id     The id of the data array of this feature.
     * @param link_type         The link type of this feature.
     *
     * @return The created feature object.
     */
    Feature createFeature(const std::string &data_array_id, LinkType link_type) {
        return backend()->createFeature(data_array_id, link_type);
    }

    /**
     * @brief Deletes a feature from the tag.
     *
     * @param id        The id of the feature to remove.
     *
     * @return True if the feature was removed, false otherwise.
     */
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
    bool deleteFeature(const Feature &feature);

    //--------------------------------------------------
    // Methods for data retrieval
    //--------------------------------------------------

    /**
     * @brief Returns the data associated with a certain reference.
     *
     * @param reference_index The index of the reference of which
     *                        the data should be returned.
     *
     * @return the data
     */
    DataView retrieveData(size_t reference_index) const;

    /**
     * @brief Returns the data associated with a certain reference.
     *
     * @param name_or_id      Name or id of the referenced dataArray.
     *
     * @return the data
     */
    DataView retrieveData(const std::string &name_or_id) const; 
    
    /**
     * @brief Returns the data stored in the selected Feature.
     *
     * @param feature_index   The index of the requested feature.
     *
     * @return The data stored in the Feature.
     *
     */
    DataView retrieveFeatureData(size_t feature_index) const;

    /**
     * @brief Returns the data stored in the selected Feature.
     *
     * @param name_or_id     The name or id of the feature or the DataArray stored
     *                       in the feature.
     *
     * @return The data stored in the Feature.
     *
     */
    DataView retrieveFeatureData(const std::string &name_or_id) const;

    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------

    /**
     * @brief Assignment operator for none.
     */
    Tag &operator=(const none_t &t) {
        ImplContainer::operator=(t);
        return *this;
    }

    /**
     * @brief Output operator
     */
    NIXAPI friend std::ostream& operator<<(std::ostream &out, const Tag &ent);

};


template<>
struct objectToType<nix::Tag> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::Tag;
    typedef nix::base::ITag backendType;
};


} // namespace nix

#endif // NIX_TAG_H
