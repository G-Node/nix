// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef PAN_PROPERTY_H_INCLUDE
#define PAN_PROPERTY_H_INCLUDE

#include <string>

#if defined(_WIN32)
#include <cpp/H5Cpp.h>
#else
#include <H5Cpp.h>
#endif

#include <pandora/Value.hpp>
#include <pandora/DataSet.hpp>
#include <pandora/NamedEntity.hpp>

namespace pandora {

class Property : public NamedEntity{

private:

    bool checkDataType(const H5::DataSet &dataset, H5T_class_t type) const;

public:

    Property(const Property &property);

    Property(Group group,const std::string &id);

    Property(Group group,const std::string &id, time_t time);

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

    //void addValue(int value, const std::string &reference, const std::string &filename, const std::string filename, const std::string encoder, const std::string checksum);


    bool operator==(const Property &other) const;

    bool operator!=(const Property &other) const;

    virtual ~Property();

};

template<typename T>
void Property::addValue(T value, double uncertainty, const std::string filename,
                        const std::string encoder, const std::string checksum, const std::string &reference) {

    std::string dt = this->dataType();
    //  if (dt.length() > 0 && dt.compare(info.type) != 0) {
    //    throw std::runtime_error("Value and data type do not match!");
    //    return;
    //  } else {
    //    dataType(info.type);
    //  }
    Value<T> tempValue(value, uncertainty, filename, encoder, checksum, reference);
    addValue(tempValue);
}

template<typename T>
void Property::addValue(const Value<T> &value) {
    const std::vector<Value<T>> vals = { value };

    PSize start;
    DataSet ds((H5::DataSet()));
    if (group.hasData("values")) {
        ds = group.openData("values");
        PSize size = ds.size();
        PSize newSize = size + 1;
        ds.extend(newSize);
        start = size;
    } else {
        Charon<const std::vector<Value<T>>> charon(vals);
        PSize size = {1};
        PSize maxsize = {H5S_UNLIMITED};
        PSize chunks = DataSet::guessChunking(size, DataType::Double);
        ds = DataSet::create(group.h5Group(), charon.getFileType(),  "values", size, &maxsize, &chunks);
        start = {0};
    }

    Selection fileSel = ds.createSelection();
    PSize count = {1};
    fileSel.select(count, start);
    ds.write(vals, fileSel);
}

template<typename T>
void Property::value(size_t index, Value<T> &value) const {
    if (group.hasData("values")) {
        if (index >= valueCount()) {
            throw std::runtime_error("Property::stringValue(index): Index out of bounds!");
        }
        DataSet dataset = group.openData("values");

        //    ValueInfo<T> info;
        //    if (!checkDataType(dataset, info.h5class)) {
        //      throw std::runtime_error("Property::stringValue(index): Value DataType is not String!");
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
}

#endif // HDX_PROPERTY_H_INCLUDE
