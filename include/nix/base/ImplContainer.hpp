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

#include <nix/None.hpp>

namespace nix {
namespace base {

template<typename T>
class ImplContainer {

protected:

    typedef boost::none_t none_t;
    none_t none = boost::none;
    std::shared_ptr<T> impl_ptr;

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
     * @param class "get function": std::function of return type T_ENT and 
     *              param type "size_t" to get entities
     * @param size_t number of entities to get
     * @param class "filter function": std::function of return type bool
     *              and param type T_ENT to filter which entities to get
     * @return class entities of the given type as a vector
     */
    template<typename TENT, typename TFUNC>
    std::vector<TENT> getEntities(
        TFUNC const &getEntity,
        size_t nT,
        std::function<bool(TENT)> filter) const 
    {
        std::vector<TENT> entities;
        TENT candidate;
        size_t skipped = 0;
        if(nT < 1) { 
			return entities; 
		}

		// loop until numeric limits of "size_t" or until "nT" entities have been found
        for (size_t i = 0; i < std::numeric_limits<size_t>::max(); i++) {
            try {
                candidate = getEntity(i);
                if(filter(candidate)) entities.push_back(candidate);
                else skipped++;
                if(entities.size()+skipped >= nT) {
					break;
				}
            } catch(std::exception& e) {
                continue;
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


    ImplContainer(const ImplContainer<T> &other)
        : impl_ptr(other.impl_ptr)
    {
    }
    
    bool isNone() const
    {
        return !impl_ptr;    
    }


    explicit operator bool() const
    {
        return !isNone();
    }


    virtual ImplContainer<T> &operator=(const ImplContainer<T> &other) {
        if (*this != other) {
            ImplContainer tmp(other);
            swap(tmp);
        }
        return *this;
    }


    virtual ImplContainer<T> &operator=(std::nullptr_t nullp) {
        impl_ptr = nullp;
        return *this;
    }


    virtual bool operator==(const ImplContainer<T> &other) const {
        return this->impl_ptr == other.impl_ptr;
    }


    virtual bool operator!=(const ImplContainer<T> &other) const {
        return !(*this == other);
    }

    // bool "==" operator "boost::none_t" overload: when an object
    // is compared to "none_t" (e.g. boost::none) internally we compare
    // the "impl_ptr" to the null pointer.
    virtual bool operator==(none_t t) const {
        std::nullptr_t nullp;
        return impl_ptr == nullp;
    }

    // bool "=!" operator "boost::none_t" overload: same as "==" operator.
    virtual bool operator!=(none_t t) const {
        std::nullptr_t nullp;
        return impl_ptr != nullp;
    }


    virtual void swap(ImplContainer<T> &second) {
        using std::swap;

        swap(impl_ptr, second.impl_ptr);
    }


    virtual ~ImplContainer() {}


    std::shared_ptr<T> impl() const {
        return impl_ptr;
    }

};


} // namespace base
} // namespace nix

#endif
