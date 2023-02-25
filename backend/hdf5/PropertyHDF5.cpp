// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "PropertyHDF5.hpp"

#include <nix/util/util.hpp>
#include <nix/Version.hpp>

#include <iostream>

using namespace std;

using namespace nix::base;

namespace nix {

//special to_data_type for our purporses (Value)
template<>
struct to_data_type<char *> {
    static const bool is_valid = true;
    static const DataType value = DataType::String;
};

template<>
struct to_data_type<const char *> {
    static const bool is_valid = true;
    static const DataType value = DataType::String;
};


namespace hdf5 {


PropertyHDF5::PropertyHDF5(const std::shared_ptr<IFile> &file, const DataSet &dataset)
    : entity_file(file)
{
    this->entity_dataset = dataset;
}


    PropertyHDF5::PropertyHDF5(const std::shared_ptr<IFile> &file, const DataSet &dataset, const string &id,
                               const string &name)
    : PropertyHDF5(file, dataset, id, name, util::getTime())
{
}


    PropertyHDF5::PropertyHDF5(const std::shared_ptr<IFile> &file, const DataSet &dataset, const string &id,
                               const string &name, time_t time)
    : entity_file(file)
{
    this->entity_dataset = dataset;
    // set name
    if (name.empty()) {
        throw EmptyString("name");
    } else {
        dataset.setAttr("name", name);
        forceUpdatedAt();
    }

    dataset.setAttr("entity_id", id);
    setUpdatedAt();
    forceCreatedAt(time);
}


string PropertyHDF5::id() const {
    string t;

    if (dataset().hasAttr("entity_id")) {
        dataset().getAttr("entity_id", t);
    }
    else {
        throw runtime_error("Entity has no id!");
    }

    return t;
}


time_t PropertyHDF5::updatedAt() const {
    string t;
    dataset().getAttr("updated_at", t);
    return util::strToTime(t);
}


void PropertyHDF5::setUpdatedAt() {
    if (!dataset().hasAttr("updated_at")) {
        time_t t = util::getTime();
        dataset().setAttr("updated_at", util::timeToStr(t));
    }
}


void PropertyHDF5::forceUpdatedAt() {
    time_t t = util::getTime();
    dataset().setAttr("updated_at", util::timeToStr(t));
}


time_t PropertyHDF5::createdAt() const {
    string t;
    dataset().getAttr("created_at", t);
    return util::strToTime(t);
}


void PropertyHDF5::setCreatedAt() {
    if (!dataset().hasAttr("created_at")) {
        time_t t = util::getTime();
        dataset().setAttr("created_at", util::timeToStr(t));
    }
}


void PropertyHDF5::forceCreatedAt(time_t t) {
    dataset().setAttr("created_at", util::timeToStr(t));
}


string PropertyHDF5::name() const {
    string name;
    if (dataset().hasAttr("name")) {
        dataset().getAttr("name", name);
        return name;
    } else {
        throw MissingAttr("name");
    }
}


void PropertyHDF5::definition(const string &definition) {
        dataset().setAttr("definition", definition);
        forceUpdatedAt();
}


std::optional<string> PropertyHDF5::definition() const {
    std::optional<string> ret;
    string definition;
    bool have_attr = dataset().getAttr("definition", definition);
    if (have_attr) {
        ret = definition;
    }
    return ret;
}


void PropertyHDF5::definition(const nix::none_t t) {
    if (dataset().hasAttr("definition")) {
        dataset().removeAttr("definition");
    }
    forceUpdatedAt();
}


DataType PropertyHDF5::dataType() const {
    const h5x::DataType dtype = dataset().dataType();
    return data_type_from_h5(dtype);
}


void PropertyHDF5::unit(const string &unit) {
    dataset().setAttr("unit", unit);
    forceUpdatedAt();
}


std::optional<string> PropertyHDF5::unit() const {
    std::optional<std::string> ret;
    string unit;
    if (dataset().getAttr("unit", unit)) {
        ret = unit;
    }
    return ret;
}


void PropertyHDF5::unit(const nix::none_t t) {
    if (dataset().hasAttr("unit")) {
        dataset().removeAttr("unit");
    }
    forceUpdatedAt();
}


void PropertyHDF5::uncertainty(double uncertainty) {
    dataset().setAttr("uncertainty", uncertainty);
    forceUpdatedAt();
}


std::optional<double> PropertyHDF5::uncertainty() const {
    std::optional<double> ret;
    double error;
    nix::FormatVersion ver(this->entity_file->version());
    if (ver < nix::FormatVersion({1, 1, 1})) {
        std::vector<Value> values = readOldstyleValues();
        if (values.size() > 0)
            ret = values[0].uncertainty;
    } else if (dataset().getAttr("uncertainty", error)) {
        ret = error;
    }
    return ret;
}


void PropertyHDF5::uncertainty(const nix::none_t t) {
    if (dataset().hasAttr("uncertainty")) {
        dataset().removeAttr("uncertainty");
    }
    forceUpdatedAt();
}


bool PropertyHDF5::isValidEntity() const {
    return dataset().referenceCount() > 0;
}


PropertyHDF5::~PropertyHDF5() {}

/* Value related functions */

#ifdef _MSC_VER
#pragma pack(push,1)
#endif
template<typename T>
struct NIX_PACKED FileValue  {
    T       value;
    //ctors
    FileValue() {}
    explicit FileValue(const T &vref) : value(vref) { }

    inline T val() const { return value; }
};

template<typename T>
struct NIX_PACKED FileOldValue  {

    T       value;

    double  uncertainty;
    char   *reference;
    char   *filename;
    char   *encoder;
    char   *checksum;

    //ctors
    FileOldValue() {}
    explicit FileOldValue(const T &vref) : value(vref) { }

    inline T val() const { return value; }
};
#ifdef _MSC_VER
#pragma pack(pop)
#endif


template<typename T>
h5x::DataType h5_type_for_value(bool for_memory) {
    h5x::DataType value_type = data_type_to_h5(to_data_type<T>::value, for_memory);
    return value_type;
}


template<typename T>
h5x::DataType h5_type_for_old_value(bool for_memory) {
    typedef FileOldValue<T> file_value_t;

    h5x::DataType ct = h5x::DataType::makeCompound(sizeof(file_value_t));
    h5x::DataType strType = h5x::DataType::makeStrType();

    h5x::DataType value_type = data_type_to_h5(to_data_type<T>::value, for_memory);
    h5x::DataType double_type = data_type_to_h5(DataType::Double, for_memory);

    ct.insert("value", HOFFSET(file_value_t, value), value_type);
    ct.insert("uncertainty", HOFFSET(file_value_t, uncertainty), double_type);
    ct.insert("reference", HOFFSET(file_value_t, reference), strType);
    ct.insert("filename", HOFFSET(file_value_t, filename), strType);
    ct.insert("encoder", HOFFSET(file_value_t, encoder), strType);
    ct.insert("checksum", HOFFSET(file_value_t, checksum), strType);

    return ct;
}


#if 0 //set to one to check that all supported DataTypes are handled
#define CHECK_SUPPORTED_VALUES
#endif
#define DATATYPE_SUPPORT_NOT_IMPLEMENTED false

h5x::DataType PropertyHDF5::fileTypeForValue(DataType dtype) {
    const bool for_memory = false;

    switch(dtype) {
        case DataType::Bool:   return h5_type_for_value<bool>(for_memory);
        case DataType::Int32:  return h5_type_for_value<int32_t>(for_memory);
        case DataType::UInt32: return h5_type_for_value<uint32_t>(for_memory);
        case DataType::Int64:  return h5_type_for_value<int64_t>(for_memory);
        case DataType::UInt64: return h5_type_for_value<uint64_t>(for_memory);
        case DataType::Double: return h5_type_for_value<double>(for_memory);
        case DataType::String: return h5_type_for_value<char *>(for_memory);
#ifndef CHECK_SUPPORTED_VALUES
        default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED); break;
#endif
    }

    return h5x::DataType{};
}


template<typename T>
void do_read_value(const DataSet &h5ds, size_t size, std::vector<Variant> &values) {
    h5x::DataType memType = h5_type_for_value<T>(true);

    typedef FileValue<T> file_value_t;
    std::vector<file_value_t> fileValues;

    fileValues.resize(size);
    values.resize(size);

    h5ds.read(fileValues.data(), memType, H5S_ALL, H5S_ALL);

    std::transform(fileValues.begin(), fileValues.end(), values.begin(), [](const file_value_t &val) {
        Variant temp(val.val());
        return temp;
    });

    h5ds.vlenReclaim(memType, fileValues.data());
}


template<typename T>
void do_read_old_value(const DataSet &h5ds, size_t size, std::vector<Value> &values) {
    h5x::DataType memType = h5_type_for_old_value<T>(true);

    typedef FileOldValue<T> file_value_t;
    std::vector<file_value_t> fileValues;

    fileValues.resize(size);
    values.resize(size);

    h5ds.read(fileValues.data(), memType, H5S_ALL, H5S_ALL);

    std::transform(fileValues.begin(), fileValues.end(), values.begin(), [](const file_value_t &val) {
        Value temp(val.val());
        temp.uncertainty = val.uncertainty;
        temp.reference = val.reference;
        temp.filename = val.filename;
        temp.encoder = val.encoder;
        temp.checksum = val.checksum;
        return temp;
    });

    h5ds.vlenReclaim(memType, fileValues.data());
}


#define NOT_IMPLEMENTED 1

template<typename T>
void do_write_value(DataSet &h5ds, const std::vector<Variant> &values) {
    typedef FileValue<T> file_value_t;
    std::vector<file_value_t> fileValues;

    fileValues.resize(values.size());

    std::transform(values.begin(), values.end(), fileValues.begin(), [](const Variant &val) {
        file_value_t fileVal(val.get<T>());
        return fileVal;
    });

    h5x::DataType memType = h5_type_for_value<T>(true);
    h5ds.write(fileValues.data(), memType, H5S_ALL, H5S_ALL);
}

// value public API

void PropertyHDF5::deleteValues() {
    dataset().setExtent({0});
}


ndsize_t PropertyHDF5::valueCount() const {
    NDSize size = dataset().size();
    return size[0];
}


void PropertyHDF5::values(const std::vector<Variant> &values) {
    if (values.size() < 1) {
        deleteValues();
        return;
    }
    DataSet dset = dataset();
    DataType dt = values[0].type();
    if (dt != data_type_from_h5(dset.dataType())) {
        throw std::invalid_argument("Inconsistent DataTypes!");
    }
    dset.setExtent(NDSize{values.size()});

    switch(values[0].type()) {
        case DataType::Bool:   do_write_value<bool>(dset, values);         break;
        case DataType::Int32:  do_write_value<int32_t>(dset, values);      break;
        case DataType::UInt32: do_write_value<uint32_t>(dset, values);     break;
        case DataType::Int64:  do_write_value<int64_t>(dset, values);      break;
        case DataType::UInt64: do_write_value<uint64_t>(dset, values);     break;
        case DataType::String: do_write_value<const char *>(dset, values); break;
        case DataType::Double: do_write_value<double>(dset, values);       break;
#ifndef CHECK_SUPPORTED_VALUES
        default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED);
#endif
     }
}

Variant valueToVariant(const Value &val) {
     switch (val.type()) {
         case DataType::Bool:   return Variant(val.get<bool>());
         case DataType::Int32:  return Variant(val.get<int32_t>());
         case DataType::UInt32: return Variant(val.get<uint32_t>());
         case DataType::Int64:  return Variant(val.get<int64_t>());
         case DataType::UInt64: return Variant(val.get<uint64_t>());
         case DataType::String: return Variant(val.get<std::string>());
         case DataType::Double: return Variant(val.get<double>());
#ifndef CHECK_SUPPORTED_VALUES
         default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED);
#endif
     }
     return Variant();
}

std::vector<Value> PropertyHDF5::readOldstyleValues() const {
    std::vector<Value> values;

    DataSet dset = dataset();
    DataType dtype = data_type_from_h5(dset.dataType());
    NDSize shape = dset.size();

    if (shape.size() < 1 || shape[0] < 1) {
        return values;
    }

    assert(shape.size() == 1);
    size_t nvalues = nix::check::fits_in_size_t(shape[0], "Can't resize: data to big for memory");

    switch (dtype) {
        case DataType::Bool:   do_read_old_value<bool>(dset, nvalues, values);     break;
        case DataType::Int32:  do_read_old_value<int32_t>(dset, nvalues, values);  break;
        case DataType::UInt32: do_read_old_value<uint32_t>(dset, nvalues, values); break;
        case DataType::Int64:  do_read_old_value<int64_t>(dset, nvalues, values);  break;
        case DataType::UInt64: do_read_old_value<uint64_t>(dset, nvalues, values); break;
        case DataType::String: do_read_old_value<char *>(dset, nvalues, values);   break;
        case DataType::Double: do_read_old_value<double>(dset, nvalues, values);   break;
#ifndef CHECK_SUPPORTED_VALUES
        default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED);
#endif
    }
    return values;
}


std::vector<Variant> PropertyHDF5::values(void) const {
    std::vector<Variant> values;
    nix::FormatVersion ver(this->entity_file->version());
    if (ver < nix::FormatVersion({1, 1, 1})) {
        for (Value v : readOldstyleValues()) {
            values.push_back(valueToVariant(v));
        }
        return values;
    }
    DataSet dset = dataset();
    DataType dtype = data_type_from_h5(dset.dataType());
    NDSize shape = dset.size();

    if (shape.size() < 1 || shape[0] < 1) {
        return values;
    }
    assert(shape.size() == 1);
    size_t nvalues = nix::check::fits_in_size_t(shape[0], "Can't resize: data to big for memory");

    switch (dtype) {
        case DataType::Bool:   do_read_value<bool>(dset, nvalues, values);     break;
        case DataType::Int32:  do_read_value<int32_t>(dset, nvalues, values);  break;
        case DataType::UInt32: do_read_value<uint32_t>(dset, nvalues, values); break;
        case DataType::Int64:  do_read_value<int64_t>(dset, nvalues, values);  break;
        case DataType::UInt64: do_read_value<uint64_t>(dset, nvalues, values); break;
        case DataType::String: do_read_value<char *>(dset, nvalues, values);   break;
        case DataType::Double: do_read_value<double>(dset, nvalues, values);   break;
#ifndef CHECK_SUPPORTED_VALUES
    default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED);
#endif
    }
    return values;
}


void PropertyHDF5::values(const nix::none_t t) {
    // TODO: rethink if we want two methods for same thing
    deleteValues();
}



} // ns nix::hdf5
} // ns nix
