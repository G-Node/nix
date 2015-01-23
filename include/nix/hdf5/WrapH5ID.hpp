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

class NIXAPI WrapH5ID {

protected:

    hid_t hid;

public:

    WrapH5ID();

    WrapH5ID(hid_t hid);

    WrapH5ID(const WrapH5ID &other);

    WrapH5ID(WrapH5ID &&other);

    WrapH5ID& operator=(const WrapH5ID &other);

    WrapH5ID& operator=(WrapH5ID &&other);

    bool operator==(const WrapH5ID &other) const;

    bool operator!=(const WrapH5ID &other) const;

    //NB: use the following functions with caution
    hid_t h5id() const; //no refcount increase

    int refCount() const;

    void close();

    virtual ~WrapH5ID();

protected:

    void inc() const;

    void dec() const;

    void invalidate();
};

} // namespace hdf5
} // namespace nix

#endif /* NIX_WRAP_ID_H */
