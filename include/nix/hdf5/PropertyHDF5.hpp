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


class PropertyHDF5 : virtual public base::IProperty, public NamedEntityHDF5 {

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


    void removeValues();


    size_t valueCount() const;

    void values(const std::vector<Value> &values);

    std::vector<Value> values(void) const;


    bool operator==(const PropertyHDF5 &other) const;


    bool operator!=(const PropertyHDF5 &other) const;


    virtual ~PropertyHDF5();

private:

    bool checkDataType(const H5::DataSet &dataset, H5T_class_t type) const;

};


} // namespace hdf5
} // namespace nix

#endif // NIX_PROPERTY_HDF5_H
