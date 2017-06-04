// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_FEATURE_H
#define NIX_I_FEATURE_H

#include <nix/base/IDataArray.hpp>
#include <nix/ObjectType.hpp>

#include <string>
#include <memory>


namespace nix {

/**
 * @brief Enumeration for link types.
 *
 *  TODO remove "untagged" from link type
 */
enum class LinkType : int {
    Tagged, Untagged, Indexed
};



namespace base {

/**
 * @brief Interface for implementations of the Feature entity.
 *
 * See {@link nix::Feature} for a more detailed description.
 */
class NIXAPI IFeature: virtual public base::IEntity {

public:

    virtual void linkType(LinkType type) = 0;


    virtual LinkType linkType() const = 0;


    virtual void data(const std::string &name_or_id) = 0;


    virtual std::shared_ptr<IDataArray> data() const = 0;


    virtual ~IFeature() {}

};


} // namespace base

template<>
struct objectToType<nix::base::IFeature> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::Feature;
};

} // namespace nix

#endif // NIX_I_FEATURE_H
