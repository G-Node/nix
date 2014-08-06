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


    DataTagHDF5(std::shared_ptr<base::IFile> file, std::shared_ptr<base::IBlock> block, const Group &group,
                const std::string &id, const std::string &type, const std::string &name, const DataArray &positions);


    DataTagHDF5(std::shared_ptr<base::IFile> file, std::shared_ptr<base::IBlock> block, const Group &group,
                const std::string &id, const std::string &type, const std::string &name, const DataArray &positions, time_t time);


    std::shared_ptr<base::IDataArray> positions() const;


    void positions(const std::string &id);


    bool hasPositions() const;


    std::shared_ptr<base::IDataArray> extents() const;


    void extents(const std::string &extentsId);


    void extents(const none_t t);


    std::vector<std::string> units() const;


    void units(const std::vector<std::string> &units);


    void units(const none_t t);

    //--------------------------------------------------
    // Methods concerning references.
    //--------------------------------------------------

    bool hasReference(const std::string &id) const;


    size_t referenceCount() const;


    std::shared_ptr<base::IDataArray> getReference(const std::string &id) const;


    std::shared_ptr<base::IDataArray> getReference(size_t index) const;


    void addReference(const std::string &id);


    bool removeReference(const std::string &id);

    // TODO evaluate if DataArray can be replaced by shared_ptr<IDataArray>
    void references(const std::vector<DataArray> &references);

    //--------------------------------------------------
    // Methods concerning features.
    //--------------------------------------------------


    bool hasFeature(const std::string &id) const;


    size_t featureCount() const;


    std::shared_ptr<base::IFeature> getFeature(const std::string &id) const;


    std::shared_ptr<base::IFeature> getFeature(size_t index) const;


    std::shared_ptr<base::IFeature> createFeature(const std::string &data_array_id, LinkType link_type);


    bool deleteFeature(const std::string &id);


    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------


    void swap(DataTagHDF5 &other);

    // TODO do we really need swap and operator=?
    DataTagHDF5& operator=(const DataTagHDF5 &other);


    virtual ~DataTagHDF5();

private:

    bool checkDimensions(const DataArray &a, const DataArray &b) const;


    bool checkPositionsAndExtents() const;

};


} // namespace hdf5
} // namespace nix

#endif // NIX_DATA_TAG_HDF5_H
