// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef PANDORA_PSIZE_H
#define PANDORA_PSIZE_H

#include <cstdint>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>
#include <iostream>


namespace nix {


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


    explicit NDSizeBase(size_t _rank)
        : rank(_rank), dims(nullptr)
    {
        allocate();
    }


    explicit NDSizeBase(size_t _rank, T fillValue)
        : rank(_rank), dims(nullptr)
    {
        allocate();
        fill(fillValue);
    }


    NDSizeBase(std::initializer_list<T> args)
        : rank(args.size())
    {
        allocate();
        std::copy(args.begin(), args.end(), dims);
    }

    //copy
    NDSizeBase(const NDSizeBase &other)
        : rank(other.rank), dims(nullptr)
    {
        allocate();
        std::copy(other.dims, other.dims + rank, dims);
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


    size_t nelms() const {
        size_t product = 1;
        std::for_each(begin(), end(), [&](T val) {
            product *= val;
        });
        return product;
    }


    size_t dot(const NDSizeBase<T> &other) const {
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


    ~NDSizeBase(){
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

/* *****  */

//Ideally we would use unit64_t (and int64_t) here to directly specify
//the size we want, but for now we stick with how the hdf5 library
//defines hsize_t, otherwise we will run into issues when on plaforms
// where unit64_t is an incompatible type to the type of hsize_t
//(e.g. Ubuntu 12.04 LTS Server Edition 64 bit.)

typedef NDSizeBase<unsigned long long int>  NDSize;

typedef NDSizeBase<long long int> NDSSize;

} // namespace nix

#endif // PANDORA_PSIZE_H
