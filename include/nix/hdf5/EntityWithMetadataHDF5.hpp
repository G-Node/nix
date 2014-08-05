// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_ENTITY_WITH_METADATA_HDF5_H
#define NIX_ENTITY_WITH_METADATA_HDF5_H

#include <string>

#include <nix/base/ISection.hpp>
#include <nix/hdf5/NamedEntityHDF5.hpp>

namespace nix {
namespace hdf5 {

/**
 * HDF5 implementation of IEntityWithMetadata.
 */
class EntityWithMetadataHDF5 : virtual public base::IEntityWithMetadata, public NamedEntityHDF5 {

public:

    /**
     * Standard constructor
     */
    EntityWithMetadataHDF5(File file, Group group, const std::string &id,
                           const std::string &type, const std::string &name);

    /**
     * Standard constructor that preserves the creation time.
     */
    EntityWithMetadataHDF5(File file, Group group, const std::string &id,
                           const std::string &type, const std::string &name, time_t time);


    std::shared_ptr<base::ISection> metadata() const;


    void metadata(const std::string &id);


    void metadata(const none_t t);


    virtual ~EntityWithMetadataHDF5();

};


} // namespace hdf5
} // namespace nix

#endif // NIX_ENTITY_WITH_METADATA_HDF5_H
