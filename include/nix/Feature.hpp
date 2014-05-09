// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_FEATURE_H
#define NIX_FEATURE_H

#include <nix/base/Entity.hpp>
#include <nix/base/IFeature.hpp>
#include <nix/DataArray.hpp>

#include <nix/Platform.hpp>

namespace nix {


/**
 * Class that represents a pandora feature entity
 */
class NIXAPI Feature : virtual public base::IFeature, public base::Entity<base::IFeature> {

public:

    Feature()
        : Entity()
    {
    }


    Feature(const Feature &other)
        : Entity(other.impl())
    {
    }


    Feature(const std::shared_ptr<base::IFeature> &p_impl)
        : Entity(p_impl)
    {
    }

    Feature(std::shared_ptr<base::IFeature> &&ptr)
        : Entity(std::move(ptr))
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
     * @return The current link type of the feature.
     */
    LinkType linkType() const {
        return backend()->linkType();
    }

    /**
     * Sets the data array associated with this feature.
     *
     * @param std::string       The id of the data array to set.
     */
    void data(const std::string &data_array_id) {
        backend()->data(data_array_id);
    }

    /**
     * Sets the data array associated with this feature.
     *
     * @param DataArray    The data array to set.
     */
    void data(const DataArray &data) {
        if(data == none) {
            throw std::runtime_error("Empty data entity (DataArray) given");
        }
        else {
            backend()->data(data.id());
        }
    }

    /**
     * Gets the data array associated with this feature.
     *
     * @return The associated data array.
     */
    DataArray data() const {
        return backend()->data();
    }

    /**
     * Destructor.
     */
    virtual ~Feature() {}
};


} // namespace nix

#endif // NIX_FEATURE_H
