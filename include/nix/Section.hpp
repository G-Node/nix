// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_SECTION_H
#define NIX_SECTION_H

#include <nix/base/ImplContainer.hpp>
#include <nix/ISection.hpp>

namespace nix {

class Section : public ISection, public base::ImplContainer<ISection> {

public:

    Section() {}

    Section(const Section &other)
        : ImplContainer(other.impl_ptr)
    {
    }

    Section(const std::shared_ptr<ISection> &p_impl)
        : ImplContainer(p_impl)
    {
    }

    Section(ISection *p_impl)
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

#endif // NIX_SECTION_H
