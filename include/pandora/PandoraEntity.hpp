// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file pandora/PandoraEntity.hpp
 * @brief Definition of the class PandoraEntity.
 */


#ifndef PAN_PANDORAENTITY_H_INCLUDED
#define PAN_PANDORAENTITY_H_INCLUDED

#include <string>
#include <iostream>

#include <pandora/Group.hpp>
#include <pandora/File.hpp>

namespace pandora {

/**
 * Base class for all pandora entities e.g. Block, Section etc.
 */
class PandoraEntity {

protected:

  /** The file hosting the entity. */
  mutable File  *file;
  /** The group that represents the entity. */
  Group         group;
  /** The id of the entity (link name of the group) */
  std::string   entity_id;

public:

  /**
   * Standard constructor
   */
  PandoraEntity(File *file, Group group, std::string id);

  /**
   * Getter for the id of the entity.
   *
   * @return The id of the entity.
   */
  std::string id() const;

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
   * Gets the time of the last update.
   *
   * @return Time of the last update.
   */
  time_t updatedAt() const;

  /**
   * Gets the creation time.
   *
   * @return The creation time of the entity.
   */
  time_t createdAt() const;

  /**
   * Checks if two entities are identical.
   *
   * @param other The entity to compare with.
   *
   * @return true if both entities are equal, false otherwise.
   */
  bool operator==(const PandoraEntity &other) const;

  /**
   * Checks if two entities are identical.
   *
   * @param other The entity to compare with.
   *
   * @return true if both entities are not equal, false otherwise.
   */
  bool operator!=(const PandoraEntity &other) const;

  /**
   * Compares two entities.
   * <br><br>
   * Results:
   * <pre>
   *   0   They compare equal (this doesn't mean that they are
   *       identical).
   *   <0  The lexicographical order of the compared entity
   *       is lower.
   *   >0  The lexicographical order of the compared entity
   *       is lower.
   * </pre>
   *
   * @param other The entity to compare with.
   *
   * @return The lexicographical order of the compared entities.
   */
  int compare(const PandoraEntity &other) const;

  friend std::ostream& operator<<(std::ostream &out, const PandoraEntity &ent);

  /**
   * Destructor.
   */
  virtual ~PandoraEntity();

protected:

  /**
   * Sets the time of the last update to the current time.
   */
  void setUpdatedAt();

  /**
   * Sets the creation time to the current time if the attribute
   * created_at is not set.
   */
  void setCreatedAt();

  /**
   * Sets the creation time to the provided value even if the
   * attribute created_at is set.
   *
   * @param time The creation time to set.
   */
  void forceCreatedAt(time_t time);

};


} /* namespace pandora */

#endif /* PAN_PANDORAENTITY_H_INCLUDED */
