// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "H5Object.hpp"
#include "H5Exception.hpp"


namespace nix {
namespace hdf5 {


H5Object::H5Object(const H5Object &other)
    : hid(other.hid)
{
    inc();
}


H5Object::H5Object(H5Object &&other) : hid(other.hid) {
    other.invalidate();
}


H5Object &H5Object::operator=(const H5Object &other) {
    if (hid != other.hid) {
        dec();
        hid = other.hid;
        inc();
    }
    return *this;
}


H5Object &H5Object::operator=(H5Object &&other) {
    hid = other.hid;
    other.invalidate();
    return *this;
}


bool H5Object::operator==(const H5Object &other) const {
    if (H5Iis_valid(hid) && H5Iis_valid(other.hid))
        return hid == other.hid;
    else
        return false;
}


bool H5Object::operator!=(const H5Object &other) const {
    return !(*this == other);
}


hid_t H5Object::h5id() const {
    return hid;
}


int H5Object::refCount() const {
    if (H5Iis_valid(hid)) {
        return H5Iget_ref(hid);
    } else {
        return -1;
    }
}

bool H5Object::isValid() const {
    HTri res = H5Iis_valid(hid);
    res.check("H5Object::isValid() failed");
    return res.result();
}

std::string H5Object::name() const {
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


H5I_type_t H5Object::type() const {
    return H5Iget_type(hid);
}

void H5Object::close() {
    dec();
    invalidate();
}


H5Object::~H5Object() {
    H5Object::close();
}


void H5Object::inc() const {
    if (H5Iis_valid(hid)) {
        H5Iinc_ref(hid);
    }
}


void H5Object::dec() const {
    if (H5Iis_valid(hid)) {
        H5Idec_ref(hid);
    }
}


void H5Object::invalidate() {
    hid = H5I_INVALID_HID;
}

} // namespace hdf5
} // namespace nix
