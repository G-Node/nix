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

#include <pandora/NamedEntity.hpp>
#include <pandora/Group.hpp>
#include <pandora/File.hpp>
#include <pandora/DataSet.hpp>
#include <pandora/Charon.hpp>
#include <pandora/Source.hpp>

namespace pandora {

/**
 * Base class for entities that are associated with sources.
 *
 * TODO: Refactoring of method sources() (use vector<Source>)
 * TODO: Write documentation.
 */
class EntityWithSources: public PandoraEntity {

public:

  size_t sourceCount() const;

  bool hasSource(std::string source_id) const;

  void addSource(const Source &source);

  void addSource(std::string source_id);

  void removeSource(std::string source_id);

  void sources(std::vector<std::string>);

  std::vector<std::string> sources() const;

  virtual ~EntityWithSources() {};

};

} // of namespace

#endif /* PAN_ENTITY_WITH_SOURCES_H_INCLUDED */
