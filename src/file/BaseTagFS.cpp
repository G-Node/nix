// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/file/BaseTagFS.hpp>

#include <nix/NDArray.hpp>
#include <nix/file/DataArrayFS.hpp>
#include <nix/file/BlockFS.hpp>
#include <nix/file/FeatureFS.hpp>

using namespace std;
using namespace nix::base;

namespace nix {
namespace file {


BaseTagFS::BaseTagFS(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const string &loc)
    : EntityWithSourcesFS(file, block, loc)
{
    createSubFolders(file);
}


BaseTagFS::BaseTagFS(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const string &loc,
                     const string &id, const std::string &type, const string &name)
    : BaseTagFS(file, block, loc, id, type, name, util::getTime())
{
}


BaseTagFS::BaseTagFS(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const string &loc,
                     const std::string &id, const std::string &type, const string &name, time_t time)
    : EntityWithSourcesFS(file, block, loc, id, type, name, time)
{
    createSubFolders(file);
}


void BaseTagFS::createSubFolders(const shared_ptr<base::IFile> &file) {
    boost::filesystem::path refs("references");
    boost::filesystem::path feats("features");
    boost::filesystem::path p(location());

    refs_group = Directory(p / refs, file->fileMode());
    feature_group = Directory(p / feats, file->fileMode());
}

//--------------------------------------------------
// Methods concerning references.
//--------------------------------------------------

bool BaseTagFS::hasReference(const std::string &name_or_id) const {
    std::string id = name_or_id;

    if (!util::looksLikeUUID(name_or_id) && block()->hasDataArray(name_or_id)) {
        id = block()->getDataArray(name_or_id)->id();
    }

    return refs_group.hasObject(id);
}


ndsize_t BaseTagFS::referenceCount() const {
    return refs_group.subdirCount();
}


shared_ptr<IDataArray>  BaseTagFS::getReference(const std::string &name_or_id) const {
    shared_ptr<IDataArray> da;

    std::string id = name_or_id;
    if (!util::looksLikeUUID(name_or_id) && block()->hasDataArray(name_or_id)) {
        id = block()->getDataArray(name_or_id)->id();
    }

    if (hasReference(id)) {
        boost::optional<boost::filesystem::path> path = refs_group.findByNameOrAttribute("name", name_or_id);
        if (path) {
            return make_shared<DataArrayFS>(file(), block(), path->string());
        }
    }
    return da;
}


shared_ptr<IDataArray>  BaseTagFS::getReference(size_t index) const {
    if(index > referenceCount()) {
        throw OutOfBounds("No reference at given index", index);
    }
    boost::filesystem::path p = refs_group.sub_dir_by_index(index);
    return make_shared<DataArrayFS>(file(), block(), p.string());
}


void BaseTagFS::addReference(const std::string &name_or_id) {
    if (name_or_id.empty())
        throw EmptyString("addReference");

    if (!block()->hasDataArray(name_or_id))
        throw std::runtime_error("BaseTagFS::addReference: DataArray not found in block!");

    auto target = dynamic_pointer_cast<DataArrayFS>(block()->getDataArray(name_or_id));
    refs_group.createDirectoryLink(target->location(), target->id());
}


bool BaseTagFS::removeReference(const std::string &name_or_id) {
    return refs_group.removeObjectByNameOrAttribute("name", name_or_id);
}


void BaseTagFS::references(const std::vector<DataArray> &refs_new) {
    // extract vectors of names from vectors of new & old references
    std::vector<std::string> names_new(refs_new.size());
    transform(refs_new.begin(), refs_new.end(), names_new.begin(), util::toName<DataArray>);
    //FIXME: issue 473
    std::vector<DataArray> refs_old(static_cast<size_t>(referenceCount()));
    for (size_t i = 0; i < refs_old.size(); i++) refs_old[i] = getReference(i);
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

    // check if all new references exist & add sources
    auto blck = dynamic_pointer_cast<BlockFS>(block());
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

bool BaseTagFS::hasFeature(const string &name_or_id) const {
    return !feature_group.findByNameOrAttribute("entity_id", name_or_id)->empty();
}


ndsize_t BaseTagFS::featureCount() const {
    return feature_group.subdirCount();
}


shared_ptr<IFeature> BaseTagFS::getFeature(const std::string &name_or_id) const {
    shared_ptr<IFeature> feature;
    boost::optional<boost::filesystem::path> p = feature_group.findByNameOrAttribute("name", name_or_id);
    if (p) {
        return make_shared<FeatureFS>(file(), block(), p->string());
    }
    return feature;
}


shared_ptr<IFeature>  BaseTagFS::getFeature(size_t index) const {
    if (index >= featureCount()) {
        throw OutOfBounds("Trying to access a feature with an invalid index!");
    }
    boost::filesystem::path id = feature_group.sub_dir_by_index(index);
    return getFeature(id.filename().string());
}


shared_ptr<IFeature>  BaseTagFS::createFeature(const std::string &name_or_id, LinkType link_type) {
    if(!block()->hasDataArray(name_or_id)) {
        throw std::runtime_error("DataArray not found in Block!");
    }
    string rep_id = util::createId();
    DataArray a = block()->getDataArray(name_or_id);
    return make_shared<FeatureFS>(file(), block(), feature_group.location(), rep_id, a, link_type);
}


bool BaseTagFS::deleteFeature(const string &name_or_id) {
    return feature_group.removeObjectByNameOrAttribute("name", name_or_id);
}


BaseTagFS::~BaseTagFS() {}

} // ns nix::file
} // ns nix