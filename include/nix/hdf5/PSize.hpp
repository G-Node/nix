// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef PANDORA_PSIZE_H
#define PANDORA_PSIZE_H

#include <stdexcept>
#include <algorithm>
#include <initializer_list>

namespace nix {
namespace hdf5 {


template<typename T>
class PSizeBase {

public:

    typedef T        value_type;
    typedef T       *iterator;
    typedef const T *const_iterator;
    typedef T       &reference;
    typedef const T  const_reference;
    typedef T       *pointer;
    typedef size_t   difference_type;
    typedef size_t   size_type;

    PSizeBase()
        : rank(0), dims(nullptr)
    {
    }


    explicit PSizeBase(size_t _rank)
        : rank(_rank), dims(nullptr)
    {
        allocate();
    }


    explicit PSizeBase(size_t _rank, T fillValue)
        : rank(_rank), dims(nullptr)
    {
        allocate();
        fill(fillValue);
    }


    PSizeBase(std::initializer_list<T> args)
        : rank(args.size())
    {
        allocate();
        std::copy(args.begin(), args.end(), dims);
    }

    //copy
    PSizeBase(const PSizeBase &other)
        : rank(other.rank), dims(nullptr)
    {
        allocate();
        std::copy(other.dims, other.dims + rank, dims);
    }

    //move (not tested due to: http://llvm.org/bugs/show_bug.cgi?id=12208)
    PSizeBase(PSizeBase &&other)
        : rank(other.rank), dims(other.dims)
    {
        other.dims = nullptr;
        other.rank = 0;
    }

    //copy and move assignment operator (not tested, see above)
    PSizeBase& operator=(PSizeBase other) {
        swap(other);
        return *this;
    }


    T& operator[] (const size_t index) {
        const PSizeBase *this_const = const_cast<const PSizeBase*>(this);
        return const_cast<T&>(this_const->operator[](index));
    }


    const T& operator[] (const size_t index) const {
        if (index + 1 > rank) {
            throw std::out_of_range ("Index out of bounds");
        }
        return dims[index];
    }

    PSizeBase<T>& operator++() {
        std::for_each(begin(), end(), [](hsize_t &val) {
            val++;
        });
        return *this;
    }


    PSizeBase<T> operator++(int) {
        PSizeBase<T> snapshot(*this);
        operator++();
        return snapshot;
    }


    PSizeBase<T>& operator+=(const PSizeBase<T> &rhs) {
        if(size() != rhs.size()) {
            throw std::out_of_range (""); //fixme: use different exception
        }

        for (size_t i = 0; i < rank; i++) {
            dims[i] += rhs.dims[i];
        }

        return *this;
    }


    PSizeBase<T>& operator+=(T val) {
        for (size_t i = 0; i < rank; i++) {
            dims[i] += val;
        }

        return *this;
    }


    PSizeBase<T>& operator+=(int val) {
        return operator+=(static_cast<T>(val));
    }


    PSizeBase<T>& operator--() {
        std::for_each(begin(), end(), [](hsize_t &val) {
            val--;
        });
        return *this;
    }


    PSizeBase<T> operator--(int) {
        PSizeBase<T> snapshot(*this);
        operator--();
        return snapshot;
    }


    PSizeBase<T>& operator-=(const PSizeBase<T> &rhs) {
        if(size() != rhs.size()) {
            throw std::out_of_range (""); //fixme: use different exception
        }

        for (size_t i = 0; i < rank; i++) {
            dims[i] -= rhs.dims[i];
        }

        return *this;
    }


    PSizeBase<T>& operator-=(T val) {
        for (size_t i = 0; i < rank; i++) {
            dims[i] -= val;
        }

        return *this;
    }


    PSizeBase<T>& operator-=(int val) {
        return operator-=(static_cast<T>(val));
    }


    void swap(PSizeBase &other) {
        using std::swap;
        swap(dims, other.dims);
        rank = other.rank;
    }


    PSizeBase<T>& operator*=(const PSizeBase<T> &rhs) {
        if(size() != rhs.size()) {
            throw std::out_of_range (""); //fixme: use different exception
        }

        for (size_t i = 0; i < rank; i++) {
            dims[i] *= rhs.dims[i];
        }

        return *this;
    }


    PSizeBase<T>& operator/=(const PSizeBase<T> &rhs) {
        if(size() != rhs.size()) {
            throw std::out_of_range (""); //fixme: use different exception
        }

        for (size_t i = 0; i < rank; i++) {
            dims[i] /= rhs.dims[i];
        }

        return *this;
    }


    size_t size() const { return rank; }


    size_t nelms() const {
        size_t product = 1;
        std::for_each(begin(), end(), [&](hsize_t val) {
            product *= val;
        });
        return product;
    }


    size_t dot(const PSizeBase<T> &other) const {
        if(size() != other.size()) {
            throw std::out_of_range (""); //fixme: use different exception
        }

        size_t res  = 0;
        for (size_t i = 0; i < rank; i++) {
            res += dims[i] * other.dims[i];
        }

        return res;
    }


    T* data() { return dims; }


    const T* data() const {return dims; }


    void fill(T value) {
        std::fill_n(dims, rank, value);
    }


    ~PSizeBase(){
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
PSizeBase<T> operator-(PSizeBase<T> lhs, const PSizeBase<T> &rhs)
{
    lhs -= rhs;
    return lhs;
}


template<typename T>
PSizeBase<T> operator+(PSizeBase<T> lhs, const PSizeBase<T> &rhs)
{
    lhs += rhs;
    return lhs;
}


template<typename T>
PSizeBase<T> operator+(PSizeBase<T> lhs, T rhs)
{
    lhs += rhs;
    return lhs;
}


template<typename T>
PSizeBase<T> operator+(T lhs, const PSizeBase<T> &rhs)
{
    return operator+(rhs, lhs);
}


template<typename T>
PSizeBase<T> operator+(PSizeBase<T> lhs, int rhs)
{
    lhs += rhs;
    return lhs;
}


template<typename T>
PSizeBase<T> operator+(int lhs, const PSizeBase<T> &rhs)
{
    return operator+(rhs, lhs);
}

template<typename T>
PSizeBase<T> operator*(PSizeBase<T> lhs, const PSizeBase<T> &rhs)
{
    lhs *= rhs;
    return lhs;
}


template<typename T>
PSizeBase<T> operator*(PSizeBase<T> lhs, T rhs)
{
    lhs *= rhs;
    return lhs;
}


template<typename T>
PSizeBase<T> operator*(T lhs, const PSizeBase<T> &rhs)
{
    return operator*(rhs, lhs);
}


template<typename T>
PSizeBase<T> operator/(PSizeBase<T> lhs, const PSizeBase<T> &rhs)
{
    lhs /= rhs;
    return lhs;
}


template<typename T>
PSizeBase<T> operator/(PSizeBase<T> lhs, T rhs)
{
    lhs /= rhs;
    return lhs;
}


template<typename T>
PSizeBase<T> operator/(T lhs, const PSizeBase<T> &rhs)
{
    return operator/(rhs, lhs);
}


template<typename T>
inline bool operator==(const PSizeBase<T> &lhs, const PSizeBase<T> &rhs)
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
inline bool operator!=(const PSizeBase<T> &lhs, const PSizeBase<T> &rhs)
{
    return !operator==(lhs, rhs);
}


/* *****  */

typedef PSizeBase<hsize_t>  PSize;

typedef PSizeBase<hssize_t> PSSize;


} // namespace hdf5
} // namespace nix

#endif // PANDORA_PSIZE_H
