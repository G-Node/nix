// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_ENTITY_HDF5_H
#define NIX_ENTITY_HDF5_H

#include <string>
#include <memory>

#include <nix/base/IFile.hpp>

#include <nix/base/IEntity.hpp>
#include <nix/hdf5/Group.hpp>


namespace nix {
namespace hdf5 {


/**
 * HDF5 implementation of IEntity
 */
class EntityHDF5 : virtual public base::IEntity {

private:

    std::shared_ptr<base::IFile>  entity_file;
    Group         entity_group;
    std::string   entity_id;

public:

    EntityHDF5(std::shared_ptr<base::IFile> file, Group group, const std::string &id);


    EntityHDF5(std::shared_ptr<base::IFile> file, Group group, const std::string &id, time_t time);


    std::string id() const;


    time_t updatedAt() const;


    time_t createdAt() const;


    void setUpdatedAt();


    void forceUpdatedAt();


    void setCreatedAt();


    void forceCreatedAt(time_t t);

    // TODO do we really need swap?
    void swap(EntityHDF5 &other);


    bool operator==(const EntityHDF5 &other) const;


    bool operator!=(const EntityHDF5 &other) const;


    virtual ~EntityHDF5();

protected:

    std::shared_ptr<base::IFile> file() const {
        return entity_file;
    }

    Group group() const {
        return entity_group;
    }

};


} // namespace hdf5
} // namespace nix

#endif // NIX_ENTITY_HDF5_H
