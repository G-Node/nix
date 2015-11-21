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
#include <nix/hdf5/BlockHDF5.hpp>

using namespace std;
using namespace nix::base;

namespace nix {
namespace hdf5 {

GroupHDF5::GroupHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                     const H5Group &h5group)  : EntityWithSourcesHDF5(file, block, h5group) {
    data_array_group = this->group().openOptGroup("data_arrays");
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
    data_array_group = this->group().openOptGroup("data_arrays");
    tag_group = this->group().openOptGroup("tags");
    multi_tag_group = this->group().openOptGroup("multi_tags");
}


bool GroupHDF5::hasDataArray(const string &name_or_id) const {
    std::string id = name_or_id;
    if (!util::looksLikeUUID(name_or_id) && block()->hasDataArray(name_or_id)) {
        id = block()->getDataArray(name_or_id)->id();
    }
    return data_array_group() ? data_array_group()->hasGroup(id) : false;
}


ndsize_t GroupHDF5::dataArrayCount() const {
    boost::optional<H5Group> g = data_array_group();
    return g ? g->objectCount() : size_t(0);
}


void GroupHDF5::addDataArray(const std::string &name_or_id) {
    boost::optional<H5Group> g = data_array_group(true);

    if (!block()->hasDataArray(name_or_id))
        throw std::runtime_error("GroupHDF5::addDataArray: DataArray not found in block!");

    auto target = dynamic_pointer_cast<DataArrayHDF5>(block()->getDataArray(name_or_id));
    g->createLink(target->group(), target->id());
}


std::shared_ptr<base::IDataArray> GroupHDF5::getDataArray(const string &name_or_id) const {
    shared_ptr<IDataArray> da;
    boost::optional<H5Group> g = data_array_group();
    std::string id = name_or_id;
    if (!util::looksLikeUUID(name_or_id) && block()->hasDataArray(name_or_id)) {
        id = block()->getDataArray(name_or_id)->id();
    }

    if (g && hasDataArray(id)) {
        H5Group h5g = g->openGroup(id);
        da = make_shared<DataArrayHDF5>(file(), block(), h5g);
    }
    return da;
}


shared_ptr<IDataArray>  GroupHDF5::getDataArray(ndsize_t index) const {
    boost::optional<H5Group> g = data_array_group();
    string id = g ? g->objectName(index) : "";
    return getDataArray(id);
}


bool GroupHDF5::removeDataArray(const std::string &name_or_id) {
    boost::optional<H5Group> g = data_array_group();
    bool removed = false;

    if (g && hasDataArray(name_or_id)) {
        shared_ptr<IDataArray> data_array = getDataArray(name_or_id);

        g->removeGroup(data_array->id());
        removed = true;
    }
    return removed;
}


void GroupHDF5::dataArrays(const std::vector<DataArray> &data_arrays) {

    // extract vectors of names from vectors of new & old references
    std::vector<std::string> names_new(data_arrays.size());
    transform(data_arrays.begin(), data_arrays.end(), names_new.begin(), util::toName<DataArray>);
    //FIXME: issue 473
    std::vector<DataArray> refs_old(static_cast<size_t>(dataArrayCount()));
    for (size_t i = 0; i < refs_old.size(); i++) refs_old[i] = getDataArray(i);
    std::vector<std::string> names_old(refs_old.size());
    transform(refs_old.begin(), refs_old.end(), names_old.begin(), util::toName<DataArray>);

    // sort them
    std::sort(names_new.begin(), names_new.end());
    std::sort(names_new.begin(), names_new.end());

    // get names only in names_new (add), names only in names_old (remove) & ignore rest
    std::vector<std::string> names_add;
    std::vector<std::string> names_rem;
    std::set_difference(names_new.begin(), names_new.end(), names_old.begin(), names_old.end(),
                        std::inserter(names_add, names_add.begin()));
    std::set_difference(names_old.begin(), names_old.end(), names_new.begin(), names_new.end(),
                        std::inserter(names_rem, names_rem.begin()));

    // check if all new data_arrays exist & add them
    auto blck = dynamic_pointer_cast<BlockHDF5>(block());
    for (auto name : names_add) {
        if (!blck->hasDataArray(name))
            throw std::runtime_error("One or more data arrays do not exist in this block!");
        addDataArray(blck->getDataArray(name)->id());
    }
    // remove references
    for (auto name : names_rem) {
        if (!blck->hasDataArray(name))
            removeDataArray(blck->getDataArray(name)->id());
    }
}

} // hdf5
} // nix
