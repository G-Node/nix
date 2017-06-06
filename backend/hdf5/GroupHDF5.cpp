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
    case ObjectType::MultiTag:
        p = multi_tag_group(true);
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
    case ObjectType::MultiTag: {
        std::shared_ptr<MultiTagHDF5> t;
        if (eg) {
            t = std::make_shared<MultiTagHDF5>(file(), block(), *eg);
        }
        return t;
    }
    default:
        return std::shared_ptr<base::IEntity>();
    }
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

} // hdf5
} // nix
