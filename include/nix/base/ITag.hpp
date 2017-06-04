// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_TAG_H
#define NIX_I_TAG_H

#include <nix/base/IEntityWithSources.hpp>
#include <nix/base/IDataArray.hpp>
#include <nix/base/IFeature.hpp>
#include <nix/base/IBaseTag.hpp>
#include <nix/ObjectType.hpp>

#include <string>
#include <vector>

namespace nix {

class DataArray;

namespace base {

/**
 * @brief Interface for implementations of the Tag entity.
 *
 * See {@link nix::Tag} for a more detailed description.
 */
class NIXAPI ITag : virtual public base::IBaseTag {

public:

    virtual std::vector<std::string> units() const = 0;


    virtual void units(const std::vector<std::string> &units) = 0;


    virtual void units(const none_t t) = 0;


    virtual std::vector<double> position() const = 0;


    virtual void position(const std::vector<double> &position) = 0;


    virtual std::vector<double> extent() const = 0;


    virtual void extent(const std::vector<double> &extent) = 0;


    virtual void extent(const none_t t) = 0;


    virtual ~ITag() {}

};


} // namespace base

template<>
struct objectToType<nix::base::ITag> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::Tag;
};

} // namespace nix

#endif // NIX_I_TAG_H
