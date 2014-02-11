// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
// Copyright (c) 2013, Christian Kellner <kellner@bio.lmu.de>
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_NDARRAY_H
#define NIX_NDARRAY_H

#include <vector>
#include <iostream>

#include <nix/Hydra.hpp>

#include <nix/Platform.hpp>

namespace nix {

class NIXAPI NDArray {

public:

    typedef uint8_t byte_type;

    NDArray(DataType dtype, NDSize dims);

    size_t rank() const { return extends.size(); }
    size_t num_elements() const { return extends.nelms(); }
    NDSize  shape() const { return extends; }
    NDSize  size()const { return extends; }
    DataType dtype() const { return dataType ;}

    template<typename T> const T get(size_t index) const;
    template<typename T> const T get(const NDSize &index) const;
    template<typename T> void set(size_t index, T value);
    template<typename T> void set(const NDSize &index, T value);

    byte_type *data() { return &dstore[0]; }
    const byte_type *data() const { return &dstore[0]; }

    void resize(const NDSize &new_size);

    size_t sub2index(const NDSize &sub) const;

private:

    DataType  dataType;
    void allocate_space();
    void calc_strides();

    NDSize                  extends;
    NDSize                  strides;
    std::vector<byte_type> dstore;

};

/* ******************************************* */

template<typename T>
const T NDArray::get(size_t index) const
{
    const T *dx = reinterpret_cast<const T *>(&dstore[0]);
    return dx[index];
}


template<typename T>
const T NDArray::get(const NDSize &index) const
{
    size_t pos = sub2index(index);
    const T *dx = reinterpret_cast<const T *>(&dstore[0]);
    return dx[pos];
}


template<typename T>
void NDArray::set(size_t index, T value)
{
    T* dx = reinterpret_cast<T *>(&dstore[0]);
    dx[index] = value;
}


template<typename T>
void NDArray::set(const NDSize &index, T value)
{
    size_t pos = sub2index(index);
    T* dx = reinterpret_cast<T *>(&dstore[0]);
    dx[pos] = value;
}

/* ****************************************** */

template<>
struct data_traits<NDArray> {

    typedef NDArray        value_type;
    typedef NDArray&       reference;
    typedef const NDArray& const_reference;

    typedef uint8_t        element_type;
    typedef uint8_t*       element_pointer;
    typedef const uint8_t* const_element_pointer;

    static DataType data_type(const_reference value) {
        return value.dtype();
    }

    static NDSize shape(const_reference value) {
        return value.shape();
    }

    static size_t num_elements(const_reference value) {
        return value.num_elements();
    }

    static const_element_pointer get_data(const_reference value) {
        return value.data();
    }

    static element_pointer get_data(reference value) {
        return value.data();
    }

    static void resize(reference value, const NDSize &dims) {
        value.resize(dims);
    }

};


} // namespace nix

#endif // NIX_NDARRAY_H
