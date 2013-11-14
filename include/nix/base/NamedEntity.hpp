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

namespace nix {
namespace base {


template<typename T>
class NamedEntity : virtual public INamedEntity, public Entity<T> {

public:

    NamedEntity()
        : Entity<T>()
    {
    }


    NamedEntity(const std::shared_ptr<T> &p_impl)
        : Entity<T>(p_impl)
    {
    }


    void type(const std::string &type) {
        Entity<T>::impl_ptr->type(type);
    }


    std::string type() const {
        return Entity<T>::impl_ptr->type();
    }


    void name(const std::string &name) {
        Entity<T>::impl_ptr->name(name);
    }


    std::string name() const {
        return Entity<T>::impl_ptr->name();
    }


    void definition(const std::string &definition) {
        Entity<T>::impl_ptr->definition(definition);
    }


    std::string definition() const {
        return Entity<T>::impl_ptr->definition();
    }


    int compare(const INamedEntity &other) const {
        return Entity<T>::impl_ptr->compare(other);
    }

};


} // namespace base
} // namespace nix
#endif // NIX_NAMED_ENTITY_H
