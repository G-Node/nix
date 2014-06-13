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
#include <vector>
#include <list>
#include <functional>
#include <utility>

#include <nix/Platform.hpp> //for pragma warnings on windows
#include <nix/None.hpp>
#include <nix/Exception.hpp>

namespace nix {
namespace base {

template<typename T>
class ImplContainer {
protected:

    /**
     * Low level helper to get multiple entities via a getter function
     * that has to be provided.
     *
     * Get multiple entities of given type and return them as vector.
     * The template param specifies which type.
     * The parameter "getEnt" is the function used to get the entities
     * of given type T_ENT and has to be of return type T_ENT with
     * parameter "size_t" specifying the index of the entity to get.
     * The parameter "nT" should give the number of entities to get and
     * should be =>0 & <= the total number of existing entities.
     * The parameter "filter" is defaulted to giving back all entities
     * of given type. To use your own filter pass a lambda that accepts
     * an entity of given type as parameter and returns a bool telling
     * whether to get it or not.
     * NOTE: there is no need to specify 2nd template param TFUNC as it
     * should be automatically deduced.
     *
     * @param getEntity         Function of return type TENT.
     * @param nT                Number of entities to get.
     * @param filter            Filter function.
     *
     * @return A vector with all filtered entities.
     */
    template<typename TENT, typename TFUNC>
    std::vector<TENT> getEntities(
        TFUNC const &getEntity,
        size_t nT,
        std::function<bool(TENT)> filter) const
    {
        std::vector<TENT> entities;
        TENT candidate;

        if (nT < 1) {
            return entities;
        }

        for (size_t i = 0; i < nT; i++) {
            candidate = getEntity(i);
            if (candidate && filter(candidate)) {
                entities.push_back(candidate);
            }
        }

        return entities;
    }

public:

    ImplContainer()
        : impl_ptr(nullptr)
    {
    }


    ImplContainer(T *p_impl)
        : impl_ptr(p_impl)
    {
    }


    ImplContainer(const std::shared_ptr<T> &p_impl)
        : impl_ptr(p_impl)
    {
    }

    ImplContainer(std::shared_ptr<T> &&ptr)
        : impl_ptr(std::move(ptr))
    {
    }


    ImplContainer(const ImplContainer<T> &other)
        : impl_ptr(other.impl_ptr)
    {
    }

    bool isNone() const {
        return !impl_ptr;
    }


    explicit operator bool() const {
        return !isNone();
    }


    virtual ImplContainer<T> &operator=(const ImplContainer<T> &other) {
        if (*this != other) {
            ImplContainer tmp(other);
            swap(tmp);
        }
        return *this;
    }


    virtual ImplContainer<T> &operator=(none_t t) {
        impl_ptr = nullptr;
        return *this;
    }


    virtual bool operator==(const ImplContainer<T> &other) const {
        return this->impl_ptr == other.impl_ptr;
    }


    virtual bool operator!=(const ImplContainer<T> &other) const {
        return !(*this == other);
    }


    virtual bool operator==(none_t t) const {
        return impl_ptr == nullptr;
    }

    virtual bool operator==(bool b) const {
        return !isNone() == b;
    }


    virtual bool operator!=(none_t t) const {
        return impl_ptr != nullptr;
    }

    virtual bool operator!=(bool b) const {
        return isNone() == b;
    }


    virtual void swap(ImplContainer<T> &second) {
        using std::swap;

        swap(impl_ptr, second.impl_ptr);
    }


    virtual ~ImplContainer() {}


    const std::shared_ptr<T> & impl() const {
        return impl_ptr;
    }

    std::shared_ptr<T> & impl() {
        return impl_ptr;
    }

protected:

    T* backend() {
        if (isNone()) {
            throw UninitializedEntity();
        }

        return impl_ptr.get();
    }

    const T* backend() const {
        return const_cast<ImplContainer *>(this)->backend();
    }

    void nullify() {
        impl_ptr = nullptr;
    }

private:

    std::shared_ptr<T> impl_ptr;

};


} // namespace base
} // namespace nix

#endif
