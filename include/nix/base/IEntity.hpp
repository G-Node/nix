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

namespace nix {
namespace base {

/**
 * @brief Interface that declares a basic features of most entities.
 *
 * See {@link nix::base::Entity} for more information.
 */
class NIXAPI IEntity {
public:

    /**
     * @brief Getter for the id of the entity.
     *
     * The id is generated at creation time of an entity. It contains
     * a randomly generated sequence of characters with low collision
     * probability.
     *
     * @return The id of the entity.
     */
    virtual std::string id() const = 0;

    /**
     * @brief Gets the time of the last update.
     *
     * @return Time of the last update.
     */
    virtual time_t updatedAt() const = 0;

    /**
     * @brief Gets the creation time.
     *
     * @return The creation time of the entity.
     */
    virtual time_t createdAt() const = 0;

    /**
     * @brief Sets the time of the last update to the current time if
     * the field is not set.
     */
    virtual void setUpdatedAt() = 0;

    /**
     * @brief Sets the time of the last update to the current time.
     */
    virtual void forceUpdatedAt() = 0;

    /**
     * @brief Sets the creation time to the current time if the creation
     * time is not set.
     */
    virtual void setCreatedAt() = 0;

    /**
     * @brief Sets the creation time to the provided value even if the
     * field is already set.
     *
     * @param time The creation time to set.
     */
    virtual void forceCreatedAt(time_t t) = 0;

    /**
     * @brief Destructor
     */
    virtual ~IEntity() {}
};


} // namespace base
} // namespace nix

#endif // NIX_I_ENTITY_H
