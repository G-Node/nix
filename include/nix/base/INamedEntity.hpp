// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_NAMEDENTITY_H
#define NIX_I_NAMEDENTITY_H

#include <memory>

#include <nix/base/IEntity.hpp>
#include <boost/optional.hpp>

namespace nix {
namespace base {

/**
 * @brief Interface for entities with a {@link name}, {@link type}
 * and {@link definition}.
 *
 * See {@link nix::base::NamedEntity} for more detailed information.
 */
class NIXAPI INamedEntity : virtual public IEntity {

public:

    /**
     * @brief Setter for the type of the entity.
     *
     * @param type The type to set.
     */
    virtual void type(const std::string &type) = 0;

    /**
     * @brief Getter for the type of the entity.
     *
     * The property {@link type} is used in order to allow the specification
     * of additional semantic meaning for an entity and therefore can introduce
     * domain-specificity into the quite generic data model.
     *
     * @return The current type.
     */
    virtual std::string type() const = 0;

    /**
     * @brief Setter for the name of the entity.
     *
     * @param name The name of the entity.
     */
    virtual void name(const std::string &name) = 0;

    /**
     * @brief Getter for the name of the entity.
     *
     * The {@link name} of an entity serves as a human readable identifier. It is not obliged
     * to be unique. However it is strongly recommended to use unique name inside one specific
     * {@link nix::Block}.
     *
     * @return string The name of the entity.
     */
    virtual std::string name() const = 0;

    /**
     * @brief Setter for the definition of the entity.
     *
     * @param definition The definition of the entity.
     */
    virtual void definition(const std::string &definition) = 0;

    /**
     * @brief Getter for the definition of the entity.
     *
     * The {@link definition} is an optional property that allows the user to add
     * a freely assignable textual definition to the entity.
     *
     * @return The definition of the entity.
     */
    virtual boost::optional<std::string> definition() const = 0;

    /**
     * @brief Deleter for the definition of the entity.
     */
    virtual void definition(const none_t t) = 0;

    /**
     * @brief Compare two named entities.
     *
     * @param other The entity to compare with.
     *
     * @return > 0 if the entity is larger that other, 0 if both are
     * equal, and < 0 otherwise.
     */
    virtual int compare(std::shared_ptr<INamedEntity> other) const = 0;

    /**
     * @brief Destructor
     */
    virtual ~INamedEntity() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_NAMEDENTITY_H
