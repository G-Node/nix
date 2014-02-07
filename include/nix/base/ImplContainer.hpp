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

namespace nix {
namespace base {

template<typename T>
class ImplContainer {

protected:

    std::shared_ptr<T> impl_ptr;

	/**
	 * Get entities by index
	 * 
     * Get multiple entities of given type and return them as vector.
     * The template param specifies which type.
     * The parameter "getEnt" is the function used to get the entities 
     * of given type T_ENT and has to be of return type T_ENT with 
     * parameter "int" specifying the index of the entity to get.
     * The parameter "nT" should give the number of entities to get and
     * should be =>0 & <= the total number of existing entities.
     * The parameter "filter" is defaulted to giving back all entities 
     * of given type. To use your own filter pass a lambda that accepts 
     * an entity of given type as parameter and returns a bool telling 
     * whether to get it or not.
     * 
     * @param class "get function": std::function of return type T_ENT and 
     * 				param type "int" to get entities
     * @param int number of entities to get
     * @param class "filter function": std::function of return type bool
     * 				and param type T_ENT to filter which entities to get
     * @return class entities of the given type as a vector
     */
	template<typename TENT, typename TFUNC>
	std::vector<TENT> getMultiple(	
		TFUNC const &getEnt,
		int nT, 
		std::function<bool(TENT)> filter = [](TENT t){ return true; }) const 
	{
		std::vector<TENT> e;
		int i = 0;
		 
		if(nT < 1) { return e; }
		e.resize(nT);

		for (typename std::vector<TENT>::iterator it = e.begin(); it!=e.end(); ++it) {
			if(filter(*it)) {
				*it = getEnt( i++ );
			}
		}

		return e;
	}
	
	template<typename TENT>
	struct FLTR
	{
		typedef std::function<bool(TENT)> type;
	};
	
	template<typename T_ENT>
	static const std::function<bool(T_ENT)> fltr()
	{
		static std::function<bool(T_ENT)> const std = [](T_ENT t){ return true; };
		
		return std;
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


    virtual bool operator==(std::nullptr_t nullp) const {
        return impl_ptr == nullp;
    }

    virtual bool operator!=(std::nullptr_t nullp) const {
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
