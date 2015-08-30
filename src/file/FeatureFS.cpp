// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/file/FeatureFS.hpp>

#include <nix/util/util.hpp>
#include <nix/DataArray.hpp>
#include <nix/file/DataArrayFS.hpp>


using namespace std;
using namespace nix::base;

namespace nix {
namespace file {


string linkTypeToString(LinkType link_type) {
    static vector<string> type_names = {"tagged", "untagged", "indexed"};
    return type_names[static_cast<int>(link_type)];
}


LinkType linkTypeFromString(const string &str) {
    if (str == "tagged")
        return LinkType::Tagged;
    else if (str == "untagged")
        return LinkType::Untagged;
    else if (str == "indexed")
        return LinkType::Indexed;
    else
        throw runtime_error("Unable to create a LinkType from the string: " + str);
}


FeatureFS::FeatureFS(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const string &loc)
    : EntityFS(file, loc), block(block)
{
}

FeatureFS::FeatureFS(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const string &loc,
                     const string &id, DataArray data, LinkType link_type)
    : FeatureFS(file, block, loc, id, data, link_type, util::getTime())
{
}


FeatureFS::FeatureFS(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const string &loc,
                     const string &id, DataArray data, LinkType link_type, time_t time)
    : EntityFS(file, (boost::filesystem::path(loc) / boost::filesystem::path(id)).string(), id, time), block(block)
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
    if (!block->hasDataArray(name_or_id))
        throw std::runtime_error("FeatureFS::data: DataArray not found in block!");
    if (hasObject("data"))
        removeObjectByNameOrAttribute("name", "data");
    auto target = dynamic_pointer_cast<DataArrayFS>(block->getDataArray(name_or_id));
    createDirectoryLink(target->location(), "data");
    forceUpdatedAt();
}


shared_ptr<IDataArray> FeatureFS::data() const {
    shared_ptr<IDataArray> da;
    boost::optional<boost::filesystem::path> p = findByNameOrAttribute("name", "data");
    if (p) {
        DataArrayFS a(file(), block, p->string());
        return make_shared<DataArrayFS>(a);
    }
    return da;
}


LinkType FeatureFS::linkType() const {
    if (hasAttr("link_type")) {
        string link_type;
        getAttr("link_type", link_type);
        return linkTypeFromString(link_type);
    } else {
        throw MissingAttr("data");
    }
}


FeatureFS::~FeatureFS() {}

} // ns nix::file
} // ns nix
