// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_PROPERTY_HDF5_H
#define NIX_PROPERTY_HDF5_H

#include <nix/base/IFile.hpp>
#include <nix/base/IEntity.hpp>
#include <nix/base/IProperty.hpp>
#include "NamedEntityHDF5.hpp"

#include <string>
#include <memory>
#include <ctime>

namespace nix {
namespace hdf5 {


class PropertyHDF5 : virtual public base::IProperty {

    std::shared_ptr<base::IFile>  entity_file;
    DataSet                       entity_dataset;

public:


    /**
     * Standard constructor for existing Property
     */
    PropertyHDF5(const std::shared_ptr<base::IFile> &file, const DataSet &dataset);

    /**
     * Standard constructor for new Property
     */
    PropertyHDF5(const std::shared_ptr<base::IFile> &file, const DataSet &dataset, const std::string &id,
                 const std::string &name);

    /**
     * Constructor for new Property with time
     */
    PropertyHDF5(const std::shared_ptr<base::IFile> &file, const DataSet &dataset, const std::string &id,
                 const std::string &name, time_t time);


    std::string id() const;


    time_t updatedAt() const;


    time_t createdAt() const;


    void setUpdatedAt();


    void forceUpdatedAt();


    void setCreatedAt();


    void forceCreatedAt(time_t t);


    std::string name() const;


    std::optional<std::string> definition() const;


    void definition(const std::string &definition);


    void definition(const none_t t);


    DataType dataType() const;


    void unit(const std::string &unit);


    std::optional<std::string> unit() const;


    void unit(const none_t t);


    void uncertainty(double uncertainty);


    std::optional<double> uncertainty() const;


    void uncertainty(const none_t t);


    void deleteValues();


    ndsize_t valueCount() const;


    void values(const std::vector<Variant> &values);


    std::vector<Variant> values(void) const;


    void values(const none_t t);


    bool isValidEntity() const;


    static h5x::DataType fileTypeForValue(DataType dtype);

    virtual ~PropertyHDF5();

private:

    DataSet dataset() const {
        return entity_dataset;
    }

    std::vector<Value> readOldstyleValues() const;

};


} // namespace hdf5
} // namespace nix

#endif // NIX_PROPERTY_HDF5_H
