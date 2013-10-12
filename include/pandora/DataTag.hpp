// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
// Author: Jan Grewe <jan.grewe@g-node.org>

/**
 * @file pandora/DataTag.hpp
 * @brief Definition of the class DataTag.
 */

#include <pandora/EntityWithSources.hpp>
#include <pandora/util/ReferenceList.hpp>
#include <pandora/DataArray.hpp>

#ifndef PAN_DATATAG_H_INCLUDED
#define PAN_DATATAG_H_INCLUDED

namespace pandora {

class Section;
//class DataArray;
class Representation;
class RepresentationType;

enum class LinkType;

/**
 * Class that represents a pandora datatag.
 *
 * TODO implement methods for representation.
 */
class DataTag : public EntityWithSources {

private:

  Group representation_group;
  util::ReferenceList reference_list;

public:

  /**
   * Copy constructor
   */
  DataTag(const DataTag &tag);

  /**
   * Standard constructor
   */
  DataTag(File file, Block block, Group group, std::string id);

  /**
   * Standard constructor that preserves the creation time.
   */
  DataTag(File file, Block block, Group group, std::string id, time_t time);

  /**
   * Getter for the positions of a tag. The positions are strored in a DataArray.
   *
   *
   * @return The positions DataArray.
   */
  DataArray positions() const;

  /**
   * Setter for the positions of a tag.
   *
   * @param position   string.
   */
  void positions(const std::string &positionid);

  /**
   * Setter for the positions of a tag.
   *
   * @param position   DataArray.
   */
  void positions(const DataArray &positions);

  /**
   * Returns whether this DataArray contains positions.
   *
   * @return bool
   */
  bool hasPositions() const;

  /**
   * Getter for the extents of the tag which are stored in a#
   * DataArray
   *
   * @return The extents of the tag.
   */
  DataArray extents() const;

  /**
   * Sets the extents DataArray of the tag.
   *
   * @param extent      The extent vector.
   */
  void extents(const DataArray &extents);

  /**
   * Set the reference to the extents DataArray.
   *
   * @param the id.
   */
  void extents(const std::string &extentsId);

  /**
   * Returns whether this DataArray contains extents.
   *
   * @return bool
   */
  bool hasExtents() const;
  //--------------------------------------------------
  // Methods concerning references.
  // TODO implement when done with the DataArray class.
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

  /**
   * Get the data that belongs to the Reference and is related to position and extent referenced
   * with the index given.
   *
   * @param boost::multi_array<T, dims> &data return argument for the data
   * @param size_t index
   *
   */
  template<typename T, size_t dims>
  void getReferencedData(std::vector<boost::multi_array<T, dims>> &data, size_t index) const{
    if (referenceCount() == 0){
      throw std::runtime_error("DataTag::getReferencedData: There is no reference attached to this tag!");
    }
    if(!hasPositions()){
      throw std::runtime_error("DataTag::getReferencedData: There is no positions array attached to this tag!");
    }

    DataArray pa = positions();
    boost::multi_array<double,1> posData, extData;
    pa.getRawData(posData);

    if(index >= posData.shape()[0]){
      throw std::runtime_error("DataTag::getReferencedData: index exeeds matrix dimensions in positions data!");
    }

    if(hasExtents()){
      DataArray ea = extents();
      ea.getRawData(extData);
    }

    //TODO convert position and extent to respective units
    //TODO get the data slice from the referenced DataArrays
    std::vector<DataArray> refs = references();
    for (size_t i = 0; i < refs.size();i++){

    }
  }


  //--------------------------------------------------
  // Other methods and functions
  //--------------------------------------------------

  /**
   * Assignment operator
   */
  DataTag& operator=(const DataTag &other);

  /**
   * Output operator
   */
  friend std::ostream& operator<<(std::ostream &out, const DataTag &ent);

  /**
   * Destructor.
   */
  virtual ~DataTag();

private:
  //TODO validate method that checks dimensionality, units, etc...
  bool checkDimensions(const DataArray &a, const DataArray &b) const;

  bool checkPositionsAndExtents() const;
};


} /* namespace pandora */

#endif /* PAN_DATATAG_H_INCLUDED */
