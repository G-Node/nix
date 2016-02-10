// Copyright © 2015 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Adrian Stoewer <adrian.stoewer@rz.ifi.lmu.de>
//         Christian Kellner <kellner@bio.lmu.de>

#ifndef NIX_WRAP_ID_H
#define NIX_WRAP_ID_H

#include <nix/Platform.hpp>
#include <nix/Hydra.hpp>
#include "H5Exception.hpp"

#include <string>
#include <boost/optional.hpp>

namespace nix {
namespace hdf5 {


class StringWriter {
public:
    typedef std::string  value_type;
    typedef value_type  *pointer;
    typedef char        *data_type;
    typedef data_type   *data_ptr;


    StringWriter(const NDSize &size, pointer stringdata)
            : nelms(size.nelms()), data(stringdata) {
		size_t bs = nix::check::fits_in_size_t(nelms,
                         "Cannot allocate storage (exceeds memory)");
        buffer = new data_type[bs];
    }

    data_ptr operator*() {
        return buffer;
    }

    void finish() {
        for (ndsize_t i = 0; i < nelms; i++) {
            data[i] = buffer[i];
        }
    }

    ~StringWriter() {
        delete[] buffer;
    }

private:
    ndsize_t nelms;
    pointer  data;
    data_ptr buffer;
};

class StringReader {
public:
    typedef const std::string   value_type;
    typedef value_type         *pointer;
    typedef const char         *data_type;
    typedef data_type          *data_ptr;


    StringReader(const NDSize &size, pointer stringdata)
            : nelms(size.nelms()), data(stringdata) {
		size_t bs = nix::check::fits_in_size_t(nelms,
                         "Cannot allocate storage (exceeds memory)");
        buffer = new data_type[bs];
        for (ndsize_t i = 0; i < bs; i++) {
            buffer[i] = data[i].c_str();
        }
    }

    data_ptr operator*() {
        return buffer;
    }

    ~StringReader() {
        delete[] buffer;
    }

private:
    ndsize_t   nelms;
    pointer  data;
    data_ptr buffer;
};


class NIXAPI H5Object {

protected:

    hid_t hid;

public:

    H5Object() : hid(H5I_INVALID_HID) { }

    H5Object(hid_t hid) : H5Object(hid, false) { };

    H5Object(hid_t hid, bool is_copy) : hid(hid) {
        if (is_copy) {
            inc();
        }
    }

    H5Object(const H5Object &other);

    H5Object(H5Object &&other);

    H5Object & operator=(const H5Object &other);

    H5Object & operator=(H5Object &&other);

    bool operator==(const H5Object &other) const;

    bool operator!=(const H5Object &other) const;

    //NB: use the following functions with caution
    hid_t h5id() const; //no refcount increase

    int refCount() const;

    bool isValid() const;

    void check(const std::string &msg_if_fail) {
        if (!isValid()) {
            throw H5Exception(msg_if_fail);
        }
    }

    std::string name() const;

    H5I_type_t type() const;

    virtual void close();

    virtual ~H5Object();

protected:

    void inc() const;

    void dec() const;

    void invalidate();
};


struct NIXAPI HTri {
    typedef htri_t value_type;

    HTri(value_type result) : value(result) {}

    inline bool result() {
        return value > 0;
    }

    inline bool isError() {
        return value < 0;
    }

    explicit operator bool() {
        return result();
    }

    inline bool check(const std::string &msg) {
        if (value < 0) {
            throw H5Exception(msg);
        }

        return result();
    }

    value_type value;
};

struct NIXAPI HErr {
    typedef herr_t value_type;

    HErr() : value(static_cast<value_type>(-1)) { }
    HErr(value_type result) : value(result) {}

    inline bool isError() {
        return value < 0;
    }

    explicit operator bool() {
        return !isError();
    }

    inline bool check(const std::string &msg) {
        if (isError()) {
            throw H5Error(value, msg);
        }

        return true;
    }

    value_type value;
};


} // namespace hdf5
} // namespace nix

#endif /* NIX_WRAP_ID_H */
