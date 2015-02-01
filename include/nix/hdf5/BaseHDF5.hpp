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

#include <boost/optional.hpp>

#include <nix/Platform.hpp>
#include <nix/Hydra.hpp>
#include <nix/hdf5/hdf5include.hpp>


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
        buffer = new data_type[nelms];
    }

    data_ptr operator*() {
        return buffer;
    }

    void finish() {
        for (size_t i = 0; i < nelms; i++) {
            data[i] = buffer[i];
        }
    }

    ~StringWriter() {
        delete[] buffer;
    }

private:
    size_t   nelms;
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
        buffer = new data_type[nelms];
        for (size_t i = 0; i < nelms; i++) {
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
    size_t   nelms;
    pointer  data;
    data_ptr buffer;
};


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

    virtual void close();

    virtual ~BaseHDF5();

protected:

    void inc() const;

    void dec() const;

    void invalidate();
};


} // namespace hdf5
} // namespace nix

#endif /* NIX_WRAP_ID_H */
