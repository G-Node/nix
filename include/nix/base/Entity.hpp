// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_ENTITY_H
#define NIX_ENTITY_H

#include <nix/base/IEntity.hpp>
#include <nix/base/ImplContainer.hpp>

namespace nix {
namespace base {

template<typename T>
class Entity : virtual public IEntity, public ImplContainer<T> {

public:

    Entity()
        : ImplContainer<T>()
    {
    }


    Entity(const std::shared_ptr<T> &p_impl)
        : ImplContainer<T>(p_impl)
    {
    }


    std::string id() const {
        return ImplContainer<T>::impl_ptr->id();
    }


    time_t updatedAt() const {
        return ImplContainer<T>::impl_ptr->updatedAt();
    }


    time_t createdAt() const {
        return ImplContainer<T>::impl_ptr->createdAt();
    }


    void setUpdatedAt() {
        ImplContainer<T>::impl_ptr->setUpdatedAt();
    }


    void forceUpdatedAt() {
        ImplContainer<T>::impl_ptr->forceUpdatedAt();
    }


    void setCreatedAt() {
        ImplContainer<T>::impl_ptr->setCreatedAt();
    }


    void forceCreatedAt(time_t t) {
        ImplContainer<T>::impl_ptr->forceCreatedAt(t);
    }


};

} // namespace base
} // namespace nix

#endif // NIX_ENTITY_H
