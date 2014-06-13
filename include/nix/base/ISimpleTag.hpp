// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_SIMPLE_TAG_H
#define NIX_I_SIMPLE_TAG_H

#include <string>
#include <vector>

#include <nix/base/IEntityWithSources.hpp>

namespace nix {

class DataArray;
class Feature;

enum class LinkType;


namespace base {

/**
 * @brief Interface for implementations of the SimpleTag entity.
 *
 * See {@link nix::SimpleTag} for a more detailed description.
 */
class NIXAPI ISimpleTag : virtual public base::IEntityWithSources {

public:

    /**
     * @brief Gets the units of the tag.
     *
     * The units are applied to all values for position and extent in order to calculate the
     * right position vectors in referenced data arrays.
     *
     * @return All units of the tag as a vector.
     */
    virtual std::vector<std::string> units() const = 0;

    /**
     * @brief Sets the units of a tag.
     *
     * @param units     All units as a vector.
     */
    virtual void units(std::vector<std::string> &units) = 0;

    /**
     * @brief Deleter for the units of a tag.
     *
     * @param t         None
     */
    virtual void units(const none_t t) = 0;

    /**
     * @brief Gets the position of a tag.
     *
     * The position is a vector that points into referenced DataArrays.
     *
     * @return The position vector.
     */
    virtual std::vector<double> position() const = 0;

    /**
     * @brief Sets the position of a tag.
     *
     * @param position  The position vector.
     */
    virtual void position(const std::vector<double> &position) = 0;

    /**
     * @brief Deleter for the position of a tag.
     *
     * @param t         None
     */
    virtual void position(const boost::none_t t) = 0;

    /**
     * @brief Gets the extent of a tag.
     *
     * Given a specified position vector, the extent vector defined the size
     * of a region of interest in the referenced DataArray entities.
     *
     * @return The extent of the tag.
     */
    virtual std::vector<double> extent() const = 0;

    /**
     * @brief Sets the extent of a tag.
     *
     * @param extent    The extent vector.
     */
    virtual void extent(const std::vector<double> &extent) = 0;

    /**
     * @brief Deleter for the extent of a tag.
     *
     * @param t         None
     */
    virtual void extent(const none_t t) = 0;

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
    virtual bool hasReference(const std::string &id) const = 0;

    /**
     * @brief Gets the number of referenced DataArray entities of the tag.
     *
     * @return The number of referenced data arrays.
     */
    virtual size_t referenceCount() const = 0;

    /**
     * @brief Gets a specific referenced DataArray from the tag.
     *
     * @param id        The id of the referenced DataArray.
     *
     * @return The referenced data array.
     */
    virtual DataArray getReference(const std::string &id) const = 0;

    /**
     * @brief Gets a referenced DataArray by its index.
     *
     * @param index     The index of the DataArray.
     *
     * @return The referenced data array.
     */
    virtual DataArray getReference(size_t index) const = 0;

    /**
     * @brief Add a DataArray to the list of referenced data of the tag.
     *
     * @param id        The id of the DataArray to add.
     */
    virtual void addReference(const std::string &id) = 0;

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
    virtual bool removeReference(const std::string &id) = 0;

    /**
     * @brief Sets all referenced DataArray entities.
     *
     * Previously referenced data arrays, that are not in the references vector
     * will be removed.
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
     * @brief Gets the number of features in this block.
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
     * @param data_array_id     The id of the data array of this feature.
     * @param link_type         The link type of this feature.
     *
     * @return The created feature object.
     */
    virtual Feature createFeature(const std::string &data_array_id, LinkType link_type) = 0;

    /**
     * @brief Deletes a feature from the tag.
     *
     * @param id        The id of the feature to remove.
     *
     * @return True if the feature was removed, false otherwise.
     */
    virtual bool deleteFeature(const std::string &id) = 0;


    /**
     * @brief Destructor
     */
    virtual ~ISimpleTag() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_SIMPLE_TAG_H
