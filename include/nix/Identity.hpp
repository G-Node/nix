// Copyright (c) 2017, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_IDENTITY_H
#define NIX_IDENTITY_H

#include <string>
#include <nix/base/NamedEntity.hpp>
#include <nix/ObjectType.hpp>

namespace nix {

class NIXAPI Identity {
public:
    Identity(const std::string &name_or_id, ObjectType type);

    Identity(std::string name, std::string id)
        : myName(std::move(name)), myId(std::move(id)) { }

    Identity(std::string name, std::string id, ObjectType type)
        : myName(std::move(name)), myId(std::move(id)), myType(type) { }

    template<typename T>
    Identity(const base::NamedEntity<T> &e)
        : myName(e.name()), myId(e.id()), myType(objectToType<T>::value) { }

    template<typename T>
    Identity(const base::Entity<T> &e)
        : myName(""), myId(e.id()), myType(objectToType<T>::value) { }

    template<typename T>
    Identity(const std::shared_ptr<T> &e)
        : myName(e->name()), myId(e->id()), myType(objectToType<T>::value) { }

    const std::string & id() const { return myId; }
    const std::string & name() const { return myName; }
    const ObjectType type() const { return myType; }

 private:
    std::string myName;
    std::string myId;
    ObjectType  myType;
};

} //nix::

#endif //NIX_IDENTITY_H
