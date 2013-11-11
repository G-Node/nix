// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BLOCK_H
#define NIX_BLOCK_H

#include <nix/base/ImplContainer.hpp>
#include <nix/IBlock.hpp>

namespace nix {


class Block : public IBlock, public base::ImplContainer<IBlock> {

public:

    Block() {}

    Block(const Block &other)
        : ImplContainer(other.impl_ptr)
    {
    }

    Block(const std::shared_ptr<IBlock> &p_impl)
        : ImplContainer(p_impl)
    {
    }

    Block(IBlock *p_impl)
        : ImplContainer(p_impl)
    {
    }

    std::string id() const {
        return impl_ptr->id();
    }


    void type(const std::string &type) {
        impl_ptr->type(type);
    }


    std::string type() const {
        return impl_ptr->type();
    }


    void name(const std::string &name) {
        impl_ptr->name(name);
    }


    std::string name() const {
        return impl_ptr->name();
    }


    void definition(const std::string &definition) {
        impl_ptr->definition(definition);
    }


    std::string definition() const {
        return impl_ptr->definition();
    }


    time_t updatedAt() const {
        return impl_ptr->updatedAt();
    }


    time_t createdAt() const {
        return impl_ptr->createdAt();
    }


    void setUpdatedAt() {
        impl_ptr->setUpdatedAt();
    }


    void forceUpdatedAt() {
        impl_ptr->forceUpdatedAt();
    }


    void setCreatedAt() {
        impl_ptr->setCreatedAt();
    }


    void forceCreatedAt(time_t t) {
        impl_ptr->forceCreatedAt(t);
    }

};

}


#endif // NIX_BLOCK_H
