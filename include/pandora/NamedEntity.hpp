// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file pandora/NamedEntity.hpp
 * @brief Definition of the class NamedEntity.
 */

#ifndef PAN_NAMEDENTITY_H_INCLUDED
#define PAN_NAMEDENTITY_H_INCLUDED

#include <string>
#include <iostream>

#include <pandora/PandoraEntity.hpp>
#include <pandora/Group.hpp>
#include <pandora/File.hpp>

namespace pandora {

/**
 * Base class for all pandora entities e.g. Block, Section etc.
 */
class NamedEntity : public PandoraEntity {

protected:


public:

  /**
   * Standard constructor
   */
  NamedEntity(File *file, Group group, std::string id);

  /**
   * Standard constructor that preserves the creation time.
   */
  NamedEntity(File *file, Group group, std::string id, time_t time);

  /**
   * Setter for the type of the entity.
   *
   * @param type The type to set.
   */
  void type(std::string type);

  /**
   * Getter for the type of the entity.
   *
   * @return The current type.
   */
  std::string type() const;

  /**
   * Setter for the name of the entity.
   *
   * @param name The name of the entity.
   */
  void name(std::string name);

  /**
   * Getter for the name of the entity.
   *
   * @return The name of the entity.
   */
  std::string name() const;

  /**
   * Setter for the definition of the entity.
   *
   * @param definition The definition of the entity.
   */
  void definition(std::string definition);

  /**
   * Getter for the definition of the entity.
   *
   * @return The definition of the entity.
   */
  std::string definition() const;

  /**
   * Setter for the metadata section linked to the entity.
   *
   * @param section_id the id of the lined metadata section.
   */
  void metadata(std::string section_id);

  /**
   * Getter for the metadata linked to this entity.
   *
   * @return The section_id of the linked metadata section.
   */
  std::string metadata() const;

  int compare(const NamedEntity &other) const;

  /**
   * Destructor.
   */
  virtual ~NamedEntity();

};


} /* namespace pandora */

#endif /* PAN_NAMEDENTITY_H_INCLUDED */
