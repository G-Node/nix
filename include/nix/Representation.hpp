// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_REPRESENTATION_H
#define NIX_REPRESENTATION_H

#include <nix/base/Entity.hpp>
#include <nix/IRepresentation.hpp>
#include <nix/DataArray.hpp>

namespace nix {


/**
 * Class that represents a pandora representation entity
 */
class Representation : virtual public IRepresentation, public base::Entity<IRepresentation> {

public:

    Representation()
        : Entity()
    {
    }


    Representation(const Representation &other)
        : Entity(other.impl_ptr)
    {
    }


    Representation(const std::shared_ptr<IRepresentation> p_impl)
        : Entity(p_impl)
    {
    }


    /**
     * Setter for the link type.
     *
     * @param type    The link type to set.
     */
    void linkType(LinkType type) {
        impl_ptr->linkType(type);
    }

    /**
     * Getter for the link type.
     *
     * @return The current link type of the representation.
     */
    LinkType linkType() const {
        return impl_ptr->linkType();
    }

    /**
     * Sets the data array associated with this representation.
     *
     * @param data    The data array to set.
     */
    void data(const DataArray &data) {
        impl_ptr->data(data);
    }

    /**
     * Gets the data array associated with this representation.
     *
     * @return The associated data array.
     */
    DataArray data() const {
        return impl_ptr->data();
    }

    /**
     * Destructor.
     */
    virtual ~Representation() {}
};


} // namespace nix

#endif // NIX_REPRESENTATION_H
