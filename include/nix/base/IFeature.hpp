// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_FEATURE_H
#define NIX_I_FEATURE_H

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
 * Class that represents a pandora feature entity
 * TODO implement methods for DataArray.
 */
class NIXAPI IFeature: virtual public base::IEntity {

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
     * @return The current link type of the feature.
     */
    virtual LinkType linkType() const = 0;

    /**
     * Sets the data array associated with this feature.
     *
     * @param data    The id of the data array to set.
     */
    virtual void data(const std::string &data_array_id) = 0;

    /**
     * Gets the data array associated with this feature.
     *
     * @return The associated data array.
     */
    virtual DataArray data() const = 0;


    virtual ~IFeature() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_FEATURE_H
