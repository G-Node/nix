// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_VALUE_HDF5_H
#define NIX_VALUE_HDF5_H

#include <string>

#include <nix.hpp>
#include <nix/hdf5/hdf5include.hpp>
#include <nix/hdf5/Charon.hpp>

namespace nix {
namespace hdf5 {


template<typename T>
struct FileValue {
    T value;
    double uncertainty;
    char *reference;
    char *filename;
    char *encoder;
    char *checksum;
};

template<typename T>
struct ValueHDF5 {
    T value;
    double uncertainty;
    std::string reference;
    std::string filename;
    std::string encoder;
    std::string checksum;

    ValueHDF5() {
    }

    ValueHDF5(const T &val) :
        value(val) {
    }

    ValueHDF5(const T &val, double uc, std::string rf, std::string fn, std::string en, std::string ck) :
        value(val), uncertainty(uc), reference(rf), filename(fn), encoder(en), checksum(ck) {
    }

    ValueHDF5(const FileValue<T> &that) :
        value(that.value), uncertainty(that.uncertainty), reference(that.reference), filename(that.filename),
        encoder(that.encoder), checksum(that.checksum) {
    }

    template<typename U>
    ValueHDF5& operator=(const FileValue<U> &that) {
        value = that.value;
        uncertainty = that.uncertainty;
        filename = that.filename;
        reference = that.reference;
        encoder = that.encoder;
        checksum = that.checksum;
        return *this;
    }

    template<typename U>
    FileValue<U> toValueBase(U) const {
        FileValue<U> base;
        base.filename = const_cast<char *> (filename.c_str());
        base.encoder = const_cast<char *> (encoder.c_str());
        base.checksum = const_cast<char *> (checksum.c_str());
        base.reference = const_cast<char *> (reference.c_str());
        base.value = value;
        return base;
    }
};

template<> template<typename U>
FileValue<U> ValueHDF5<std::string>::toValueBase(const U) const {
    FileValue<U> base;
    base.filename = const_cast<char *> (filename.c_str());
    base.encoder = const_cast<char *> (encoder.c_str());
    base.checksum = const_cast<char *> (checksum.c_str());
    base.reference = const_cast<char *> (reference.c_str());
    base.value = const_cast<char *> (value.c_str());
    return base;
}

template<> template<typename U>
FileValue<U> ValueHDF5<bool>::toValueBase(const U) const {
    FileValue<U> base;
    base.filename = const_cast<char *> (filename.c_str());
    base.encoder = const_cast<char *> (encoder.c_str());
    base.checksum = const_cast<char *> (checksum.c_str());
    base.reference = const_cast<char *> (reference.c_str());
    if (value)
        base.value = (int8_t) 1;
    else
        base.value = (int8_t) 0;
    return base;
}

typedef ValueHDF5<std::string> StringValue;
typedef ValueHDF5<double> DoubleValue;
typedef ValueHDF5<int64_t> LongValue;
typedef ValueHDF5<int32_t> IntValue;
typedef ValueHDF5<bool> BoolValue;

/* **** */
namespace hades {

template<typename T>
H5::DataType ValueToMemtype() {
    TypeSpec<T> spec;
    H5::CompType memtype(sizeof(FileValue<typename TypeSpec<T>::inner_type> ));
    memtype.insertMember("value", HOFFSET(ValueHDF5<T>, value), spec.memType);

    memtype.insertMember("uncertainty", HOFFSET(ValueHDF5<T>, uncertainty),
                         H5::PredType::NATIVE_DOUBLE);
    memtype.insertMember("reference", HOFFSET(ValueHDF5<T>, reference), H5::StrType(
                             H5::PredType::C_S1, H5T_VARIABLE));
    memtype.insertMember("filename", HOFFSET(ValueHDF5<T>, filename), H5::StrType(H5::PredType::C_S1,
                                                                              H5T_VARIABLE));
    memtype.insertMember("encoder", HOFFSET(ValueHDF5<T>, encoder), H5::StrType(H5::PredType::C_S1,
                                                                            H5T_VARIABLE));
    memtype.insertMember("checksum", HOFFSET(ValueHDF5<T>, checksum), H5::StrType(H5::PredType::C_S1,
                                                                              H5T_VARIABLE));
    return memtype;
}

template<typename T>
struct TypeSpec<ValueHDF5<T> > {

    static const bool is_valid = true;
    typedef typename TypeSpec<T>::inner_type inner_type;

    TypeSpec() :
        fileType(ValueToMemtype<T> ()), memType(ValueToMemtype<T> ()) {
    }

    const H5::DataType fileType;
    const H5::DataType memType;

};


template<typename T>
class TypeInfo<std::vector<ValueHDF5<T> > > {

public:

    typedef ValueHDF5<T> element_type;
    typedef std::vector<ValueHDF5<T> > myType;
    typedef TypeSpec<element_type> spec_type;

    static spec_type type_spec(const myType &value) { return spec_type(); };

    static NDSize shape(const myType &value) {
        return  NDSize( {value.size()});}

    static size_t num_elements(const myType &value) {
        return value.size();
    }

    static const element_type* getData(const myType &value) {
        return &value[0];
    }

    static element_type* getData(myType &value) {
        return &value[0];
    }

    static void resize(myType &value, const NDSize &dims) {
        if (dims.size() != 1) {
            throw InvalidRankException("Cannot resize scalar");
        }
        value.resize(dims[0]);
    }
};


template<typename Inner, typename Outer>
Inner get_inner(const Outer &outer) {
    return outer;
}

template<> char *get_inner<char*, std::string>(const std::string &outer);
template<> int8_t get_inner<int8_t, bool>(const bool &outer);

template<
        typename T,
        typename U
        >
class DataBox<T, ValueHDF5<U>, typename std::enable_if<std::is_const<T>::value>::type> {

public:

    typedef typename std::remove_const<T>::type      vanilla;
    typedef hades::TypeInfo<vanilla>                 type_info_t;
    typedef typename TypeSpec<ValueHDF5<U>>::inner_type  inner_type;
    typedef FileValue<inner_type>                    data_type;
    typedef data_type                               *data_ptr;


    DataBox(T &val) : value(val) {
        size_t nelms = type_info_t::num_elements(value);
        data = new data_type[nelms];
        const ValueHDF5<U> *vptr = type_info_t::getData(value);
        for (size_t i = 0; i < nelms; i++) {
            FileValue<inner_type> val;
            val.value = get_inner<inner_type,U>(vptr[i].value);
            val.uncertainty = vptr[i].uncertainty;
            val.reference = get_inner<char *,std::string>(vptr[i].reference);
            val.encoder = get_inner<char *,std::string>(vptr[i].encoder);
            val.checksum = get_inner<char *,std::string>(vptr[i].checksum);
            val.filename = get_inner<char *,std::string>(vptr[i].filename);
            data[i] = val;
        }
    }

    data_ptr operator *() {return get();}
    data_ptr get() const {return data;}
    void finish(const H5::DataSpace *space = nullptr) {}

    ~DataBox() {
        delete[] data;
    }

private:

    data_ptr data;
    T &value;
};


template<
        typename T,
        typename U
        >
class DataBox<T, ValueHDF5<U>, typename std::enable_if<! std::is_const<T>::value>::type> {

public:

    typedef hades::TypeInfo<T>                       type_info_t;
    typedef typename TypeSpec<ValueHDF5<U>>::inner_type  inner_type;
    typedef FileValue<inner_type>                    data_type;
    typedef data_type                               *data_ptr;

    DataBox(T &val) : value(val) {
        size_t nelms = type_info_t::num_elements(value);
        data = new data_type[nelms];
    }

    data_ptr operator *() {return get();}
    data_ptr get() {return data;}

    void finish(const H5::DataSpace *space = nullptr) {
        size_t nelms = type_info_t::num_elements(value);
        ValueHDF5<U> *vptr = type_info_t::getData(value);

        for (size_t i = 0; i < nelms; i++) {
            vptr[i] = data[i];
        }

        if (space) {
            TypeSpec<ValueHDF5<U>> spec;
            H5::DataSet::vlenReclaim(data, spec.memType, *space);
        }
    }

    ~DataBox() {
        delete[] data;
    }

private:

    data_ptr data;
    T &value;
};

} // namespace hades
} // namespace hdf5
} // namespace nix

#endif // NIX_VALUE_HDF5_H
