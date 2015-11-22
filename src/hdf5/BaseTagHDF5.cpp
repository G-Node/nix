// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/BaseTagHDF5.hpp>
#include <nix/NDArray.hpp>
#include <nix/util/util.hpp>
#include <nix/hdf5/DataArrayHDF5.hpp>
#include <nix/hdf5/BlockHDF5.hpp>
#include <nix/hdf5/FeatureHDF5.hpp>

using namespace nix::base;

namespace nix {
namespace hdf5 {


BaseTagHDF5::BaseTagHDF5(const std::shared_ptr<IFile> &file, const std::shared_ptr<IBlock> &block, const H5Group &group)
    : EntityWithSourcesHDF5(file, block, group)
{
    feature_group = this->group().openOptGroup("features");
    refs_group = this->group().openOptGroup("references");
}


BaseTagHDF5::BaseTagHDF5(const std::shared_ptr<IFile> &file, const std::shared_ptr<IBlock> &block, const H5Group &group,
                         const std::string &id, const std::string &type, const std::string &name)
    : BaseTagHDF5(file, block, group, id, type, name, util::getTime())
{
}


BaseTagHDF5::BaseTagHDF5(const std::shared_ptr<IFile> &file, const std::shared_ptr<IBlock> &block, const H5Group &group,
                         const std::string &id, const std::string &type, const std::string &name, time_t time)
    : EntityWithSourcesHDF5(file, block, group, id, type, name, time)
{
    feature_group = this->group().openOptGroup("features");
    refs_group = this->group().openOptGroup("references");
}

//--------------------------------------------------
// Methods concerning references.
//--------------------------------------------------

bool BaseTagHDF5::hasReference(const std::string &name_or_id) const {
    std::string id = name_or_id;

    if (!util::looksLikeUUID(name_or_id) && block()->hasDataArray(name_or_id)) {
        id = block()->getDataArray(name_or_id)->id();
    }

    return refs_group(false) ? refs_group(false)->hasGroup(id) : false;
}


ndsize_t BaseTagHDF5::referenceCount() const {
    boost::optional<H5Group> g = refs_group(false);
    return g ? g->objectCount() : size_t(0);
}


std::shared_ptr<IDataArray>  BaseTagHDF5::getReference(const std::string &name_or_id) const {
    std::shared_ptr<IDataArray> da;
    boost::optional<H5Group> g = refs_group(false);

    std::string id = name_or_id;

    if (!util::looksLikeUUID(name_or_id) && block()->hasDataArray(name_or_id)) {
        id = block()->getDataArray(name_or_id)->id();
    }

    if (g && hasReference(id)) {
        H5Group group = g->openGroup(id);
        da = std::make_shared<DataArrayHDF5>(file(), block(), group);
    }

    return da;
}

std::shared_ptr<IDataArray>  BaseTagHDF5::getReference(ndsize_t index) const {
    boost::optional<H5Group> g = refs_group(false);
    std::string id = g ? g->objectName(index) : "";
    return getReference(id);
}

void BaseTagHDF5::addReference(const std::string &name_or_id) {
    boost::optional<H5Group> g = refs_group(true);

    if (!block()->hasDataArray(name_or_id))
        throw std::runtime_error("BaseTagHDF5::addReference: DataArray not found in block!");

    auto target = std::dynamic_pointer_cast<DataArrayHDF5>(block()->getDataArray(name_or_id));

    g->createLink(target->group(), target->id());
}


bool BaseTagHDF5::removeReference(const std::string &name_or_id) {
    boost::optional<H5Group> g = refs_group(false);
    bool removed = false;

    if (g && hasReference(name_or_id)) {
        std::shared_ptr<IDataArray> reference = getReference(name_or_id);

        g->removeGroup(reference->id());
        removed = true;
    }

    return removed;
}


void BaseTagHDF5::references(const std::vector<DataArray> &refs_new) {

    // extract vectors of names from vectors of new & old references
    std::vector<std::string> names_new(refs_new.size());
    std::transform(refs_new.begin(), refs_new.end(), names_new.begin(), util::toName<DataArray>);

    std::vector<DataArray> refs_old(static_cast<size_t>(referenceCount()));
    for (size_t i = 0; i < refs_old.size(); i++) refs_old[i] = getReference(i);
    std::vector<std::string> names_old(refs_old.size());
    std::transform(refs_old.begin(), refs_old.end(), names_old.begin(), util::toName<DataArray>);

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

    // check if all new references exist & add sources
    auto blck = std::dynamic_pointer_cast<BlockHDF5>(block());
    for (auto name : names_add) {
        if (!blck->hasDataArray(name))
            throw std::runtime_error("One or more data arrays do not exist in this block!");
        addReference(blck->getDataArray(name)->id());
    }
    // remove references
    for (auto name : names_rem) {
        if (!blck->hasDataArray(name))
            removeReference(blck->getDataArray(name)->id());
    }
}

//--------------------------------------------------
// Methods concerning features.
//--------------------------------------------------

bool BaseTagHDF5::hasFeature(const std::string &name_or_id) const {
    return getFeature(name_or_id) != nullptr;
}


ndsize_t BaseTagHDF5::featureCount() const {
    boost::optional<H5Group> g = feature_group(false);
    return g ? g->objectCount() : size_t(0);
}


std::shared_ptr<IFeature> BaseTagHDF5::getFeature(const std::string &name_or_id) const {
    std::shared_ptr<FeatureHDF5> feature;
    boost::optional<H5Group> g = feature_group(false);

    if (g) {
        boost::optional<H5Group> group = g->findGroupByNameOrAttribute("entity_id", name_or_id);
        if (group)
            feature = std::make_shared<FeatureHDF5>(file(), block(), group.get());
    }

    return feature;
}


std::shared_ptr<IFeature>  BaseTagHDF5::getFeature(ndsize_t index) const {
    boost::optional<H5Group> g = feature_group(false);
    std::string id = g->objectName(index);
    return getFeature(id);
}


std::shared_ptr<IFeature>  BaseTagHDF5::createFeature(const std::string &name_or_id, LinkType link_type) {
    if(!block()->hasDataArray(name_or_id)) {
        throw std::runtime_error("DataArray not found in Block!");
    }
    std::string rep_id = util::createId();
    boost::optional<H5Group> g = feature_group(true);

    H5Group group = g->openGroup(rep_id, true);
    DataArray data = block()->getDataArray(name_or_id);
    return std::make_shared<FeatureHDF5>(file(), block(), group, rep_id, data, link_type);
}


bool BaseTagHDF5::deleteFeature(const std::string &name_or_id) {
    boost::optional<H5Group> g = feature_group(false);
    bool deleted = false;

    if (g && hasFeature(name_or_id)) {
        std::shared_ptr<IFeature> feature = getFeature(name_or_id);

        g->removeGroup(feature->id());
        deleted = true;
    }

    return deleted;
}


BaseTagHDF5::~BaseTagHDF5() {}

} // ns nix::hdf5
} // ns nix