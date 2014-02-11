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
#include <nix/Representation.hpp>

#include <nix/Platform.hpp>

namespace nix {


class NIXAPI DataTag : virtual public base::IDataTag, public base::EntityWithSources<base::IDataTag> {

public:

	DataTag()
: EntityWithSources()
{}

	DataTag(const DataTag &other)
	: EntityWithSources(other.impl_ptr)
	{
	}

	DataTag(const std::shared_ptr<base::IDataTag> &p_impl)
	: EntityWithSources(p_impl)
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
		return impl_ptr->positions();
	}

	/**
	 * Setter for the positions of a tag.
	 *
	 * @param id   string.
	 */
	void positions(const std::string &id) {
		impl_ptr->positions(id);
	}

	/**
	 * Setter for the positions of a tag.
	 *
	 * @param position   DataArray.
	 */
	void positions(const DataArray &positions) {
		impl_ptr->positions(positions.id());
	}

	/**
	 * Returns whether this DataArray contains positions.
	 *
	 * @return bool
	 */
	bool hasPositions() const {
		return impl_ptr->hasPositions();
	}

	/**
	 * Removes the positions reference form the DataTag, does not delete the
	 * referenced DataArray itself.
	 *
	 * @return bool
	 */
	bool removePositions(){
		return impl_ptr->removePositions();
	}

	/**
	 * Getter for the extents of the tag which are stored in a#
	 * DataArray
	 *
	 * @return The extents of the tag.
	 */
	DataArray extents() const {
		return impl_ptr->extents();
	}

	/**
	 * Sets the extents DataArray of the tag.
	 *
	 * @param extent      The extent vector.
	 */
	void extents(const DataArray &extents) {
		impl_ptr->extents(extents.id());
	}

	/**
	 * Set the reference to the extents DataArray.
	 *
	 * @param the id.
	 */
	void extents(const std::string &id) {
		impl_ptr->extents(id);
	}

	/**
	 * Returns whether this DataArray contains extents.
	 *
	 * @return bool
	 */
	bool hasExtents() const {
		return impl_ptr->hasExtents();
	}

	/**
	 * Removes the extents reference form the DataTag, does not delete the
	 * referenced DataArray itself.
	 *
	 * @return bool
	 */
	bool removeExtents(){
		return impl_ptr->removeExtents();
	}
	//--------------------------------------------------
	// Methods concerning references.
	// TODO implement when done with the DataArray class.
	//--------------------------------------------------

	bool hasReference(const DataArray &reference) const {
		return impl_ptr->hasReference(reference.id());
	}


	bool hasReference(const std::string &id) const {
		return impl_ptr->hasReference(id);
	}


	size_t referenceCount() const {
		return impl_ptr->referenceCount();
	}


	DataArray getReference(const std::string &id) const {
		return impl_ptr->getReference(id);
	}


	void addReference(const DataArray &reference) {
		impl_ptr->addReference(reference);
	}


	bool removeReference(const std::string &id) {
		return impl_ptr->removeReference(id);
	}

	/**
	 * Getter for all referenced DataArrays.
	 *
	 * @return All referenced DataArrays
	 */
	std::vector<DataArray> references() const {
		return impl_ptr->references();
	}

	/**
	 * Setter for all referenced DataArrays. Previously referenced
	 * DataArrays, that are not in the references vector will be
	 * removed.
	 *
	 * @param references    All referenced arrays.
	 */
	void references(const std::vector<DataArray> &references) {
		impl_ptr->references(references);
	}

	//TODO to be implemented later
	/*
	template<typename T, size_t dims>
	void getReferencedData(std::vector<boost::multi_array<T, dims>> &data, size_t index) const{

		if (referenceCount() == 0){
			throw std::runtime_error("DataTagHDF5::getReferencedData: There is no reference attached to this tag!");
		}
		if(!hasPositions()){
			throw std::runtime_error("DataTagHDF5::getReferencedData: There is no positions array attached to this tag!");
		}

		DataArray pa = positions();
		boost::multi_array<double,1> posData, extData;
		pa.getData(posData);

		if(index >= posData.shape()[0]){
			throw std::runtime_error("DataTagHDF5::getReferencedData: index exeeds matrix dimensions in positions data!");
		}

		if(hasExtents()){
			DataArray ea = extents();
			ea.getData(extData);
		}

		//TODO convert position and extent to respective units
		//TODO get the data slice from the referenced DataArrays
		std::vector<DataArray> refs = references();
		for (size_t i = 0; i < refs.size();i++){

		}
	}
	*/
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
	bool hasRepresentation(const std::string &id) const {
		return impl_ptr->hasRepresentation(id);
	}

	/**
	 * Returns the number of representations in this block.
	 *
	 * @return The number of representations.
	 */
	size_t representationCount() const {
		return impl_ptr->referenceCount();
	}

	/**
	 * Retrieves a specific representation from the tag.
	 *
	 * @param id        The id of the representation.
	 *
	 * @return The representation with the specified id. If it doesn't exist
	 *         an exception will be thrown.
	 */
	Representation getRepresentation(const std::string &id) const {
		return impl_ptr->getRepresentation(id);
	}

	/**
	 * Retrieves a specific representation from the tag.
	 *
	 * @param index        The index of the representation.
	 *
	 * @return The representation with the specified index.
	 */
	Representation getRepresentation(size_t index) const {
		return impl_ptr->getRepresentation(index);
	}

	/**
	 * Getter for all representations of the tag.
	 *
	 * @return All representations as vector.
	 */
	std::vector<Representation> representations() const {
		return impl_ptr->representations();
	}

	/**
	 * Create a new representation.
	 *
	 * @param data      The data array of this representation.
	 * @param type      The link type of this representation.
	 *
	 * @return The created representation object.
	 */
	Representation createRepresentation(DataArray data, LinkType link_type) {
		return impl_ptr->createRepresentation(data, link_type);
	}

	/**
	 * Delete a representation from the tag.
	 *
	 * @param id        The id of the representation to delete.
	 *
	 * @return True if the representation was removed, false otherwise.
	 */
	bool deleteRepresentation(const std::string &id) {
		return impl_ptr->deleteRepresentation(id);
	}

	virtual DataTag &operator=(std::nullptr_t nullp) {
		impl_ptr = nullp;
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
