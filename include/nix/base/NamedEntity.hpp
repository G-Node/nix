// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_NAMED_ENTITY_H
#define NIX_NAMED_ENTITY_H

#include <nix/base/INamedEntity.hpp>
#include <nix/base/Entity.hpp>

#include <nix/valid/validate.hpp>

namespace nix {
namespace base {

/**
 * @brief Base class for entities with a {@link name}, {@link type} and a
 * {@link definition}
 *
 * In addition to the properties defined by {@link nix::base::Entity} most entities of the
 * NIX data model further provide a {@link name}, {@link type} and a {@link definition}.
 * The {@link name} of an entity serves as a human readable identifier. It is not obliged
 * to be unique. However it is strongly recommended to use unique name inside one specific
 * {@link nix::Block}. The property {@link type} is used in order to allow the specification
 * of additional semantic meaning for an entity and therefore can introduce
 * domain-specificity into the quite generic data model. The {@link definition} is an optional
 * property that allows the user to add a freely assignable textual definition to the entity.
 */
template<typename T>
class NamedEntity : virtual public INamedEntity, public Entity<T> {

public:

    /**
     * @brief Constructor that creates a null entity.
     */
    NamedEntity()
        : Entity<T>()
    {
    }

    /**
     * @brief Constructor that creates a new entity from a shared pointer to
     * an implementation instance.
     */
    NamedEntity(const std::shared_ptr<T> &p_impl)
        : Entity<T>(p_impl)
    {
    }

    /**
     * @brief Constructor with move semantics that creates a new entity from a
     * shared pointer to an implementation instance.
     */
    NamedEntity(std::shared_ptr<T> &&ptr)
        : Entity<T>(std::move(ptr))
    {
    }

    void type(const std::string &type) {
        Entity<T>::backend()->type(type);
    }


    std::string type() const {
        return Entity<T>::backend()->type();
    }


    void name(const std::string &name) {
        Entity<T>::backend()->name(name);
    }


    std::string name() const {
        return Entity<T>::backend()->name();
    }
    

    void definition(const std::string &definition) {
        Entity<T>::backend()->definition(definition);
    }


    boost::optional<std::string> definition() const {
        return Entity<T>::backend()->definition();
    }


    void definition(const none_t t)
    {
        Entity<T>::backend()->definition(t);
    }


    int compare(const INamedEntity &other) const {
        return Entity<T>::backend()->compare(other);
    }

    /**
     * @brief Destructor
     */
    virtual ~NamedEntity() {}
    
    //------------------------------------------------------
    // Validation
    //------------------------------------------------------
    
    valid::Result validate() {
        valid::Result result_base = Entity<T>::validate();
        valid::Result result = valid::validate(std::initializer_list<valid::condition> {
            valid::must(*this, &NamedEntity::name, valid::notEmpty(), "no name set!"),            
            valid::must(*this, &NamedEntity::type, valid::notEmpty(), "no type set!")
        });
        
        return result.concat(result_base);
    }

};


} // namespace base
} // namespace nix
#endif // NIX_NAMED_ENTITY_H
