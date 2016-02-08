// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "PropertyHDF5.hpp"

#include <nix/util/util.hpp>

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


boost::optional<string> PropertyHDF5::definition() const {
    boost::optional<string> ret;
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
    return this->dataset().dataType();
}


void PropertyHDF5::mapping(const string &mapping) {
    dataset().setAttr("mapping", mapping);
    forceUpdatedAt();
}


boost::optional<string> PropertyHDF5::mapping() const {
    boost::optional<string> ret;
    string mapping;
    if (dataset().getAttr("mapping", mapping)) {
        ret = mapping;
    }
    return ret;
}


void PropertyHDF5::mapping(const nix::none_t t) {
    if (dataset().hasAttr("mapping")) {
        dataset().removeAttr("mapping");
        forceUpdatedAt();
    }
}


void PropertyHDF5::unit(const string &unit) {
    dataset().setAttr("unit", unit);
    forceUpdatedAt();
}


boost::optional<string> PropertyHDF5::unit() const {
    boost::optional<std::string> ret;
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


bool PropertyHDF5::isValidEntity() const {
    return dataset().referenceCount() > 0;
}


PropertyHDF5::~PropertyHDF5() {}

/* Value related functions */

template<typename T>
struct FileValue  {

    T       value;

    double  uncertainty;
    char   *reference;
    char   *filename;
    char   *encoder;
    char   *checksum;

    //ctors
    FileValue() {}
    explicit FileValue(const T &vref) : value(vref) { }

    inline T val() const { return value; }
};

template<>
struct FileValue<bool>  {

    unsigned char value;

    double  uncertainty;
    char   *reference;
    char   *filename;
    char   *encoder;
    char   *checksum;

    //ctors
    FileValue() {}
    explicit FileValue(const bool &vref) :
            value(static_cast<unsigned char>(vref ? 1 : 0)) {
    }

    inline bool val() const { return value > 0; }
};


//

template<typename T>
h5x::DataType h5_type_for_value(bool for_memory)
{
    typedef FileValue<T> file_value_t;

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
#define CHECK_SUPOORTED_VALUES
#endif
#define DATATYPE_SUPPORT_NOT_IMPLEMENTED false

h5x::DataType PropertyHDF5::fileTypeForValue(DataType dtype)
{
    const bool for_memory = true;

    switch(dtype) {
        case DataType::Bool:   return h5_type_for_value<bool>(for_memory);
        case DataType::Int32:  return h5_type_for_value<int32_t>(for_memory);
        case DataType::UInt32: return h5_type_for_value<uint32_t>(for_memory);
        case DataType::Int64:  return h5_type_for_value<int64_t>(for_memory);
        case DataType::UInt64: return h5_type_for_value<uint64_t>(for_memory);
        case DataType::Double: return h5_type_for_value<double>(for_memory);
        case DataType::String: return h5_type_for_value<char *>(for_memory);
#ifndef CHECK_SUPOORTED_VALUES
        default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED); break;
#endif
    }

    return h5x::DataType{};
}


template<typename T>
void do_read_value(const DataSet &h5ds, size_t size, std::vector<Value> &values)
{
    h5x::DataType memType = h5_type_for_value<T>(true);

    typedef FileValue<T> file_value_t;
    std::vector<file_value_t> fileValues;

    fileValues.resize(size);
    values.resize(size);

    h5ds.read(memType.h5id(), fileValues.data());

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
void do_write_value(DataSet &h5ds, const std::vector<Value> &values)
{
    typedef FileValue<T> file_value_t;
    std::vector<file_value_t> fileValues;

    fileValues.resize(values.size());

    std::transform(values.begin(), values.end(), fileValues.begin(), [](const Value &val) {
        file_value_t fileVal(val.get<T>());
        fileVal.uncertainty = val.uncertainty;
        fileVal.reference = const_cast<char *>(val.reference.c_str());
        fileVal.filename = const_cast<char *>(val.filename.c_str());
        fileVal.encoder = const_cast<char *>(val.encoder.c_str());
        fileVal.checksum = const_cast<char *>(val.checksum.c_str());

        return fileVal;
    });

    h5x::DataType memType = h5_type_for_value<T>(true);
    h5ds.write(memType.h5id(), fileValues.data());
}

// value public API

void PropertyHDF5::deleteValues() {
    dataset().setExtent({0});
}


ndsize_t PropertyHDF5::valueCount() const {
    NDSize size = dataset().size();
    return size[0];
}


void PropertyHDF5::values(const std::vector<Value> &values)
{
    if (values.size() < 1) {
        deleteValues();
        return;
    }

    DataSet dset = dataset();
    dset.setExtent(NDSize{values.size()});

    if (values.size() < 1) {
        return; //nothing to do
    }

    switch(values[0].type()) {

        case DataType::Bool:   do_write_value<bool>(dset, values); break;
        case DataType::Int32:  do_write_value<int32_t>(dset, values); break;
        case DataType::UInt32: do_write_value<uint32_t>(dset, values); break;
        case DataType::Int64:  do_write_value<int64_t>(dset, values); break;
        case DataType::UInt64: do_write_value<uint64_t>(dset, values); break;
        case DataType::String: do_write_value<const char *>(dset, values); break;
        case DataType::Double: do_write_value<double>(dset, values); break;
#ifndef CHECK_SUPOORTED_VALUES
        default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED);
#endif
    }
}


std::vector<Value> PropertyHDF5::values(void) const
{
    std::vector<Value> values;

    DataSet dset = dataset();
    DataType dtype = dset.dataType();
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
#ifndef CHECK_SUPOORTED_VALUES
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
