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

    /**
     * @brief Get the number of sources associated with this entity.
     *
     * @return The number sources.
     */
    virtual size_t sourceCount() const = 0;

    /**
     * @brief Associate a new source with the entity.
     *
     * If a source with the given id already is associated with the
     * entity, the call will have no effect.
     *
     * @param id      The id of the source.
     */
    virtual void addSource(const std::string &id) = 0;

    /**
     * @brief Checks if a specific source is associated with this entity.
     *
     * @param id      The source id to check.
     *
     * @return True if the source is associated with this entity, false otherwise.
     */
    virtual bool hasSource(const std::string &id) const = 0;

    /**
     * @brief Remove a source from the list of associated sources.
     *
     * This method just removes the association between the entity and the source.
     * The source itself will not be deleted from the file.
     *
     * @param id      The id of the source to remove.
     *
     * @return True if the source was removed, false otherwise.
     */
    virtual bool removeSource(const std::string &id) = 0;

    /**
     * @brief Set all sources associations for this entity.
     *
     * All previously existing associations will be overwritten.
     *
     * @param sources     A vector with all sources.
     */
    virtual void sources(const std::vector<Source> &sources) = 0;

    /**
     * @brief Returns an associated source identified by its id.
     *
     * @param id        The id of the associated source.
     */
    virtual Source getSource(const std::string &id) const = 0;

    /**
     * @brief Retrieves an associated source identified by its index.
     *
     * @param index        The index of the associated source.
     *
     * @return The source with the given id. If it doesn't exist an exception
     *         will be thrown.
     */
    virtual Source getSource(const size_t index) const = 0;
    
    /**
     * Destructor
     */
    virtual ~IEntityWithSources() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_ENTITY_WITH_SOURCES_H
