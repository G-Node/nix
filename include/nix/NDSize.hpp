// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_PSIZE_H
#define NIX_PSIZE_H

#include <nix/Platform.hpp>
#include <nix/Exception.hpp>

#include <cstdint>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <vector>

namespace nix {

//Ideally we would use unit64_t (and int64_t) here to directly specify
//the size we want, but for now we stick with how the hdf5 library
//defines hsize_t, otherwise we will run into issues when on plaforms
// where unit64_t is an incompatible type to the type of hsize_t
//(e.g. Ubuntu 12.04 LTS Server Edition 64 bit.)

typedef unsigned long long int ndsize_t;
typedef long long int          ndssize_t;


#ifdef _MSC_VER
// This is a workaround for MVSC that spits out warnings for
// unsafe operations when using copy_n, fill_n and raw pointers
// To avoid this we implement the following functions in NDSize.cpp
// and disable the correspoding warning (C4996) for <algorithm>

NIXAPI void nd_fill(ndsize_t *data, size_t len, ndsize_t value);
NIXAPI void nd_fill(ndssize_t *data, size_t len, ndssize_t value);

NIXAPI void nd_copy(ndsize_t *source, size_t n, ndsize_t *dest);
NIXAPI void nd_copy(ndssize_t *source, size_t n, ndssize_t *dest);

#else

#define nd_fill std::fill_n
#define nd_copy std::copy_n

#endif

template<typename T>
class NDSizeBase {

public:

    typedef T        value_type;
    typedef T       *iterator;
    typedef const T *const_iterator;
    typedef T       &reference;
    typedef const T  const_reference;
    typedef T       *pointer;
    typedef size_t   difference_type;
    typedef size_t   size_type;

    NDSizeBase()
        : rank(0), dims(nullptr)
    {
    }


    explicit NDSizeBase(size_t rank)
        : rank(rank), dims(nullptr)
    {
        allocate();
    }


    explicit NDSizeBase(size_t rank, T fill_value)
        : rank(rank), dims(nullptr)
    {
        allocate();
        fill(fill_value);
    }

    template<typename U>
    NDSizeBase(std::initializer_list<U> args)
        : rank(args.size())
    {
        allocate();

        #ifdef _MSC_VER
        //std::transform triggers C4996, see nd_copy, nd_fill above
        std::vector<U> u = args;
        for (size_t i = 0; i < rank; i++) {
            dims[i] = static_cast<T>(u[i]);
        }
        #else
        std::transform(args.begin(), args.end(), dims,
                       [](const U& val) {
                           return static_cast<T>(val);
                       });
        #endif
    }

    template<typename U>
    NDSizeBase(const std::vector<U> &args)
        : rank(args.size())
    {
        allocate();

        #ifdef _MSC_VER
        //std::transform triggers C4996, see nd_copy, nd_fill above
        for (size_t i = 0; i < rank; i++) {
            dims[i] = static_cast<T>(args[i]);
        }
        #else
        std::transform(args.begin(), args.end(), dims,
                       [](const U& val) {
                           return static_cast<T>(val);
                       });
        #endif
    }

    //copy
    NDSizeBase(const NDSizeBase &other)
        : rank(other.rank), dims(nullptr)
    {
        allocate();
        nd_copy(other.dims, rank, dims);
    }

    //move (not tested due to: http://llvm.org/bugs/show_bug.cgi?id=12208)
    NDSizeBase(NDSizeBase &&other)
        : rank(other.rank), dims(other.dims)
    {
        other.dims = nullptr;
        other.rank = 0;
    }

    //copy and move assignment operator (not tested, see above)
    NDSizeBase& operator=(NDSizeBase other) {
        swap(other);
        return *this;
    }

    // safe bool of the future (i.e. C++11)
    explicit operator bool() const {
        return rank > 0;
    }

    T& operator[] (const size_t index) {
        const NDSizeBase *this_const = const_cast<const NDSizeBase*>(this);
        return const_cast<T&>(this_const->operator[](index));
    }


    const T& operator[] (const size_t index) const {
        if (index + 1 > rank) {
            throw std::out_of_range ("Index out of bounds");
        }
        return dims[index];
    }

    NDSizeBase<T>& operator++() {
        std::for_each(begin(), end(), [](T &val) {
            val++;
        });
        return *this;
    }


    NDSizeBase<T> operator++(int) {
        NDSizeBase<T> snapshot(*this);
        operator++();
        return snapshot;
    }


    NDSizeBase<T>& operator+=(const NDSizeBase<T> &rhs) {
        if(size() != rhs.size()) {
            throw std::out_of_range (""); //fixme: use different exception
        }

        for (size_t i = 0; i < rank; i++) {
            dims[i] += rhs.dims[i];
        }

        return *this;
    }


    NDSizeBase<T>& operator+=(T val) {
        for (size_t i = 0; i < rank; i++) {
            dims[i] += val;
        }

        return *this;
    }


    NDSizeBase<T>& operator+=(int val) {
        return operator+=(static_cast<T>(val));
    }


    NDSizeBase<T>& operator--() {
        std::for_each(begin(), end(), [](T &val) {
            val--;
        });
        return *this;
    }


    NDSizeBase<T> operator--(int) {
        NDSizeBase<T> snapshot(*this);
        operator--();
        return snapshot;
    }


    NDSizeBase<T>& operator-=(const NDSizeBase<T> &rhs) {
        if(size() != rhs.size()) {
            throw std::out_of_range (""); //fixme: use different exception
        }

        for (size_t i = 0; i < rank; i++) {
            dims[i] -= rhs.dims[i];
        }

        return *this;
    }


    NDSizeBase<T>& operator-=(T val) {
        for (size_t i = 0; i < rank; i++) {
            dims[i] -= val;
        }

        return *this;
    }


    NDSizeBase<T>& operator-=(int val) {
        return operator-=(static_cast<T>(val));
    }


    void swap(NDSizeBase &other) {
        using std::swap;
        swap(dims, other.dims);
        rank = other.rank;
    }


    NDSizeBase<T>& operator*=(const NDSizeBase<T> &rhs) {
        if(size() != rhs.size()) {
            throw std::out_of_range (""); //fixme: use different exception
        }

        for (size_t i = 0; i < rank; i++) {
            dims[i] *= rhs.dims[i];
        }

        return *this;
    }


    NDSizeBase<T>& operator/=(const NDSizeBase<T> &rhs) {
        if(size() != rhs.size()) {
            throw std::out_of_range (""); //fixme: use different exception
        }

        for (size_t i = 0; i < rank; i++) {
            dims[i] /= rhs.dims[i];
        }

        return *this;
    }


    size_t size() const { return rank; }


    T nelms() const {
        T product = 1;
        //TODO: check for "overflow" in calculations
		std::for_each(begin(), end(), [&](T val) {
            product *= val;
        });

        return product;
    }


    T dot(const NDSizeBase<T> &other) const {
        if(size() != other.size()) {
            throw std::out_of_range ("Dimensions do not match"); //fixme: use different exception
        }

        T res  = 0;
        for (size_t i = 0; i < rank; i++) {
            res += dims[i] * other.dims[i];
        }

        return res;
    }


    T* data() { return dims; }


    const T* data() const {return dims; }


    void fill(T value) {
        nd_fill(dims, rank, value);
    }


    ~NDSizeBase() {
        delete[] dims;
    }


    //we are modelling a boost::Collection
    iterator begin() { return dims; }


    iterator end() { return dims + rank; }


    const_iterator begin() const { return dims; }


    const_iterator end() const { return dims + rank; }


    bool empty() const { return rank == 0; }

private:

    void allocate() {
        if (rank > 0) {
            dims = new T[rank];
        }
    }

    size_t   rank;
    T *dims;
};


template<typename T>
NDSizeBase<T> operator-(NDSizeBase<T> lhs, const NDSizeBase<T> &rhs)
{
    lhs -= rhs;
    return lhs;
}


template<typename T>
NDSizeBase<T> operator+(NDSizeBase<T> lhs, const NDSizeBase<T> &rhs)
{
    lhs += rhs;
    return lhs;
}


template<typename T>
NDSizeBase<T> operator+(NDSizeBase<T> lhs, T rhs)
{
    lhs += rhs;
    return lhs;
}


template<typename T>
NDSizeBase<T> operator+(T lhs, const NDSizeBase<T> &rhs)
{
    return operator+(rhs, lhs);
}


template<typename T>
NDSizeBase<T> operator+(NDSizeBase<T> lhs, int rhs)
{
    lhs += rhs;
    return lhs;
}


template<typename T>
NDSizeBase<T> operator+(int lhs, const NDSizeBase<T> &rhs)
{
    return operator+(rhs, lhs);
}

template<typename T>
NDSizeBase<T> operator*(NDSizeBase<T> lhs, const NDSizeBase<T> &rhs)
{
    lhs *= rhs;
    return lhs;
}


template<typename T>
NDSizeBase<T> operator*(NDSizeBase<T> lhs, T rhs)
{
    lhs *= rhs;
    return lhs;
}


template<typename T>
NDSizeBase<T> operator*(T lhs, const NDSizeBase<T> &rhs)
{
    return operator*(rhs, lhs);
}


template<typename T>
NDSizeBase<T> operator/(NDSizeBase<T> lhs, const NDSizeBase<T> &rhs)
{
    lhs /= rhs;
    return lhs;
}


template<typename T>
NDSizeBase<T> operator/(NDSizeBase<T> lhs, T rhs)
{
    lhs /= rhs;
    return lhs;
}


template<typename T>
NDSizeBase<T> operator/(T lhs, const NDSizeBase<T> &rhs)
{
    return operator/(rhs, lhs);
}


template<typename T>
inline bool operator==(const NDSizeBase<T> &lhs, const NDSizeBase<T> &rhs)
{
    if (lhs.size() != rhs.size())
        return false;

    for (size_t i = 0; i < lhs.size(); i++) {
        if (lhs[i] != rhs[i])
            return false;
    }

    return true;
}


template<typename T>
inline bool operator!=(const NDSizeBase<T> &lhs, const NDSizeBase<T> &rhs)
{
    return !operator==(lhs, rhs);
}


template<typename T>
inline bool operator<(const NDSizeBase<T> &lhs, const NDSizeBase<T> &rhs)
{
    if (lhs.size() != rhs.size()) {
        throw IncompatibleDimensions("size must agree to compare",
                                     "NDSizeBase < NDSizeBase ");
    }

    const size_t size = lhs.size();
    for (size_t i = 0; i < size; i++) {
        if (lhs[i] >= rhs[i]) {
            return false;
        }
    }

    return true;
}

template<typename T>
inline bool operator<=(const NDSizeBase<T> &lhs, const NDSizeBase<T> &rhs)
{
    if (lhs.size() != rhs.size()) {
        throw IncompatibleDimensions("size must agree to compare",
                                     "NDSizeBase < NDSizeBase ");
    }

    const size_t size = lhs.size();
    for (size_t i = 0; i < size; i++) {
        if (lhs[i] > rhs[i]) {
            return false;
        }
    }

    return true;
}

template<typename T>
inline bool operator>(const NDSizeBase<T> &lhs, const NDSizeBase<T> &rhs)
{
    return !(lhs <= rhs);
}

template<typename T>
inline bool operator>=(const NDSizeBase<T> &lhs, const NDSizeBase<T> &rhs)
{
    return !(lhs < rhs);
}

template<typename T>
inline std::ostream& operator<<(std::ostream &os, const NDSizeBase<T> &ndsize)
{
  os << "NDSize {";
  for(size_t i = 0; i < ndsize.size(); i++) {
    if (i != 0) {
      os << ", ";
    }

    os << ndsize[i];
  }

  os << "}\n";
  return os;
}


typedef NDSizeBase<ndsize_t>  NDSize;

typedef NDSizeBase<ndssize_t> NDSSize;


} // namespace nix

#endif // NIX_PSIZE_H
