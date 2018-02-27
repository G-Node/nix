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
    const h5x::DataType dtype = dataset().dataType();
    return data_type_from_h5(dtype);
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


void PropertyHDF5::uncertainty(double uncertainty) {
    dataset().setAttr("uncertainty", uncertainty);
    forceUpdatedAt();
}


boost::optional<double> PropertyHDF5::uncertainty() const {
    boost::optional<double> ret;
    double error;
    if (dataset().getAttr("uncertainty", error)) {
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
#ifdef _MSC_VER
#pragma pack(pop)
#endif

//

template<typename T>
h5x::DataType h5_type_for_value(bool for_memory)
{
    typedef FileValue<T> file_value_t;

    h5x::DataType ct = h5x::DataType::makeCompound(sizeof(file_value_t));
    // h5x::DataType strType = h5x::DataType::makeStrType();

    h5x::DataType value_type = data_type_to_h5(to_data_type<T>::value, for_memory);
    /*
    h5x::DataType double_type = data_type_to_h5(DataType::Double, for_memory);

    ct.insert("value", HOFFSET(file_value_t, value), value_type);
    ct.insert("uncertainty", HOFFSET(file_value_t, uncertainty), double_type);
    ct.insert("reference", HOFFSET(file_value_t, reference), strType);
    ct.insert("filename", HOFFSET(file_value_t, filename), strType);
    ct.insert("encoder", HOFFSET(file_value_t, encoder), strType);
    ct.insert("checksum", HOFFSET(file_value_t, checksum), strType);
    */
    return ct;
}

#if 0 //set to one to check that all supported DataTypes are handled
#define CHECK_SUPOORTED_VALUES
#endif
#define DATATYPE_SUPPORT_NOT_IMPLEMENTED false

h5x::DataType PropertyHDF5::fileTypeForValue(DataType dtype)
{
    const bool for_memory = false;

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
void do_read_value(const DataSet &h5ds, size_t size, std::vector<Variant> &values)
{
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


#define NOT_IMPLEMENTED 1

template<typename T>
void do_write_value(DataSet &h5ds, const std::vector<Variant> &values)
{
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

void copyValue(char *buffer, const Variant &v) {
    const char n = '\0';

    switch (v.type()) {
    case DataType::Bool:
        bool b;
        v.get(b);
        std::memcpy(buffer, &b, sizeof(b));
        break;

    case DataType::Double:
        double d;
        v.get(d);
        std::memcpy(buffer, &d, sizeof(d));
        break;

    case DataType::UInt32:
        uint32_t ui32;
        v.get(ui32);
        std::memcpy(buffer, &ui32, sizeof(ui32));
        break;

    case DataType::Int32:
        int32_t i32;
        v.get(i32);
        std::memcpy(buffer, &i32, sizeof(i32));
        break;

    case DataType::UInt64:
        uint64_t ui64;
        v.get(ui64);
        std::memcpy(buffer, &ui64, sizeof(ui64));
        break;

    case DataType::Int64:
        int64_t i64;
        v.get(i64);
        std::memcpy(buffer, &i64, sizeof(i64));
        break;

    case DataType::String:
        const char *str;
        str = v.get<const char *>();
        std::cerr << "Pointer address1 = " << static_cast<void*>(buffer) << std::endl;

        std::memcpy(buffer, &str, strlen(str));
        buffer += strlen(str);
        std::cerr << "Pointer address2 = " << static_cast<void*>(buffer) << std::endl;
        std::memcpy(buffer, &n, sizeof(n));
        buffer += sizeof(n);
        std::cerr << "Pointer address3 = " << static_cast<void*>(buffer) << std::endl;
        break;

    default:
        throw std::invalid_argument("Unhandled DataType");
    };
}


void PropertyHDF5::values(const std::vector<Variant> &values)
{
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

    if (dt == DataType::String) {
        h5x::DataType memType = data_type_to_h5_memtype(dt);
        DataSpace fileSpace, memSpace;
        std::tie(memSpace, fileSpace) = dset.offsetCount2DataSpaces(NDSize{values.size()}, NDSize{0});
        std::vector<std::string> strings;
        for (Variant v : values) {
            if (v.type() != dt) {
                throw std::invalid_argument("Inconsistent DataTypes!");
            }
            std::string temp;
            v.get(temp);
            strings.push_back(temp);
        }
        StringReader reader(NDSize{values.size()}, strings.data());
        dset.write(*reader, memType, memSpace, fileSpace);
    } else {
        size_t buffer_size  = nix::data_type_to_size(values[0].type()) * values.size();
        char *buffer = new char[buffer_size];
        char *mem = buffer;
        for (Variant v : values) {
            if (v.type() == dt) {
                copyValue(mem, v);
                mem += nix::data_type_to_size(dt);
            } else {
                delete [] buffer;
                throw std::invalid_argument("Inconsistent DataTypes");
            }
        }
        dset.write(buffer, data_type_to_h5_memtype(dt), NDSize{values.size()});
        delete [] buffer;
    }
}


std::vector<Variant> PropertyHDF5::values(void) const
{
    std::vector<Variant> values;

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
