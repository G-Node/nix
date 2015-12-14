// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_ENTITY_H
#define NIX_I_ENTITY_H

#include <nix/Platform.hpp>
#include <nix/None.hpp>

#include <string>
#include <ctime>

namespace nix {
namespace base {

/**
 * @brief Interface that declares a basic features of most entities.
 *
 * See {@link nix::base::Entity} for more information.
 */
class NIXAPI IEntity {
public:

    virtual std::string id() const = 0;


    virtual time_t updatedAt() const = 0;


    virtual time_t createdAt() const = 0;


    virtual void setUpdatedAt() = 0;


    virtual void forceUpdatedAt() = 0;


    virtual void setCreatedAt() = 0;


    virtual void forceCreatedAt(time_t t) = 0;

    virtual  bool isValidEntity() const = 0;

    virtual ~IEntity() {}
};


} // namespace base
} // namespace nix

#endif // NIX_I_ENTITY_H
