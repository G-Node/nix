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

namespace nix {

class Section;

namespace base {

/**
 * @brief Interface for classes that can be associated with additional
 * metadata.
 *
 * See {@link nix::base::EntityWithMetadata} for a more detailed description.
 */
class NIXAPI IEntityWithMetadata : virtual public INamedEntity {

public:

    /**
     * @brief Get metadata associated with this entity.
     *
     * @return The associated section, if no such section exists an
     *         uninitialized {@link nix::Section} will be returned.
     */
    virtual Section metadata() const = 0;

    /**
     * @brief Associate the entity with some metadata.
     *
     * Calling this method will replace previously stored information.
     *
     * @param id the id of the {@link nix::Section} that should be associated
     *                    with this entity.
     */
    virtual void metadata(const std::string &id) = 0;


    /**
     * @brief Remove associated metadata from the entity.
     *
     * This method just removes the association between the respective
     * {@link nix::Section} but will not remove it from the file.
     * 
     * @param t 
     */
    virtual void metadata(const none_t t) = 0;


    /**
     * Destructor
     */
    virtual ~IEntityWithMetadata() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_ENTITY_WITH_METADATA_H
