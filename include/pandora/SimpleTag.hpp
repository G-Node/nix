// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file pandora/SimpleTag.hpp
 * @brief Definition of the class SimpleTag.
 */

#include <pandora/PSize.hpp>
#include <pandora/EntityWithSources.hpp>

#ifndef PAN_SIMPLETAG_H_INCLUDED
#define PAN_SIMPLETAG_H_INCLUDED

namespace pandora {

class Section;
class DataArray;
class Representation;
class RepresentationType;

enum class LinkType;

/**
 * Class that represents a pandora tag.
 *
 * TODO implement methods for representation.
 * TODO implement methods for references.
 */
class SimpleTag : public EntityWithSources {

private:

  static const PSize MIN_CHUNK_SIZE;
  static const PSize MAX_SIZE_1D;

  Group representation_group;

public:

  /**
   * Copy constructor
   */
  SimpleTag(const SimpleTag &tag);

  /**
   * Standard constructor
   */
  SimpleTag(File file, Block block, Group group, std::string id);

  /**
   * Standard constructor that preserves the creation time.
   */
  SimpleTag(File file, Block block, Group group, std::string id, time_t time);

  /**
   * Getter for the units of the tag. The units are applied to all values for position
   * and extent in order to calculate the right position vectors in referenced data arrays.
   *
   * @return All units of the tag as a vector.
   */
  std::vector<std::string> units() const;

  /**
   * Setter for the units of a tag.
   *
   * @param units     All units as a vector.
   */
  void units(std::vector<std::string> &units);

  /**
   * Getter for the position of a tag. The position is a vector that
   * points into referenced DataArrays.
   *
   * @return The position vector.
   */
  std::vector<double> position() const;

  /**
   * Setter for the position of a tag.
   *
   * @param position    The position vector.
   */
  void position(const std::vector<double> &position);

  /**
   * Getter for the extent of a tag. Given a specified position
   * vector, the extent vector defined the size of a region of
   * interest in the referenced DataArrays.
   *
   * @return The extent of the tag.
   */
  std::vector<double> extent() const;

  /**
   * Setter for the extent of a tag.
   *
   * @param extent      The extent vector.
   */
  void extent(const std::vector<double> &extent);

  //--------------------------------------------------
  // Methods concerning references.
  // TODO implement when done with the DataArray class.
  // TODO add hasXy getXy addXy and removeXy methods for references.
  //--------------------------------------------------

  bool hasReference(const DataArray &reference) const;

  bool hasReference(const std::string &id) const;

  size_t referenceCount() const;

  DataArray getReference(const std::string &id) const;

  void addReference(const DataArray &reference);

  bool removeReference(const DataArray &reference);

  /**
   * Getter for all referenced DataArrays.
   *
   * @return All referenced DataArrays
   */
  std::vector<DataArray> references() const;

  /**
   * Setter for all referenced DataArrays. Previously referenced
   * DataArrays, that are not in the references vector will be
   * removed.
   *
   * @param references    All referenced arrays.
   */
  void references(const std::vector<DataArray> &references);

  //--------------------------------------------------
  // Methods concerning representations.
  //--------------------------------------------------

  /**
   * Checks if a specific representation exists on the tag.
   *
   * @param id        The id of a representation.
   *
   * @return True if the representation exists, false otherwise.
   */
  bool hasRepresentation(std::string id) const;

  /**
   * Returns the number of representations in this block.
   *
   * @return The number of representations.
   */
  size_t representationCount() const;

  /**
   * Retrieves a specific representation from the tag.
   *
   * @param id        The id of the representation.
   *
   * @return The representation with the specified id. If it doesn't exist
   *         an exception will be thrown.
   */
  Representation getRepresentation(std::string id) const;

  /**
   * Retrieves a specific representation from the tag.
   *
   * @param index        The index of the representation.
   *
   * @return The representation with the specified index.
   */
  Representation getRepresentation(size_t index) const;

  /**
   * Getter for all representations of the tag.
   *
   * @return All representations as vector.
   */
  std::vector<Representation> representations() const;

  /**
   * Create a new representation.
   *
   * @param data      The data array of this representation.
   * @param type      The link type of this representation.
   *
   * @return The created representation object.
   */
  Representation createRepresentation(DataArray data, RepresentationType link_type);

  /**
   * Remove a representation from the tag.
   *
   * @param id        The id of the representation to remove.
   *
   * @return True if the representation was removed, false otherwise.
   */
  bool removeRepresentation(std::string id);


  //--------------------------------------------------
  // Other methods and functions
  //--------------------------------------------------

  /**
   * Assignment operator
   */
  SimpleTag& operator=(const SimpleTag &other);

  /**
   * Output operator
   */
  friend std::ostream& operator<<(std::ostream &out, const SimpleTag &ent);

  /**
   * Destructor.
   */
  virtual ~SimpleTag();
};


} /* namespace pandora */

#endif /* PAN_SIMPLETAG_H_INCLUDED */
