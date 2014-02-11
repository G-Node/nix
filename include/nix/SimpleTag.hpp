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
#include <nix/Representation.hpp>

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
        : EntityWithSources(other.impl_ptr)
    {
    }


    SimpleTag(const std::shared_ptr<base::ISimpleTag> &p_impl)
        : EntityWithSources(p_impl)
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
        return impl_ptr->units();
    }

    /**
     * Setter for the units of a tag.
     *
     * @param units     All units as a vector.
     */
    void units(std::vector<std::string> &units) {
        impl_ptr->units(units);
    }

    /**
     * Getter for the position of a tag. The position is a vector that
     * points into referenced DataArrays.
     *
     * @return The position vector.
     */
    std::vector<double> position() const {
        return impl_ptr->position();
    }

    /**
     * Setter for the position of a tag.
     *
     * @param position    The position vector.
     */
    void position(const std::vector<double> &position) {
        impl_ptr->position(position);
    }

    /**
     * Getter for the extent of a tag. Given a specified position
     * vector, the extent vector defined the size of a region of
     * interest in the referenced DataArrays.
     *
     * @return The extent of the tag.
     */
    std::vector<double> extent() const {
        return impl_ptr->extent();
    }

    /**
     * Setter for the extent of a tag.
     *
     * @param extent      The extent vector.
     */
    void extent(const std::vector<double> &extent) {
        impl_ptr->extent(extent);
    }

    //--------------------------------------------------
    // Methods concerning references.
    //--------------------------------------------------

    // TODO can be removed from interface.
    bool hasReference(const DataArray &reference) const {
        return hasReference(reference.id());
    }


    bool hasReference(const std::string &id) const {
        return impl_ptr->hasReference(id);
    }

    size_t referenceCount() const {
        return impl_ptr->referenceCount();
    }


    DataArray getReference(const std::string &id) const {
        return getReference(id);
    }


    void addReference(const DataArray &reference) {
        impl_ptr->addReference(reference);
    }


    bool removeReference(const DataArray &reference) {
        return impl_ptr->removeReference(reference);
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
        return impl_ptr->representationCount();
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
     * Remove a representation from the tag.
     *
     * @param id        The id of the representation to remove.
     *
     * @return True if the representation was removed, false otherwise.
     */
    bool removeRepresentation(const std::string &id) {
        return impl_ptr->removeRepresentation(id);
    }


    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------


};


} // namespace nix

#endif // NIX_SIMPLE_TAG_H
