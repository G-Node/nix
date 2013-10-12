// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file pandora/Source.hpp
 * @brief Definition of the class Source.
 */

#ifndef PAN_SOURCE_H_INCLUDE
#define PAN_SOURCE_H_INCLUDE


#include <string>
#include <functional>
#include <pandora/EntityWithMetadata.hpp>
#include <pandora/TNode.hpp>

namespace pandora {

/**
 * An entity that represents a source as defined in the pandora
 * data model.
 */
class Source : public EntityWithMetadata, public TNode<Source>  {

private:

  Group source_group;

public:

  //typedefs
  using size_type = TNode<Source>::size_type;
  
  
  /**
   * Copyconstructor.
   */
  Source(const Source &source);

  /**
   * Default constructor.
   */
  Source(File file, Group group, const std::string &id);

  /**
   * Default constructor that preserves the creation time.
   */
  Source(File file, Group group, const std::string &id, time_t time);

  
  //TNode interface
  virtual size_type childCount() const;
  virtual Source    getChild(size_type index) const;


  //--------------------------------------------------
  // Methods concerning child sources
  // TODO implement source methods
  //--------------------------------------------------

  /**
   * Checks if this source has a specific source as direct descendant.
   *
   * @param id        The id of the source.
   *
   * @return True if a source with the given id is a direct descendant, false
   *         otherwise.
   */
  bool hasSource(const std::string &id) const;

  /**
   * Retrieves a specific child source that is a direct descendant.
   *
   * @param id        The id of the source.
   *
   * @return The source with the given id. If it doesn't exist an exception
   *         will be thrown.
   */
  Source getSource(const std::string &id) const;

  /**
   * Retrieves a specific source by index.
   *
   * @param index     The index of the source.
   *
   * @return The source at the specified index.
   */
  Source getSource(size_t index) const;
  
  /**
   * Returns the number of sources that are direct descendants of this source.
   *
   * @return The number of direct child sources.
   */
  size_t sourceCount() const;

  /**
   * Returns all sources that are direct descendant of this source as a vector.
   *
   * @return All direct child sources.
   */
  std::vector<Source> sources() const;

  /**
   * Create a new root source.
   *
   * @param name      The name of the source to create.
   * @param type      The type of the source.
   *
   * @return The created source object.
   */
  Source createSource(const std::string &name, const std::string &type);

  /**
   * Remove a root source and all its child sources from
   * the source.
   *
   * @param id        The id of the source to remove.
   *
   * @return True if the source was removed, false otherwise.
   */
  bool removeSource(std::string id);

  //--------------------------------------------------
  // Other methods and functions
  //--------------------------------------------------

  /**
   * Assignment operator.
   */
  Source& operator=(const Source &other);

  /**
   * Output operator
   */
  friend std::ostream& operator<<(std::ostream &out, const Source &ent);


  virtual ~Source();
};
}//of namespace


#endif // PAN_SOURCE_H_INCLUDE
