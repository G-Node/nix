// Copyright (c) 2017, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Authors: Christian Kellner <ckellner@redhat.com>

#ifndef NIX_DATA_FRAME_HDF5_H
#define NIX_DATA_FRAME_HDF5_H


#include <nix/base/IDataFrame.hpp>
#include "EntityWithSourcesHDF5.hpp"

namespace nix {
namespace hdf5 {

class DataFrameHDF5 : virtual public base::IDataFrame, public EntityWithSourcesHDF5 {
private:


public:

    /**
     * Standard constructor for existing DataFrames
     */
    DataFrameHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const H5Group &group);

    /**
     * Standard constructor for new DataFrames
     */
    DataFrameHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const H5Group &group, const std::string &id, const std::string &type, const std::string &name);

    /**
     * Standard constructor for new DataFrames that preserves the creation time.
     */
    DataFrameHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const H5Group &group, const std::string &id, const std::string &type, const std::string &name, time_t time);


    void createData(const std::vector<Column> &cols);

    std::vector<Column> columns() const override;


};


}
}

#endif
