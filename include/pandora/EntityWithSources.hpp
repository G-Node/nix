// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file EntityWithSources.hpp
 * @brief Declaration of the class EntityWithSources.
 */

#ifndef PAN_ENTITY_WITH_SOURCES_H_INCLUDED
#define PAN_ENTITY_WITH_SOURCES_H_INCLUDED

#include <string>
#include <vector>
#include <iostream>

#include <pandora/EntityWithMetadata.hpp>


namespace pandora {

class Block;
class Source;

/**
 * Base class for entities that are associated with sources.
 *
 * TODO: Refactoring of method sources() (use vector<Source>)
 * TODO: Write documentation.
 */
class EntityWithSources: public EntityWithMetadata {

protected:

  Block block;

public:

  /**
   * Standard constructor.
   */
  EntityWithSources(File file, Block block, Group group, std::string id);

  /**
   * Standard constructor that preserves the creation time.
   */
  EntityWithSources(File file, Block block, Group group, std::string id, time_t time);

  /**
   * Get the number of sources associated with this entity.
   *
   * @return The number sources.
   */
  size_t sourceCount() const;

  /**
   * Checks if a specific source is associated with this entity.
   *
   * @param source      The source to check.
   *
   * @return True if the source is associated with this entity, false otherwise.
   */
  bool hasSource(Source source) const;

  /**
   * Add a specific source to the list of associated sources.
   *
   * @param source      The source to associate with this entity.
   */
  void addSource(const Source &source);

  /**
   * Remove a source from the list of associated sources.
   * This source will not be deleted from the file.
   *
   * @param source      The source to remove.
   *
   * @return True if the source was removed, false otherwise.
   */
  bool removeSource(const Source &source);

  /**
   * Set all sources associated with this entity. All previosly
   * associated sources, that are not in the vector will be removed.
   *
   * @param sources     A vector with all sources.
   */
  void sources(std::vector<Source>);

  /**
   * Get all associated sources of this entity.
   *
   * @return A vector with all sources.
   */
  std::vector<Source> sources() const;

  /**
   * Destructor.
   */
  virtual ~EntityWithSources();

};

} // of namespace

#endif /* PAN_ENTITY_WITH_SOURCES_H_INCLUDED */
