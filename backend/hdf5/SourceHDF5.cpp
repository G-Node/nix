// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include "SourceHDF5.hpp"
#include <nix/Source.hpp>

using namespace std;
using namespace nix::base;

namespace nix {
namespace hdf5 {


SourceHDF5::SourceHDF5(const std::shared_ptr<IFile> &file,  const std::shared_ptr<IBlock> &block, const H5Group &group)
    : EntityWithMetadataHDF5(file, group), entity_block(block)
{
    source_group = this->group().openOptGroup("sources");
}
    
    
SourceHDF5::SourceHDF5(const shared_ptr<IFile> &file,  const std::shared_ptr<IBlock> &block, const H5Group &group, const std::string &id, const string &type, const string &name)
    : SourceHDF5(file, block, group, id, type, name, util::getTime())
{
}


SourceHDF5::SourceHDF5(const shared_ptr<IFile> &file,  const std::shared_ptr<IBlock> &block, const H5Group &group, const std::string &id, const string &type, const string &name, time_t time)
    : EntityWithMetadataHDF5(file, group, id, type, name, time), entity_block(block)
{
    source_group = this->group().openOptGroup("sources");
}


bool SourceHDF5::hasSource(const string &name_or_id) const {
    return getSource(name_or_id) != nullptr;
}


shared_ptr<ISource> SourceHDF5::getSource(const string &name_or_id) const {
    shared_ptr<SourceHDF5> source;
    std::optional<H5Group> g = source_group();

    if (g) {
        std::optional<H5Group> group = g->findGroupByNameOrAttribute("entity_id", name_or_id);
        if (group)
            source = make_shared<SourceHDF5>(file(), parentBlock(), *group);
    }

    return source;
}


shared_ptr<ISource> SourceHDF5::getSource(ndsize_t index) const {
    std::optional<H5Group> g = source_group();
    string name = g ? g->objectName(index) : "";
    return getSource(name);
}


ndsize_t SourceHDF5::sourceCount() const {
    std::optional<H5Group> g = source_group(false);
    return g ? g->objectCount() : size_t(0);
}


shared_ptr<ISource> SourceHDF5::createSource(const string &name, const string &type) {
    string id = util::createId();
    std::optional<H5Group> g = source_group(true);

    H5Group group = g->openGroup(name, true);
    return make_shared<SourceHDF5>(file(), parentBlock(), group, id, type, name);
}


bool SourceHDF5::deleteSource(const string &name_or_id) {
    std::optional<H5Group> g = source_group();
    bool deleted = false;
    
    if(g) {
        // call deleteSource on sources to trigger recursive call to all sub-sources
        if (hasSource(name_or_id)) {
            // get instance of source about to get deleted
            Source source = getSource(name_or_id);
            // loop through all child sources and call deleteSource on them
            for(auto &child : source.sources()) {
                source.deleteSource(child.id());
            }
            // if hasSource is true then source_group always exists
            deleted = g->removeAllLinks(source.name());
        }
    }

    return deleted;
}


shared_ptr<IFile> SourceHDF5::parentFile() const {
    return file();
}


std::shared_ptr<base::IBlock> SourceHDF5::parentBlock() const {
    return entity_block;
}

SourceHDF5::~SourceHDF5() {}

} // ns nix::hdf5
} // ns nix

