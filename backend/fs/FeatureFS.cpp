// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "FeatureFS.hpp"

#include <nix/util/util.hpp>
#include <nix/DataArray.hpp>
#include "DataArrayFS.hpp"

namespace bfs = boost::filesystem;

namespace nix {
namespace file {


std::string linkTypeToString(LinkType link_type) {
    static std::vector<std::string> type_names = {"tagged", "untagged", "indexed"};
    return type_names[static_cast<int>(link_type)];
}


LinkType linkTypeFromString(const std::string &str) {
    if (str == "tagged")
        return LinkType::Tagged;
    else if (str == "untagged")
        return LinkType::Untagged;
    else if (str == "indexed")
        return LinkType::Indexed;
    else
        throw std::runtime_error("Unable to create a LinkType from the string: " + str);
}


FeatureFS::FeatureFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc)
    : EntityFS(file, loc), block(block)
{
}

FeatureFS::FeatureFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc,
                     const std::string &id, DataArray data, LinkType link_type)
    : FeatureFS(file, block, loc, id, data, link_type, util::getTime())
{
}


FeatureFS::FeatureFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc,
                     const std::string &id, DataArray data, LinkType link_type, time_t time)
    : EntityFS(file, (bfs::path(loc) / bfs::path(id)).string(), id, time), block(block)
{
    linkType(link_type);
    // TODO: the line below currently throws an exception if the DataArray
    // is not in block - to consider if we prefer copying it to the block
    this->data(data.id());
}


void FeatureFS::linkType(LinkType link_type) {
    // linkTypeToString will generate an error if link_type is invalid
    setAttr("link_type", linkTypeToString(link_type));
    forceUpdatedAt();
}


void FeatureFS::data(const std::string &name_or_id) {
    if (name_or_id.empty())
        throw EmptyString("data(id)");
    if (!block->hasEntity({name_or_id, ObjectType::DataArray}))
        throw std::runtime_error("FeatureFS::data: DataArray not found in block!");
    if (hasObject("data"))
        removeObjectByNameOrAttribute("name", "data");
    auto target = std::dynamic_pointer_cast<DataArrayFS>(block->getEntity({name_or_id, ObjectType::DataArray}));
    bfs::path p(location()), m("data");
    target->createLink(p / m);
    forceUpdatedAt();
}


std::shared_ptr<base::IDataArray> FeatureFS::data() const {
    std::shared_ptr<base::IDataArray> da;
    boost::optional<bfs::path> p = findByNameOrAttribute("name", "data");
    if (p) {
        DataArrayFS a(file(), block, p->string());
        return std::make_shared<DataArrayFS>(a);
    }
    return da;
}


LinkType FeatureFS::linkType() const {
    if (hasAttr("link_type")) {
        std::string link_type;
        getAttr("link_type", link_type);
        return linkTypeFromString(link_type);
    } else {
        throw MissingAttr("data");
    }
}


FeatureFS::~FeatureFS() {}

} // ns nix::file
} // ns nix
