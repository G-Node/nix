// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_DATA_TAG_H
#define NIX_I_DATA_TAG_H

#include <vector>
#include <string>

#include <nix/base/IEntityWithSources.hpp>
#include <nix/Feature.hpp>

namespace nix {

class Feature;
class DataArray;


namespace base {

/**
 * @brief Interface for implementations of the DataTag entity.
 *
 * See {@link nix::DataTag} for a more detailed description.
 */
class NIXAPI IDataTag : virtual public base::IEntityWithSources {

public:

    //--------------------------------------------------
    // Element getters and setters
    //--------------------------------------------------

    /**
     * @brief Getter for the positions of a tag.
     *
     * The positions of a data tag are defined in a DataArray. This array has to define a set of
     * origin vectors, each defining a point inside the referenced data or the beginning of a
     * region of interest.
     *
     * @return The DataArray defining the positions of the tag.
     */
    virtual DataArray positions() const = 0;

    /**
     * @brief Setter for the positions of the tag.
     *
     * @param id      The id of the DataArray that defines the positions for this tag.
     */
    virtual void positions(const std::string &id) = 0;

    /**
     * @brief Determine whether this DataArray contains positions.
     *
     * @return True if the DataArray has positions, false otherwise.
     */
    virtual bool hasPositions() const = 0;

    /**
     * @brief Getter for the extents of a tag.
     *
     * The extents of a data tag are defined in an associated DataArray. This array has to define a set of
     * extent vectors, each defining the size of the corresponding region of interest.
     *
     * @return The DataArray defining the extents of the tag.
     */
    virtual DataArray extents() const = 0;

    /**
     * @brief Setter for the positions of the tag.
     *
     * @param id        The id of the DataArray that defines the extents of the tag.
     */
    virtual void extents(const std::string &id) = 0;

    /**
     * @brief Deleter for the reference to the extents DataArray.
     *
     * This function only removes the association between the tag and the data array,
     * but does not delete the data array itself.
     *
     * @param t         None
     */
    virtual void extents(const none_t t) = 0;

    /**
     * @brief Gets for the units of the tag.
     *
     * The units are applied to all values for position and extent in order to calculate the right
     * position vectors in referenced data arrays.
     *
     * @return All units of the tag as a vector.
     */
    virtual std::vector<std::string> units() const = 0;

    /**
     * @brief Setter for the units of a tag.
     *
     * All previously defined units will be replaced by the ones passed
     * to the units parameter.
     *
     * @param units     All units as a vector.
     */
    virtual void units(const std::vector<std::string> &units) = 0;

    /**
     * @brief Deleter for the units of a tag.
     *
     * Removes all units from this tag.
     *
     * @param t         None
     */
    virtual void units(const none_t t) = 0;

    //--------------------------------------------------
    // Methods concerning references.
    // TODO implement when done with the DataArray class.
    //--------------------------------------------------

    /**
     * @brief Checks if the specified DataArray is referenced in this tag.
     *
     * @param id        The id of the data array to check.
     *
     * @return True if a data array with the given id is referenced, false otherwise.
     */
    virtual bool hasReference(const std::string &id) const = 0;

    /**
     * @brief Get the number of DataArray entities that are referenced by the tag.
     *
     * @return The number of referenced data arrays.
     */
    virtual size_t referenceCount() const = 0;

    /**
     * @brief Get a referenced DataArray by its id.
     *
     * @param id        The id of the data array.
     *
     * @return The referenced data array.
     */
    virtual DataArray getReference(const std::string &id) const = 0;

    /**
     * @brief Get a referenced DataArray by its index.
     *
     * @param index     The index of the data array.
     *
     * @return The referenced data array.
     */
    virtual DataArray getReference(size_t index) const = 0;

    /**
     * @brief Add a new DataArray to the list of referenced data.
     *
     * @param id        The id of the data array.
     */
    virtual void addReference(const std::string &id) = 0;

    /**
     * @brief Remove a DataArray from the list of referenced data.
     *
     * This function only removes the association between the tag and the data array,
     * but does not delete the data array itself.
     *
     * @param id        the id of the data array.
     *
     * @return True if the data array was removed, false otherwise.
     */
    virtual bool removeReference(const std::string &id) = 0;

    /**
     * @brief Setter for all referenced DataArrays.
     *
     * Previously referenced data will be replaced.
     * removed.
     *
     * @param references    All referenced arrays.
     */
    virtual void references(const std::vector<DataArray> &references) = 0;

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
    virtual bool hasFeature(const std::string &id) const = 0;

    /**
     * @brief Returns the number of features in this block.
     *
     * @return The number of features.
     */
    virtual size_t featureCount() const = 0;

    /**
     * @brief Retrieves a specific feature from the tag.
     *
     * @param id        The id of the feature.
     *
     * @return The feature with the specified id. If it doesn't exist
     *         an exception will be thrown.
     */
    virtual Feature getFeature(const std::string &id) const = 0;

    /**
     * @brief Retrieves a specific feature from the tag.
     *
     * @param index        The index of the feature.
     *
     * @return The feature with the specified index.
     */
    virtual Feature getFeature(size_t index) const = 0;

    /**
     * @brief Create a new feature.
     *
     * @param id        The id of the data array that is part of the new feature.
     * @param link_type The link type of this feature.
     *
     * @return The created feature object.
     */
    virtual Feature createFeature(const std::string &id, LinkType link_type) = 0;

    /**
     * @brief Delete a feature from the tag.
     *
     * @param id        The id of the feature to remove.
     *
     * @return True if the feature was removed, false otherwise.
     */
    virtual bool deleteFeature(const std::string &id) = 0;

    /**
     * @brief Destructor
     */
    virtual ~IDataTag() {}

};

} // namespace base
} // namespace nix

#endif // NIX_I_DATA_TAG_H
