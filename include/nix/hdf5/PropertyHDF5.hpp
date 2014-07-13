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


class PropertyHDF5 : virtual public base::IProperty, public EntityHDF5 {

public:


    PropertyHDF5(const PropertyHDF5 &property);


    PropertyHDF5(const File &file, const Group &group, const DataSet &dataset, const std::string &id, const std::string &name);


    PropertyHDF5(const File &file, const Group &group, const DataSet &dataset, const std::string &id, const std::string &name, time_t time);


    void name(const std::string &name);


    std::string name() const;


    boost::optional<std::string> definition() const;


    void definition(const std::string &definition);


    void definition(const none_t t);


    void mapping(const std::string &mapping);


    boost::optional<std::string> mapping() const;


    void mapping(const none_t t);


    boost::optional<DataType> dataType() const;


    void unit(const std::string &unit);


    boost::optional<std::string> unit() const;


    void unit(const none_t t);


    void deleteValues();


    size_t valueCount() const;


    void values(const std::vector<Value> &values);


    std::vector<Value> values(void) const;


    void values(const boost::none_t t);


    int compare(const IProperty &other) const;


    bool operator==(const PropertyHDF5 &other) const;


    bool operator!=(const PropertyHDF5 &other) const;


    virtual ~PropertyHDF5();

private:
    
    DataSet entity_dataset;

    bool checkDataType(const H5::DataSet &dataset, H5T_class_t type) const;

protected:
    DataSet dataset() const {
        return entity_dataset;
    }


};


} // namespace hdf5 
} // namespace nix

#endif // NIX_PROPERTY_HDF5_H
