// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>


#include <nix/DataView.hpp>
#include <nix/Exception.hpp>

namespace nix {

void DataView::dataExtent(const NDSize &extent) {
    throw std::runtime_error("Not allowed!");
}

NDSize DataView::dataExtent() const {
    return count;
}

NDSize DataView::transform_coordinates(const NDSize &cnt, const NDSize &off) const {

    if (!off) {
        //maybe NDSize should accept "x + {}" as valid
        if (cnt > count) {
            throw OutOfBounds("Trying to access data outside of range", 0);
        }

        return offset;

    } else {
        if (cnt + off > count) {
            throw OutOfBounds("Trying to access data outside of range", 0);
        }

        return offset + off;
    }
}

void DataView::ioRead(DataType dtype, void *data, const NDSize &count, const NDSize &offset) const {

    const NDSize &real_count =  count ? count : this->count;
    NDSize base = transform_coordinates(real_count, offset);
    array.getData(dtype, data, real_count, base);
}

void DataView::ioWrite(DataType dtype, const void *data, const NDSize &count, const NDSize &offset) {

    const NDSize &real_count =  count ? count : this->count;
    NDSize base = transform_coordinates(real_count, offset);
    array.setData(dtype, data, real_count, base);
}

DataType DataView::dataType() const {
    return array.dataType();
}

}