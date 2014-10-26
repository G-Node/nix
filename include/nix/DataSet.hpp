// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>


#ifndef NIX_DATA_IO_H
#define NIX_DATA_IO_H

#include <boost/multi_array.hpp>
#include <nix/Dimensions.hpp>
#include <nix/Hydra.hpp>

#include <nix/Platform.hpp>

namespace nix {

class NIXAPI DataSet {
public:
    template<typename T> void getData(T &value) const;

    template<typename T> void setData(const T &value);

    template<typename T> void getData(T &value, const NDSize &count, const NDSize &offset) const;

    template<typename T> void getData(T &value, const NDSize &offset) const;

    template<typename T> void setData(const T &value, const NDSize &offset);


    void getData(DataType dtype,
                         void *data,
                         const NDSize &count,
                         const NDSize &offset) const {
        ioRead(dtype, data, count, offset);
    }

    void setData(DataType dtype,
                         const void *data,
                         const NDSize &count,
                         const NDSize &offset) {
        ioWrite(dtype, data, count, offset);
    }

    // *** the virtual interface ***
    virtual void dataExtent(const NDSize &extent) = 0;
    virtual NDSize dataExtent() const = 0;


protected:
    virtual void ioRead(DataType dtype,
                        void *data,
                        const NDSize &count,
                        const NDSize &offset) const = 0;

    virtual void ioWrite(DataType dtype,
                         const void *data,
                         const NDSize &count,
                         const NDSize &offset) = 0;

};

template<typename T>
void DataSet::getData(T &value) const
{
    Hydra<T> hydra(value);

    NDSize extent = dataExtent();
    hydra.resize(extent);

    DataType dtype = hydra.element_data_type();
    NDSize shape = hydra.shape();

    getData(dtype, hydra.data(), shape, {});
}

template<typename T>
void DataSet::setData(const T &value)
{
    const Hydra<const T> hydra(value);

    DataType dtype = hydra.element_data_type();
    NDSize shape = hydra.shape();

    dataExtent(shape);
    setData(dtype, hydra.data(), shape, {});
}

template<typename T>
void DataSet::getData(T &value, const NDSize &count, const NDSize &offset) const
{
    Hydra<T> hydra(value);
    DataType dtype = hydra.element_data_type();

    hydra.resize(count);
    getData(dtype, hydra.data(), count, offset);
}

template<typename T>
void DataSet::getData(T &value, const NDSize &offset) const
{
    Hydra<T> hydra(value);
    DataType dtype = hydra.element_data_type();

    NDSize count = hydra.shape();
    if (! count) {
        count = NDSize(offset.size(), 1);
    }
    getData(dtype, hydra.data(), count, offset);
}


template<typename T>
void DataSet::setData(const T &value, const NDSize &offset)
{
    const Hydra<const T> hydra(value);

    DataType dtype = hydra.element_data_type();
    NDSize shape = hydra.shape();

    setData(dtype, hydra.data(), shape, offset);
}

}

#endif
