// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/WrapH5ID.hpp>
#include <H5Gpublic.h>


namespace nix {
namespace hdf5 {


WrapH5ID::WrapH5ID()
    : hid(H5I_INVALID_HID)
{}


WrapH5ID::WrapH5ID(hid_t id)
    : hid(id)
{
    inc();
}


WrapH5ID::WrapH5ID(const WrapH5ID &other)
    : hid(other.hid)
{
    inc();
}


WrapH5ID::WrapH5ID(WrapH5ID &&other) : hid(other.hid) {
    other.invalidate();
}


WrapH5ID& WrapH5ID::operator=(const WrapH5ID &other) {
    if (hid != other.hid) {
        dec();
        hid = other.hid;
        inc();
    }
    return *this;
}


WrapH5ID& WrapH5ID::operator=(WrapH5ID &&other) {
    hid = other.hid;
    other.invalidate();
    return *this;
}


bool WrapH5ID::operator==(const WrapH5ID &other) const {
    if (H5Iis_valid(hid) && H5Iis_valid(other.hid))
        return hid == other.hid;
    else
        return false;
}


bool WrapH5ID::operator!=(const WrapH5ID &other) const {
    return !(*this == other);
}


hid_t WrapH5ID::h5id() const {
    return hid;
}


int WrapH5ID::refCount() const {
    if (H5Iis_valid(hid)) {
        return H5Iget_ref(hid);
    } else {
        return -1;
    }
}


void WrapH5ID::close() {
    dec();
    invalidate();
}


WrapH5ID::~WrapH5ID() {
    close();
}


void WrapH5ID::inc() const {
    if (H5Iis_valid(hid)) {
        H5Iinc_ref(hid);
    }
}


void WrapH5ID::dec() const {
    if (H5Iis_valid(hid)) {
        H5Idec_ref(hid);
    }
}


void WrapH5ID::invalidate() {
    hid = H5I_INVALID_HID;
}

} // namespace hdf5
} // namespace nix
