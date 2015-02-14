// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_ENTITY_WITH_SOURCES_H
#define NIX_I_ENTITY_WITH_SOURCES_H

#include <nix/NDSize.hpp>

#include <nix/base/ISource.hpp>
#include <nix/base/IEntityWithMetadata.hpp>

#include <string>
#include <vector>
#include <memory>

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

    virtual ndsize_t sourceCount() const = 0;


    virtual void addSource(const std::string &id) = 0;


    virtual bool hasSource(const std::string &id) const = 0;


    virtual bool removeSource(const std::string &id) = 0;

    // TODO evaluate if Source can be replaced by shared_ptr<ISource>
    //      or if this could be moved to the front-end
    virtual void sources(const std::vector<Source> &sources) = 0;


    virtual std::shared_ptr<ISource> getSource(const std::string &id) const = 0;


    virtual std::shared_ptr<ISource> getSource(const size_t index) const = 0;


    virtual ~IEntityWithSources() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_ENTITY_WITH_SOURCES_H
