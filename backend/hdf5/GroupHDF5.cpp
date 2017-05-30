// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "GroupHDF5.hpp"
#include <nix/Block.hpp>
#include "TagHDF5.hpp"
#include "MultiTagHDF5.hpp"
#include "BlockHDF5.hpp"
#include "DataArrayHDF5.hpp"
#include <boost/range/irange.hpp>

using namespace nix::base;

namespace nix {
namespace hdf5 {

boost::optional<H5Group> GroupHDF5::groupForObjectType(ObjectType type) const {
    boost::optional<H5Group> p;

    switch (type) {
    case ObjectType::DataArray:
        p = data_array_group(true);
        break;
    case ObjectType::Tag:
        p = tag_group(true);
        break;
        //TODO
    default:
        p = boost::optional<H5Group>();
    }

    return p;
}


boost::optional<H5Group> GroupHDF5::findEntityGroup(const nix::Identity &ident) const {
    boost::optional<H5Group> p = groupForObjectType(ident.type());
    if (!p) {
        return p;
    }

    boost::optional<H5Group> g;
    const std::string &iname = ident.name();
    const std::string &iid = ident.id();

    bool haveName = !iname.empty();
    bool haveId = !iid.empty();

    if (!haveName && !haveId) {
        return g;
    }
    std::string needle = haveId ? iid : iname;
    bool foundNeedle = p->hasObject(needle);

    if (foundNeedle) {
        g = boost::make_optional(p->openGroup(needle, false));
    } else if (haveName) {
        g = p->findGroupByAttribute("name", iname);
    }

    if (g && haveName && haveId) {
        std::string ename;
        g->getAttr("name", ename);

        if (ename != iname) {
            return boost::optional<H5Group>();
        }
    }

    return g;
}


GroupHDF5::GroupHDF5(const std::shared_ptr<base::IFile> &file,
                     const std::shared_ptr<base::IBlock> &block,
                     const H5Group &h5group)
    : EntityWithSourcesHDF5(file, block, h5group) {
    data_array_group = this->group().openOptGroup("data_arrays");
    tag_group = this->group().openOptGroup("tags");
    multi_tag_group = this->group().openOptGroup("multi_tags");
}


GroupHDF5::GroupHDF5(const std::shared_ptr<base::IFile> &file,
                     const std::shared_ptr<base::IBlock> &block,
                     const H5Group &h5group, const std::string &id,
                     const std::string &type, const std::string &name)
    : GroupHDF5(file, block, h5group, id, type, name, util::getTime())  {

}


GroupHDF5::GroupHDF5(const std::shared_ptr<base::IFile> &file,
                     const std::shared_ptr<base::IBlock> &block,
                     const H5Group &h5group, const std::string &id,
                     const std::string &type, const std::string &name,
                     time_t time)
    : EntityWithSourcesHDF5(file, block, h5group, id, type, name, time) {
    data_array_group = this->group().openOptGroup("data_arrays");
    tag_group = this->group().openOptGroup("tags");
    multi_tag_group = this->group().openOptGroup("multi_tags");
}

bool GroupHDF5::hasEntity(const nix::Identity &ident) const {
    boost::optional<H5Group> p = findEntityGroup(ident);
    return !!p;
}

std::shared_ptr<base::IEntity> GroupHDF5::getEntity(const nix::Identity &ident) const {
    boost::optional<H5Group> eg = findEntityGroup(ident);

    switch (ident.type()) {
    case ObjectType::DataArray: {
        std::shared_ptr<DataArrayHDF5> da;
        if (eg) {
            da = std::make_shared<DataArrayHDF5>(file(), block(), *eg);
        }
        return da;
    }
    case ObjectType::Tag: {
        std::shared_ptr<TagHDF5> t;
        if (eg) {
            t = std::make_shared<TagHDF5>(file(), block(), *eg);
        }
        return t;
    }
    }
    return std::shared_ptr<base::IEntity>();
}

std::shared_ptr<base::IEntity>GroupHDF5::getEntity(ObjectType type, ndsize_t index) const {
    boost::optional<H5Group> eg = groupForObjectType(type);
    std::string name = eg ? eg->objectName(index) : "";
    return getEntity({name, "", type});
}


ndsize_t GroupHDF5::entityCount(ObjectType type) const {
    boost::optional<H5Group> g = groupForObjectType(type);
    return g ? g->objectCount() : ndsize_t(0);
}


bool GroupHDF5::removeEntity(const nix::Identity &ident) {
    boost::optional<H5Group> p = groupForObjectType(ident.type());
    boost::optional<H5Group> eg = findEntityGroup(ident);

    if (!p || !eg) {
        return false;
    }

    std::string eid;
    eg->getAttr("entity_id", eid);

    p->removeGroup(eid);
    return true;
}


void GroupHDF5::addEntity(const nix::Identity &ident) {
    boost::optional<H5Group> p = groupForObjectType(ident.type());
    if(!block()->hasEntity(ident)) {
        throw std::runtime_error("Entity do not exist in this block!");
    }

    auto target = std::dynamic_pointer_cast<EntityHDF5>(block()->getEntity(ident));
    p->createLink(target->group(), target->id());
}


bool GroupHDF5::hasMultiTag(const std::string &name_or_id) const {
    std::string id = block()->resolveEntityId({name_or_id, ObjectType::MultiTag});
    return multi_tag_group(false) ? multi_tag_group(false)->hasGroup(id) : false;
}


ndsize_t GroupHDF5::multiTagCount() const {
    boost::optional<H5Group> g = multi_tag_group(false);
    return g ? g->objectCount() : size_t(0);
}


void GroupHDF5::addMultiTag(const std::string &name_or_id) {
    boost::optional<H5Group> g = multi_tag_group(true);
    std::shared_ptr<IMultiTag> tag = block()->getEntity<IMultiTag>(name_or_id);

    if (!tag)
        throw std::runtime_error("GroupHDF5::addMultiTag: MultiTag not found in block!");

    auto target = std::dynamic_pointer_cast<MultiTagHDF5>(tag);
    g->createLink(target->group(), target->id());
}


std::shared_ptr<base::IMultiTag> GroupHDF5::getMultiTag(const std::string &name_or_id) const {
    std::shared_ptr<IMultiTag> da;
    boost::optional<H5Group> g = multi_tag_group(false);
    std::string id = block()->resolveEntityId({name_or_id, ObjectType::MultiTag});

    if (g && hasMultiTag(id)) {
        H5Group h5g = g->openGroup(id);
        da = std::make_shared<MultiTagHDF5>(file(), block(), h5g);
    }
    return da;
}


std::shared_ptr<IMultiTag>  GroupHDF5::getMultiTag(ndsize_t index) const {
    boost::optional<H5Group> g = multi_tag_group(false);
    std::string id = g ? g->objectName(index) : "";
    return getMultiTag(id);
}


bool GroupHDF5::removeMultiTag(const std::string &name_or_id) {
    boost::optional<H5Group> g = multi_tag_group(false);
    bool removed = false;

    if (g && hasMultiTag(name_or_id)) {
        std::shared_ptr<IMultiTag> mtag = getMultiTag(name_or_id);

        g->removeGroup(mtag->id());
        removed = true;
    }
    return removed;
}


void GroupHDF5::multiTags(const std::vector<MultiTag> &multi_tags) {
    auto cmp = [](const MultiTag &a, const MultiTag& b) { return a.name() < b.name(); };

    std::vector<MultiTag> new_tags(multi_tags);
    size_t tag_count = nix::check::fits_in_size_t(multiTagCount(), "multiTagCount() failed; count > size_t.");
    std::vector<MultiTag> old_tags(tag_count);
    for (size_t i = 0; i < old_tags.size(); i++) {
        old_tags[i] = getMultiTag(i);
    }
    std::sort(new_tags.begin(), new_tags.end(), cmp);
    std::sort(old_tags.begin(), old_tags.end(), cmp);
    std::vector<MultiTag> add;
    std::vector<MultiTag> rem;

    std::set_difference(new_tags.begin(), new_tags.end(), old_tags.begin(), old_tags.end(),
                        std::inserter(add, add.begin()), cmp);
    std::set_difference(old_tags.begin(), old_tags.end(), new_tags.begin(), new_tags.end(),
                        std::inserter(rem, rem.begin()), cmp);

    for (const auto &t : add) {
        MultiTag tag = block()->getEntity<IMultiTag>(t);
        if (!tag || tag.id() != t.id())
            throw std::runtime_error("One or more data multiTags do not exist in this block!");
        addMultiTag(t.id());
    }

    for (const auto &t : rem) {
        removeMultiTag(t.id());
    }
}

} // hdf5
} // nix
