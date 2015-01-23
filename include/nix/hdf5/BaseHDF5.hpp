// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_WRAP_ID_H
#define NIX_WRAP_ID_H

#include <string>

#include <nix/hdf5/hdf5include.hpp>
#include <nix/Platform.hpp>

#include <boost/optional.hpp>

namespace nix {
namespace hdf5 {

class NIXAPI BaseHDF5 {

protected:

    hid_t hid;

public:

    BaseHDF5();

    BaseHDF5(hid_t hid);

    BaseHDF5(const BaseHDF5 &other);

    BaseHDF5(BaseHDF5 &&other);

    BaseHDF5& operator=(const BaseHDF5 &other);

    BaseHDF5& operator=(BaseHDF5 &&other);

    bool operator==(const BaseHDF5 &other) const;

    bool operator!=(const BaseHDF5 &other) const;

    //NB: use the following functions with caution
    hid_t h5id() const; //no refcount increase

    int refCount() const;

    void close();

    virtual ~BaseHDF5();

protected:

    void inc() const;

    void dec() const;

    void invalidate();
};

} // namespace hdf5
} // namespace nix

#endif /* NIX_WRAP_ID_H */
