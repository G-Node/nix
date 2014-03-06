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
#include <nix/base/IRepresentation.hpp>
#include <nix/DataArray.hpp>

#include <nix/Platform.hpp>

namespace nix {


/**
 * Class that represents a pandora representation entity
 */
class NIXAPI Representation : virtual public base::IRepresentation, public base::Entity<base::IRepresentation> {

public:

    Representation()
        : Entity()
    {
    }


    Representation(const Representation &other)
        : Entity(other.impl())
    {
    }


    Representation(const std::shared_ptr<base::IRepresentation> p_impl)
        : Entity(p_impl)
    {
    }


    /**
     * Setter for the link type.
     *
     * @param type    The link type to set.
     */
    void linkType(LinkType type) {
        backend()->linkType(type);
    }

    /**
     * Getter for the link type.
     *
     * @return The current link type of the representation.
     */
    LinkType linkType() const {
        return backend()->linkType();
    }

    /**
     * Sets the data array associated with this representation.
     *
     * @param data    The data array to set.
     */
    void data(const DataArray &data) {
        backend()->data(data);
    }

    /**
     * Gets the data array associated with this representation.
     *
     * @return The associated data array.
     */
    DataArray data() const {
        return backend()->data();
    }

    /**
     * Destructor.
     */
    virtual ~Representation() {}
};


} // namespace nix

#endif // NIX_REPRESENTATION_H
