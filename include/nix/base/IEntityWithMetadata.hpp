// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_ENTITY_WITH_METADATA_H
#define NIX_I_ENTITY_WITH_METADATA_H

#include <nix/base/INamedEntity.hpp>
#include <nix/base/ISection.hpp>

namespace nix {
namespace base {

/**
 * @brief Interface for classes that can be associated with additional
 * metadata.
 *
 * See {@link nix::base::EntityWithMetadata} for a more detailed description.
 */
class NIXAPI IEntityWithMetadata : virtual public INamedEntity {

public:

    virtual std::shared_ptr<ISection> metadata() const = 0;


    virtual void metadata(const std::string &id) = 0;


    virtual void metadata(const none_t t) = 0;


    virtual ~IEntityWithMetadata() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_ENTITY_WITH_METADATA_H
