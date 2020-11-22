// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "BaseTagHDF5.hpp"
#include <nix/NDArray.hpp>
#include <nix/util/util.hpp>
#include "DataArrayHDF5.hpp"
#include "BlockHDF5.hpp"
#include "FeatureHDF5.hpp"

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
    std::string id = block()->resolveEntityId({name_or_id, ObjectType::DataArray});
    return refs_group(false) ? refs_group(false)->hasGroup(id) : false;
}


ndsize_t BaseTagHDF5::referenceCount() const {
    boost::optional<H5Group> g = refs_group(false);
    return g ? g->objectCount() : size_t(0);
}


std::shared_ptr<IDataArray>  BaseTagHDF5::getReference(const std::string &name_or_id) const {
    std::shared_ptr<IDataArray> da;
    boost::optional<H5Group> g = refs_group(false);

    std::string id = block()->resolveEntityId({name_or_id, ObjectType::DataArray});
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

    if (!block()->hasEntity({name_or_id, ObjectType::DataArray}))
        throw std::runtime_error("BaseTagHDF5::addReference: DataArray not found in block!");

    auto target = std::dynamic_pointer_cast<DataArrayHDF5>(block()->getEntity({name_or_id, ObjectType::DataArray}));

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
    while (referenceCount() > 0) {
        removeReference(getReference(0)->id());
    }

    for (const auto &ref : refs_new) {
        addReference(ref.id());
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
    // FIXME
    if (g) {
        boost::optional<H5Group> group = g->findGroupByNameOrAttribute("name", name_or_id);
        if (group)
            feature = std::make_shared<FeatureHDF5>(file(), block(), group.get());
        else {
            for (ndsize_t i = 0; i < g->objectCount(); i++) {
                H5Group gr = g->openGroup(g->objectName(i), false);
                std::shared_ptr<FeatureHDF5> feat = std::make_shared<FeatureHDF5>(file(), block(), gr);
                std::shared_ptr<base::IDataArray> da = feat->data();
                if (da->name() == name_or_id || da->id() == name_or_id) {
                    feature = std::make_shared<FeatureHDF5>(file(), block(), gr);
                    break;
                }
            }
        }
    }
    return feature;
}


std::shared_ptr<IFeature>  BaseTagHDF5::getFeature(ndsize_t index) const {
    boost::optional<H5Group> g = feature_group(false);
    std::string id = g->objectName(index);
    return getFeature(id);
}


std::shared_ptr<IFeature>  BaseTagHDF5::createFeature(const std::string &name_or_id, LinkType link_type, TargetType target_type) {
    bool entity_valid = true;
    std::shared_ptr<IFeature> ptr;
    switch(target_type) {
    case TargetType::DataArray: 
        entity_valid = block()->hasEntity({name_or_id, ObjectType::DataArray});
        break;
    case TargetType::DataFrame:
        entity_valid = block()->hasEntity({name_or_id, ObjectType::DataFrame});
        break;
    }
    if (!entity_valid) {
        throw std::runtime_error("DataArray/DataFrame not found in Block!");
    }
    std::string rep_id = util::createId();
    boost::optional<H5Group> g = feature_group(true);
    H5Group group = g->openGroup(rep_id, true);
    
    switch(target_type) {
    case TargetType::DataArray: { 
            DataArray data = std::dynamic_pointer_cast<IDataArray>(block()->getEntity({name_or_id, ObjectType::DataArray}));
            ptr = std::make_shared<FeatureHDF5>(file(), block(), group, rep_id, data, link_type);
            break;
        }
    case TargetType::DataFrame: {
            entity_valid = block()->hasEntity({name_or_id, ObjectType::DataFrame});
            DataFrame data = std::dynamic_pointer_cast<IDataFrame>(block()->getEntity({name_or_id, ObjectType::DataFrame}));
            ptr = std::make_shared<FeatureHDF5>(file(), block(), group, rep_id, data, link_type);
            break;
        }
    }
    return ptr;
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
