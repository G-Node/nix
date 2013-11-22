// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_PROPERTY_HDF5_H
#define NIX_PROPERTY_HDF5_H

#include <string>

#include <nix.hpp>
#include <nix/hdf5/hdf5include.hpp>
#include <nix/hdf5/NamedEntityHDF5.hpp>

namespace nix {
namespace hdf5 {


class PropertyHDF5 : virtual public IProperty, public NamedEntityHDF5 {

public:


    PropertyHDF5(const PropertyHDF5 &property);


    PropertyHDF5(const File &file, const Group &group,const std::string &id);


    PropertyHDF5(const File &file, const Group &group,const std::string &id, time_t time);


    void include(const std::string &include);


    std::string include() const;


    void mapping(const std::string &mapping);


    std::string mapping() const;


    void dataType(const std::string &dataType);


    std::string dataType() const;


    void unit(const std::string &unit);


    std::string unit() const;


    template<typename T>
    void addValue(const Value<T> &value);


    template<typename T>
    void addValue(const T value, double uncertainty = 0.0, const std::string filename = "",
                  const std::string encoder = "", const std::string checksum = "", const std::string &reference = "");


    template<typename T>
    void value(size_t index, Value<T> &value) const;


    void removeValue(size_t index);


    void removeValues();


    size_t valueCount() const;


    bool operator==(const PropertyHDF5 &other) const;


    bool operator!=(const PropertyHDF5 &other) const;


    virtual ~PropertyHDF5();

private:

    bool checkDataType(const H5::DataSet &dataset, H5T_class_t type) const;

};


template<typename T>
void PropertyHDF5::addValue(T value, double uncertainty, const std::string filename,
                        const std::string encoder, const std::string checksum, const std::string &reference) {

    std::string dt = this->dataType();
    Value<T> tempValue(value, uncertainty, filename, encoder, checksum, reference);
    addValue(tempValue);
}


template<typename T>
void PropertyHDF5::addValue(const Value<T> &value) {
    const std::vector<Value<T>> vals = { value };

    PSize start;
    DataSet ds((H5::DataSet()));
    if (group().hasData("values")) {
        ds = group().openData("values");
        PSize size = ds.size();
        PSize newSize = size + 1;
        ds.extend(newSize);
        start = size;
    } else {
        Charon<const std::vector<Value<T>>> charon(vals);
        PSize size = {1};
        PSize maxsize = {H5S_UNLIMITED};
        PSize chunks = DataSet::guessChunking(size, DataType::Double);
        ds = DataSet::create(group().h5Group(), charon.getFileType(),  "values", size, &maxsize, &chunks);
        start = {0};
    }

    Selection fileSel = ds.createSelection();
    PSize count = {1};
    fileSel.select(count, start);
    ds.write(vals, fileSel);
}


template<typename T>
void PropertyHDF5::value(size_t index, Value<T> &value) const {
    if (group().hasData("values")) {
        if (index >= valueCount()) {
            throw std::runtime_error("PropertyHDF5::stringValue(index): Index out of bounds!");
        }
        DataSet dataset = group().openData("values");

        //    ValueInfo<T> info;
        //    if (!checkDataType(dataset, info.h5class)) {
        //      throw std::runtime_error("PropertyHDF5::stringValue(index): Value DataType is not String!");
        //    }

        std::vector<Value<T> > vals;

        Selection fileSel = dataset.createSelection();
        PSize start = {index};
        PSize count = {1};
        fileSel.select(count, start);

        dataset.read(vals, fileSel, true);
        value = vals[0];
    }
}


} // namespace hdf5
} // namespace nix

#endif // NIX_PROPERTY_HDF5_H
