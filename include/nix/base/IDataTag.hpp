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
#include <nix/Representation.hpp>

namespace nix {

class Representation;
class DataArray;


namespace base {


class IDataTag : virtual public base::IEntityWithSources {

public:

	//--------------------------------------------------
	// Element getters and setters
	//--------------------------------------------------

	/**
	 * Getter for the positions of a tag. The positions are strored in a DataArray.
	 *
	 *
	 * @return The positions DataArray.
	 */
	virtual DataArray positions() const = 0;

	/**
	 * Setter for the positions of a tag.
	 *
	 * @param position   string.
	 */
	virtual void positions(const std::string &positionid) = 0;

	/**
	 * Returns whether this DataArray contains positions.
	 *
	 * @return bool
	 */
	virtual bool hasPositions() const = 0;

	/**
	 * Removes the reference to the positions DataArray from the DataTag.
	 * Note: This function does not delete the referenced DataArray!
	 *
	 * @return bool
	 */
	virtual bool removePositions() = 0;

	/**
	 * Getter for the extents of the tag which are stored in a#
	 * DataArray
	 *
	 * @return The extents of the tag.
	 */
	virtual DataArray extents() const = 0;

	/**
	 * Set the reference to the extents DataArray.
	 *
	 * @param the id.
	 */
	virtual void extents(const std::string &extentsId) = 0;

	/**
	 * Returns whether this DataArray contains extents.
	 *
	 * @return bool
	 */
	virtual bool hasExtents() const = 0;

	/**
	 * Removes the reference to the extents DataArray from the DataTag.
	 * Note: This function does not delete the referenced DataArray!
	 *
	 * @return bool
	 */
	virtual bool removeExtents() = 0;

	//--------------------------------------------------
	// Methods concerning references.
	// TODO implement when done with the DataArray class.
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
	 * Delete a representation from the tag.
	 *
	 * @param id        The id of the representation to remove.
	 *
	 * @return True if the representation was removed, false otherwise.
	 */
	virtual bool deleteRepresentation(const std::string &id) = 0;

};

} // namespace base
} // namespace nix

#endif // NIX_I_DATA_TAG_H
