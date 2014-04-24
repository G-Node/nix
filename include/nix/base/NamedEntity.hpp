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


    boost::optional<std::string> name() const {
        return Entity<T>::backend()->name();
    }


    void name(const none_t t)
    {
        Entity<T>::backend()->name(t);
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


    virtual ~NamedEntity() {}

};


} // namespace base
} // namespace nix
#endif // NIX_NAMED_ENTITY_H
