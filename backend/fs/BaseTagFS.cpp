// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "BaseTagFS.hpp"

#include <nix/NDArray.hpp>
#include "DataArrayFS.hpp"
#include "BlockFS.hpp"
#include "FeatureFS.hpp"

namespace bfs= boost::filesystem;

namespace nix {
namespace file {


BaseTagFS::BaseTagFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                     const std::string &loc)
    : EntityWithSourcesFS(file, block, loc)
{
    createSubFolders(file);
}


BaseTagFS::BaseTagFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                     const std::string &loc, const std::string &id, const std::string &type, const std::string &name)
    : BaseTagFS(file, block, loc, id, type, name, util::getTime())
{
}


BaseTagFS::BaseTagFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                     const std::string &loc, const std::string &id, const std::string &type, const std::string &name, time_t time)
    : EntityWithSourcesFS(file, block, loc, id, type, name, time)
{
    createSubFolders(file);
}


void BaseTagFS::createSubFolders(const std::shared_ptr<base::IFile> &file) {
    bfs::path refs("references");
    bfs::path feats("features");
    bfs::path p(location());

    refs_group = Directory(p / refs, file->fileMode());
    feature_group = Directory(p / feats, file->fileMode());
}

//--------------------------------------------------
// Methods concerning references.
//--------------------------------------------------

bool BaseTagFS::hasReference(const std::string &name_or_id) const {
    std::string id = name_or_id;

    if (!util::looksLikeUUID(name_or_id) && block()->hasEntity({name_or_id, ObjectType::DataArray})) {
        id = block()->getEntity({name_or_id, ObjectType::DataArray})->id();
    }

    return refs_group.hasObject(id);
}


ndsize_t BaseTagFS::referenceCount() const {
    return refs_group.subdirCount();
}


std::shared_ptr<base::IDataArray>  BaseTagFS::getReference(const std::string &name_or_id) const {
    std::shared_ptr<base::IDataArray> da;

    std::string id = name_or_id;
    if (!util::looksLikeUUID(name_or_id) && block()->hasEntity({name_or_id, ObjectType::DataArray})) {
        id = block()->getEntity({name_or_id, ObjectType::DataArray})->id();
    }

    if (hasReference(id)) {
        boost::optional<bfs::path> path = refs_group.findByNameOrAttribute("name", name_or_id);
        if (path) {
            return std::make_shared<DataArrayFS>(file(), block(), path->string());
        }
    }
    return da;
}


std::shared_ptr<base::IDataArray>  BaseTagFS::getReference(ndsize_t index) const {
    bfs::path p = refs_group.sub_dir_by_index(index);
    return std::make_shared<DataArrayFS>(file(), block(), p.string());
}


void BaseTagFS::addReference(const std::string &name_or_id) {
    if (!block()->hasEntity({name_or_id, ObjectType::DataArray}))
        throw std::runtime_error("BaseTagFS::addReference: DataArray not found in block!");

    auto target = std::dynamic_pointer_cast<DataArrayFS>(block()->getEntity({name_or_id, ObjectType::DataArray}));
    refs_group.createDirectoryLink(target->location(), target->id());
}


bool BaseTagFS::removeReference(const std::string &name_or_id) {
    return refs_group.removeObjectByNameOrAttribute("name", name_or_id);
}


void BaseTagFS::references(const std::vector<DataArray> &refs_new) {
    auto cmp = [](const DataArray &a, const DataArray& b) { return a.name() < b.name(); };
    std::vector<DataArray> new_arrays(refs_new);
    size_t ref_count = nix::check::fits_in_size_t(referenceCount(), "referenceCount() failed; count > size_t.");
    std::vector<DataArray> old_arrays(ref_count);
    for (size_t i = 0; i < old_arrays.size(); i++) {
        old_arrays[i] = getReference(i);
    }
    std::sort(new_arrays.begin(), new_arrays.end(), cmp);
    std::sort(old_arrays.begin(), old_arrays.end(), cmp);
    std::vector<DataArray> add;
    std::vector<DataArray> rem;

    std::set_difference(new_arrays.begin(), new_arrays.end(), old_arrays.begin(), old_arrays.end(),
                        std::inserter(add, add.begin()), cmp);
    std::set_difference(old_arrays.begin(), old_arrays.end(), new_arrays.begin(), new_arrays.end(),
                        std::inserter(rem, rem.begin()), cmp);

    auto blck = std::dynamic_pointer_cast<BlockFS>(block());
    for (const auto &da : add) {
        auto a = std::dynamic_pointer_cast<DataArrayFS>(blck->getEntity(da));
        if (!a)
            throw std::runtime_error("One or more data arrays do not exist in this block!");
        addReference(a->id());
    }
    for (const auto &da : rem) {
        removeReference(da.id());
    }
}

//--------------------------------------------------
// Methods concerning features.
//--------------------------------------------------

bool BaseTagFS::hasFeature(const std::string &name_or_id) const {
    return getFeature(name_or_id) != nullptr;
}


ndsize_t BaseTagFS::featureCount() const {
    return feature_group.subdirCount();
}


std::shared_ptr<base::IFeature> BaseTagFS::getFeature(const std::string &name_or_id) const {
    std::shared_ptr<base::IFeature> feature;
    boost::optional<bfs::path> p = feature_group.findByNameOrAttribute("name", name_or_id);
    if (p) {
        return std::make_shared<FeatureFS>(file(), block(), p->string());
    } else {
        for (ndsize_t i = 0; i < feature_group.subdirCount(); i++) {
            bfs::path dir = feature_group.sub_dir_by_index(i);
            std::shared_ptr<base::IFeature> feat = std::make_shared<FeatureFS>(file(), block(), dir.string());
            std::shared_ptr<base::IDataArray> da = feat->data();
            if (da->name() == name_or_id || da->id() == name_or_id) {
                feature = std::make_shared<FeatureFS>(file(), block(), dir.string());
                break;
            }
        }
    }
    return feature;
}


std::shared_ptr<base::IFeature>  BaseTagFS::getFeature(ndsize_t index) const {
    bfs::path id = feature_group.sub_dir_by_index(index);
    return getFeature(id.filename().string());
}


std::shared_ptr<base::IFeature>  BaseTagFS::createFeature(const std::string &name_or_id, LinkType link_type) {
    if(!block()->hasEntity({name_or_id, ObjectType::DataArray})) {
        throw std::runtime_error("DataArray not found in Block!");
    }
    std::string rep_id = util::createId();
    DataArray a = std::dynamic_pointer_cast<base::IDataArray>(block()->getEntity({name_or_id, ObjectType::DataArray}));
    return std::make_shared<FeatureFS>(file(), block(), feature_group.location(), rep_id, a, link_type);
}


bool BaseTagFS::deleteFeature(const std::string &name_or_id) {
    return feature_group.removeObjectByNameOrAttribute("name", name_or_id);
}


BaseTagFS::~BaseTagFS() {}

} // ns nix::file
} // ns nix
