// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/BaseHDF5.hpp>
#include <H5ACpublic.h>
#include <H5Gpublic.h>
#include <nix/hdf5/ExceptionHDF5.hpp>


namespace nix {
namespace hdf5 {


BaseHDF5::BaseHDF5(const BaseHDF5 &other)
    : hid(other.hid)
{
    inc();
}


BaseHDF5::BaseHDF5(BaseHDF5 &&other) : hid(other.hid) {
    other.invalidate();
}


BaseHDF5& BaseHDF5::operator=(const BaseHDF5 &other) {
    if (hid != other.hid) {
        dec();
        hid = other.hid;
        inc();
    }
    return *this;
}


BaseHDF5& BaseHDF5::operator=(BaseHDF5 &&other) {
    hid = other.hid;
    other.invalidate();
    return *this;
}


bool BaseHDF5::operator==(const BaseHDF5 &other) const {
    if (H5Iis_valid(hid) && H5Iis_valid(other.hid))
        return hid == other.hid;
    else
        return false;
}


bool BaseHDF5::operator!=(const BaseHDF5 &other) const {
    return !(*this == other);
}


hid_t BaseHDF5::h5id() const {
    return hid;
}


int BaseHDF5::refCount() const {
    if (H5Iis_valid(hid)) {
        return H5Iget_ref(hid);
    } else {
        return -1;
    }
}


std::string BaseHDF5::name() const {
    if (! H5Iis_valid(hid)) {
        //maybe throw an exception?
        return "";
    }

    ssize_t len = H5Iget_name(hid, nullptr, 0);

    if (len < 0) {
        throw H5Exception("Could not get size of name");
    }

    std::vector<char> buffer(static_cast<size_t>(len + 1), 0);
    len = H5Iget_name(hid, buffer.data(), buffer.size());

    if (len < 0) {
        throw H5Exception("Could not obtain name");
    }

    std::string name =  std::string(buffer.data());
    return name;
}


void BaseHDF5::close() {
    dec();
    invalidate();
}


BaseHDF5::~BaseHDF5() {
    close();
}


void BaseHDF5::inc() const {
    if (H5Iis_valid(hid)) {
        H5Iinc_ref(hid);
    }
}


void BaseHDF5::dec() const {
    if (H5Iis_valid(hid)) {
        H5Idec_ref(hid);
    }
}


void BaseHDF5::invalidate() {
    hid = H5I_INVALID_HID;
}

} // namespace hdf5
} // namespace nix
