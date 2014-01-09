// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_REPRESENTATION_H
#define NIX_I_REPRESENTATION_H

#include <nix/base/IEntity.hpp>

namespace nix {

class DataArray;

/**
 * Enumeration for link types.
 *
 *  TODO remove "untagged" from link type
 */
enum class LinkType : int {
    Tagged, Untagged, Indexed
};



namespace base {

/**
 * Class that represents a pandora representation entity
 * TODO implement methods for DataArray.
 */
class IRepresentation: virtual public base::IEntity {

public:

    /**
     * Setter for the link type.
     *
     * @param type    The link type to set.
     */
    virtual void linkType(LinkType type) = 0;

    /**
     * Getter for the link type.
     *
     * @return The current link type of the representation.
     */
    virtual LinkType linkType() const = 0;

    /**
     * Sets the data array associated with this representation.
     *
     * @param data    The data array to set.
     */
    virtual void data(const DataArray &data) = 0;

    /**
     * Gets the data array associated with this representation.
     *
     * @return The associated data array.
     */
    virtual DataArray data() const = 0;

};


} // namespace base
} // namespace nix

#endif // NIX_I_REPRESENTATION_H
