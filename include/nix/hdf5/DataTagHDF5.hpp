// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
// Author: Jan Grewe <jan.grewe@g-node.org>


#ifndef NIX_DATA_TAG_HDF5_H
#define NIX_DATA_TAG_HDF5_H

#include <string>
#include <vector>

#include <nix.hpp>
#include <nix/hdf5/EntityWithSourcesHDF5.hpp>
#include <nix/hdf5/ReferenceList.hpp>

namespace nix {
namespace hdf5 {


class DataTagHDF5 : virtual public base::IDataTag, public EntityWithSourcesHDF5 {

private:

    Group feature_group;
    ReferenceList reference_list;

public:

    DataTagHDF5(const DataTagHDF5 &tag);


    DataTagHDF5(const File &file, const Block &block, const Group &group, 
                const std::string &id, const std::string &type, const string &name, const DataArray _positions);


    DataTagHDF5(const File &file, const Block &block, const Group &group, 
                const std::string &id, const std::string &type, const string &name, const DataArray _positions, time_t time);


    DataArray positions() const;


    void positions(const std::string &id);


    bool hasPositions() const;


    DataArray extents() const;


    void extents(const DataArray &extents);


    void extents(const std::string &extentsId);


    void extents(const none_t t);
    

    //--------------------------------------------------
    // Methods concerning references.
    //--------------------------------------------------

    bool hasReference(const std::string &id) const;


    size_t referenceCount() const;


    DataArray getReference(const std::string &id) const;


    DataArray getReference(size_t index) const;
    

    void addReference(const std::string &id);


    bool removeReference(const std::string &id);


    void references(const std::vector<DataArray> &references);

    //--------------------------------------------------
    // Methods concerning features.
    //--------------------------------------------------


    bool hasFeature(const std::string &id) const;


    size_t featureCount() const;


    Feature getFeature(const std::string &id) const;


    Feature getFeature(size_t index) const;


    Feature createFeature(const std::string &data_array_id, LinkType link_type);


    bool deleteFeature(const std::string &id);


    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------


    void swap(DataTagHDF5 &other);


    DataTagHDF5& operator=(const DataTagHDF5 &other);


    friend std::ostream& operator<<(std::ostream &out, const DataTagHDF5 &ent);


    virtual ~DataTagHDF5();

private:

    bool checkDimensions(const DataArray &a, const DataArray &b) const;

    bool checkPositionsAndExtents() const;

};


} // namespace hdf5
} // namespace nix

#endif // NIX_DATA_TAG_HDF5_H
