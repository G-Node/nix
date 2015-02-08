// Copyright © 2015 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include <nix/hdf5/Attribute.hpp>
#include <nix/hdf5/DataTypeHDF5.hpp>

namespace nix {
namespace hdf5 {


Attribute::Attribute() : BaseHDF5() {
}

Attribute::Attribute(hid_t hid) : BaseHDF5(hid) {
}

Attribute::Attribute(const Attribute &other) : BaseHDF5(other) {
}


void Attribute::read(h5x::DataType mem_type, const NDSize &size, void *data) {
    HErr status = H5Aread(hid, mem_type.h5id(), data);
    status.check("Attribute::read(): Could not read data");
}

void Attribute::read(h5x::DataType mem_type, const NDSize &size, std::string *data) {
    StringWriter writer(size, data);
    read(mem_type, size, *writer);
    writer.finish();

    DataSpace space = getSpace();

    HErr status = H5Dvlen_reclaim(mem_type.h5id(), space.h5id(), H5P_DEFAULT, *writer);
    status.check("Attribute::read(): Could not reclaim variable length data");
}

void Attribute::write(h5x::DataType mem_type, const NDSize &size, const void *data) {
    HErr status = H5Awrite(hid, mem_type.h5id(), data);
    status.check("Attribute::write(): Could not write data");
}

void Attribute::write(h5x::DataType mem_type, const NDSize &size, const std::string *data) {
    StringReader reader(size, data);
    write(mem_type, size, *reader);
}


DataSpace Attribute::getSpace() const {

    DataSpace space = H5Aget_space(hid);
    space.check("Attribute::getSpace(): Dould not get data space");
    return space;
}

NDSize Attribute::extent() const {
    return getSpace().extent();
}

} //nix::hdf5::
} //nix::