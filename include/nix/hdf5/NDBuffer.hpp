// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
// Copyright (c) 2013, Christian Kellner <kellner@bio.lmu.de>
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_NDBUFFER_H
#define NIX_NDBUFFER_H

#include <vector>
#include <iostream>

#include <nix/hdf5/Charon.hpp>

namespace nix {
namespace hdf5 {


class NDBuffer {

public:

    typedef uint8_t byte_type;

    NDBuffer(DataType dtype, NDSize dims);

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
const T NDBuffer::get(size_t index) const
{
    const T *dx = reinterpret_cast<const T *>(&dstore[0]);
    return dx[index];
}


template<typename T>
const T NDBuffer::get(const NDSize &index) const
{
    size_t pos = sub2index(index);
    const T *dx = reinterpret_cast<const T *>(&dstore[0]);
    return dx[pos];
}


template<typename T>
void NDBuffer::set(size_t index, T value)
{
    T* dx = reinterpret_cast<T *>(&dstore[0]);
    dx[index] = value;
}


template<typename T>
void NDBuffer::set(const NDSize &index, T value)
{
    size_t pos = sub2index(index);
    T* dx = reinterpret_cast<T *>(&dstore[0]);
    dx[pos] = value;
}

/* ****************************************** */

namespace hades {

template<>
class TypeInfo<NDBuffer> {
public:
    typedef uint8_t element_type;
    typedef TypeSpec<DataType> spec_type;

    static spec_type type_spec(const NDBuffer &value) { return spec_type(value.dtype()); };

    static NDSize shape(const NDBuffer &value) { return value.shape(); }

    static size_t num_elements(const NDBuffer &value) {
        return value.num_elements();
    }

    static const element_type* getData(const NDBuffer &value) {
        return value.data();
    }

    static element_type* getData(NDBuffer &value) {
        return value.data();
    }
};


} // namespace hades
} // namespace hdf5
} // namespace nix

#endif // NIX_NDBUFFER_H
