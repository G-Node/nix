// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
// Author: Jan Grewe <jan.grewe@g-node.org>


#ifndef NIX_MULTI_TAG_HDF5_H
#define NIX_MULTI_TAG_HDF5_H

#include <nix/base/IMultiTag.hpp>
#include "BaseTagHDF5.hpp"

#include <string>
#include <vector>
#include <memory>

namespace nix {
namespace hdf5 {


class MultiTagHDF5 : public BaseTagHDF5, virtual public base::IMultiTag {

public:


    /**
     * Standard constructor for new MultiTag
     */
    MultiTagHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const H5Group &group);
                
    /**
     * Standard constructor for new MultiTag
     */
    MultiTagHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const H5Group &group,
                const std::string &id, const std::string &type, const std::string &name, const DataArray &positions);


    /**
     * Standard constructor for new MultiTag with time
     */
    MultiTagHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const H5Group &group,
                const std::string &id, const std::string &type, const std::string &name, const DataArray &positions, time_t time);


    std::shared_ptr<base::IDataArray> positions() const;


    void positions(const std::string &name_or_id);


    bool hasPositions() const;


    std::shared_ptr<base::IDataArray> extents() const;


    void extents(const std::string &name_or_id);


    void extents(const none_t t);


    std::vector<std::string> units() const;


    void units(const std::vector<std::string> &units);


    void units(const none_t t);


    virtual ~MultiTagHDF5();

private:

    bool checkDimensions(const DataArray &a, const DataArray &b) const;

};


} // namespace hdf5
} // namespace nix

#endif // NIX_MULTI_TAG_HDF5_H
