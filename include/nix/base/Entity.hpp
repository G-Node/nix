// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_ENTITY_H
#define NIX_ENTITY_H

#include <nix/base/ImplContainer.hpp>

namespace nix {
namespace base {

/**
 * @brief A class serving as base class for most entities of
 * the NIX data model.
 *
 * Most entities defined by the NIX data model share some common properties.
 * Among those an unique identifier {@link id} with low collision probability.
 * The {@link id} allows to identify individual entities even across large
 * collection of data spread over multiple files.
 * Further all entities provide a the properties {@link createdAt} and
 * {@link updatedAt} containing information about the creation time and the
 * last modification time.
 */
template<typename T>
class Entity : public ImplContainer<T> {

public:

    /**
     * @brief Constructor that creates a null entity.
     */
    Entity()
        : ImplContainer<T>()
    {
    }

    /**
     * @brief Constructor that creates a new entity from a shared pointer to
     * an implementation instance.
     */
    Entity(const std::shared_ptr<T> &p_impl)
        : ImplContainer<T>(p_impl)
    {
    }

    /**
     * @brief Constructor with move semantics that creates a new entity from a
     * shared pointer to an implementation instance.
     */
    Entity(const std::shared_ptr<T> &&ptr)
        : ImplContainer<T>(std::move(ptr))
    {
    }

    /**
     * @brief Getter for the id of the entity.
     *
     * The id is generated at creation time of an entity. It contains
     * a randomly generated sequence of characters with low collision
     * probability.
     *
     * @return The id of the entity.
     */
    std::string id() const {
        return ImplContainer<T>::backend()->id();
    }

    /**
     * @brief Gets the time of the last update.
     *
     * @return Time of the last update.
     */
    time_t updatedAt() const {
        return ImplContainer<T>::backend()->updatedAt();
    }

    /**
     * @brief Gets the creation time.
     *
     * @return The creation time of the entity.
     */
    time_t createdAt() const {
        return ImplContainer<T>::backend()->createdAt();
    }

    /**
     * @brief Sets the time of the last update to the current time if
     * the field is not set.
     */
    void setUpdatedAt() {
        ImplContainer<T>::backend()->setUpdatedAt();
    }

    /**
     * @brief Sets the time of the last update to the current time.
     */
    void forceUpdatedAt() {
        ImplContainer<T>::backend()->forceUpdatedAt();
    }

    /**
     * @brief Sets the creation time to the current time if the creation
     * time is not set.
     */
    void setCreatedAt() {
        ImplContainer<T>::backend()->setCreatedAt();
    }

    /**
     * @brief Sets the creation time to the provided value even if the
     * field is already set.
     *
     * @param t The creation time to set.
     */
    void forceCreatedAt(time_t t) {
        ImplContainer<T>::backend()->forceCreatedAt(t);
    }

    /**
     * @brief Destructor
     */
    virtual ~Entity() {}

};

} // namespace base
} // namespace nix

#endif // NIX_ENTITY_H
