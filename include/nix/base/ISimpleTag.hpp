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
class Representation;

enum class LinkType;


namespace base {


class ISimpleTag : virtual public base::IEntityWithSources {

public:

    /**
     * Getter for the units of the tag. The units are applied to all values for position
     * and extent in order to calculate the right position vectors in referenced data arrays.
     *
     * @return All units of the tag as a vector.
     */
    virtual std::vector<std::string> units() const = 0;

    /**
     * Setter for the units of a tag.
     *
     * @param units     All units as a vector.
     */
    virtual void units(std::vector<std::string> &units) = 0;

    /**
     * Getter for the position of a tag. The position is a vector that
     * points into referenced DataArrays.
     *
     * @return The position vector.
     */
    virtual std::vector<double> position() const = 0;

    /**
     * Setter for the position of a tag.
     *
     * @param position    The position vector.
     */
    virtual void position(const std::vector<double> &position) = 0;

    /**
     * Getter for the extent of a tag. Given a specified position
     * vector, the extent vector defined the size of a region of
     * interest in the referenced DataArrays.
     *
     * @return The extent of the tag.
     */
    virtual std::vector<double> extent() const = 0;

    /**
     * Setter for the extent of a tag.
     *
     * @param extent      The extent vector.
     */
    virtual void extent(const std::vector<double> &extent) = 0;

    //--------------------------------------------------
    // Methods concerning references.
    // TODO implement when done with the DataArray class.
    // TODO add hasXy getXy addXy and removeXy methods for references.
    //--------------------------------------------------

    virtual bool hasReference(const DataArray &reference) const = 0;

    virtual bool hasReference(const std::string &id) const = 0;

    virtual size_t referenceCount() const = 0;

    virtual DataArray getReference(const std::string &id) const = 0;

    virtual void addReference(const DataArray &reference) = 0;

    virtual bool removeReference(const DataArray &reference) = 0;

    /**
     * Getter for all referenced DataArrays.
     *
     * @return All referenced DataArrays
     */
    virtual std::vector<DataArray> references() const = 0;

    /**
     * Setter for all referenced DataArrays. Previously referenced
     * DataArrays, that are not in the references vector will be
     * removed.
     *
     * @param references    All referenced arrays.
     */
    virtual void references(const std::vector<DataArray> &references) = 0;

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
    virtual bool hasRepresentation(const std::string &id) const = 0;

    /**
     * Returns the number of representations in this block.
     *
     * @return The number of representations.
     */
    virtual size_t representationCount() const = 0;

    /**
     * Retrieves a specific representation from the tag.
     *
     * @param id        The id of the representation.
     *
     * @return The representation with the specified id. If it doesn't exist
     *         an exception will be thrown.
     */
    virtual Representation getRepresentation(const std::string &id) const = 0;

    /**
     * Retrieves a specific representation from the tag.
     *
     * @param index        The index of the representation.
     *
     * @return The representation with the specified index.
     */
    virtual Representation getRepresentation(size_t index) const = 0;

    /**
     * Getter for all representations of the tag.
     *
     * @return All representations as vector.
     */
    virtual std::vector<Representation> representations() const = 0;

    /**
     * Create a new representation.
     *
     * @param data      The data array of this representation.
     * @param type      The link type of this representation.
     *
     * @return The created representation object.
     */
    virtual Representation createRepresentation(DataArray data, LinkType link_type) = 0;

    /**
     * Deletes a representation from the tag.
     *
     * @param id        The id of the representation to remove.
     *
     * @return True if the representation was removed, false otherwise.
     */
    virtual bool deleteRepresentation(const std::string &id) = 0;


    virtual ~ISimpleTag() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_SIMPLE_TAG_H
