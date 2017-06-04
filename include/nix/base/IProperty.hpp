// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_PROPERTY_H
#define NIX_I_PROPERTY_H

#include <nix/Value.hpp>
#include <nix/base/INamedEntity.hpp>
#include <nix/ObjectType.hpp>

#include <nix/NDSize.hpp>

#include <string>
#include <vector>
#include <cstdlib>

namespace nix {
namespace base {

/**
 * @brief Interface for implementations of the Property entity.
 *
 * See {@link nix::Property} for a more detailed description.
 */
class NIXAPI IProperty : virtual public base::IEntity {

public:

    virtual std::string name() const = 0;


    virtual void definition(const std::string &definition) = 0;


    virtual boost::optional<std::string> definition() const = 0;


    virtual void definition(const none_t t) = 0;


    virtual void mapping(const std::string &mapping) = 0;


    virtual boost::optional<std::string> mapping() const = 0;


    virtual void mapping(const none_t t) = 0;


    virtual DataType dataType() const = 0;


    virtual void unit(const std::string &unit) = 0;


    virtual boost::optional<std::string> unit() const = 0;


    virtual void unit(const none_t t) = 0;


    virtual void deleteValues() = 0;


    virtual ndsize_t valueCount() const = 0;


    virtual void values(const std::vector<Value> &values) = 0;


    virtual std::vector<Value> values(void) const = 0;


    virtual void values(const boost::none_t t) = 0;


    virtual ~IProperty() {}
};


} // namespace base

template<>
struct objectToType<nix::base::IProperty> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::Property;
};

} // namespace nix

#endif // NIX_I_PROPERTY_H
