// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/GroupHDF5.hpp>

#include <nix/util/util.hpp>
#include <nix/Block.hpp>
#include <nix/hdf5/SourceHDF5.hpp>
#include <nix/hdf5/DataArrayHDF5.hpp>
#include <nix/hdf5/TagHDF5.hpp>
#include <nix/hdf5/MultiTagHDF5.hpp>

#include <boost/range/irange.hpp>

using namespace std;
using namespace nix::base;

namespace nix {
namespace hdf5 {

GroupHDF5::GroupHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                     const H5Group &h5group)  : EntityWithSourcesHDF5(file, block, h5group) {
    data_array_group = this->group().openOptGroup("dimensions");
    tag_group = this->group().openOptGroup("tags");
    multi_tag_group = this->group().openOptGroup("multi_tags");
}


GroupHDF5::GroupHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                     const H5Group &h5group, const std::string &id, const std::string &type,
                     const std::string &name) : GroupHDF5(file, block, h5group, id, type, name, util::getTime())  {

}


GroupHDF5::GroupHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                     const H5Group &h5group, const std::string &id, const std::string &type, const std::string &name,
                     time_t time): EntityWithSourcesHDF5(file, block, h5group, id, type, name, time) {
    data_array_group = this->group().openOptGroup("dimensions");
    tag_group = this->group().openOptGroup("tags");
    multi_tag_group = this->group().openOptGroup("multi_tags");
}


bool GroupHDF5::hasDataArray(const string &name_or_id) const {
    return getDataArray(name_or_id) != nullptr;
}


std::shared_ptr<base::IDataArray> GroupHDF5::getDataArray(const string &name_or_id) const {
    shared_ptr<DataArrayHDF5> da;
    boost::optional<H5Group> g = data_array_group();
    if (g) {
        boost::optional<H5Group> group = g->findGroupByNameOrAttribute("entity_id", name_or_id);
        if (group)
            da = make_shared<DataArrayHDF5>(file(), block(), *group);
    }
    return da;
}

} // hdf5
} // nix
