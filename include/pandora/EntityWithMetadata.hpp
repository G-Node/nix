// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file pandora/EntityWithMetadata.hpp
 * @brief Definition of the class EntityWithMetadata.
 */

#ifndef PAN_ENTITY_WITH_METADATA_H_INCLUDED
#define PAN_ENTITY_WITH_METADATA_H_INCLUDED

#include <string>
#include <iostream>

#include <pandora/PandoraEntity.hpp>
#include <pandora/Group.hpp>
#include <pandora/File.hpp>

namespace pandora {

/**
 * Base class for entities that are associated with metadata such
 * as Block, Source etc.
 *
 * TODO Implement all methods of EntityWithMetadata.
 */
class EntityWithMetadata : public PandoraEntity {

public:

  /**
   * Checks if the block has associated metadata.
   *
   * @return True if the block has metadata (odML section),
   *         false otherwise.
   */
  bool hasMetadata() const;

  /**
   * Get metadata associated with this entity.
   *
   * @return The associated section, if no such section exists
   *         an exception will be thrown.
   */
  Section metadata() const;

  /**
   * Associate the entity with some metadata.
   *
   * @param metadata    The section that should be associated
   *                    with this entity.
   */
  void metadata(const Section &metadata);

  /**
   * Destructor of this class.
   */
  virtual ~EntityWithMetadata() {};

};


} /* namespace pandora */

#endif /* PAN_ENTITY_WITH_METADATA_H_INCLUDED */
