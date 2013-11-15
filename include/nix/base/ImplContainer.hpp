// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef IMPL_CONTAINER_H
#define IMPL_CONTAINER_H

#include <memory>

namespace nix {
namespace base {

template<typename T>
class ImplContainer {

protected:

    std::shared_ptr<T> impl_ptr;

public:

    ImplContainer() {}

    ImplContainer(T *p_impl)
        : impl_ptr(p_impl)
    {
    }

    ImplContainer(const std::shared_ptr<T> &p_impl)
        : impl_ptr(p_impl)
    {
    }

    ImplContainer(const ImplContainer<T> &other)
        : impl_ptr(other.impl_ptr)
    {
    }

    /*
    std::shared_ptr<T> impl() const {
        return impl_ptr;
    }
    */

    virtual ImplContainer<T> &operator=(const ImplContainer<T> &other) {
        ImplContainer tmp(other);

        swap(*this, tmp);
        return *this;
    }

    virtual bool operator==(const ImplContainer<T> &other) {
        return this->impl_ptr == other.impl_ptr;
    }

    virtual bool operator!=(const ImplContainer<T> &other) {
        return !(*this == other);
    }

    friend void swap(ImplContainer<T> &first, ImplContainer<T> &second) {
        using std::swap;

        swap(first.impl_ptr, second.impl_ptr);
    }

    virtual ~ImplContainer() {}
};


} // namespace base
} // namespace nix

#endif
