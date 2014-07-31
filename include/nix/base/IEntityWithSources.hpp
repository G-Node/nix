// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_ENTITY_WITH_SOURCES_H
#define NIX_I_ENTITY_WITH_SOURCES_H

#include <string>
#include <vector>

#include <nix/base/IEntityWithMetadata.hpp>

namespace nix {

class Source;

namespace base {

/**
 * @brief Interface for entities that can be associated with sources.
 *
 * See {@link nix::base::EntityWithSources} for a more detailed description.
 */
class NIXAPI IEntityWithSources : virtual public IEntityWithMetadata {

public:

    virtual size_t sourceCount() const = 0;


    virtual void addSource(const std::string &id) = 0;


    virtual bool hasSource(const std::string &id) const = 0;


    virtual bool removeSource(const std::string &id) = 0;


    virtual void sources(const std::vector<Source> &sources) = 0;


    virtual Source getSource(const std::string &id) const = 0;


    virtual Source getSource(const size_t index) const = 0;


    virtual ~IEntityWithSources() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_ENTITY_WITH_SOURCES_H
