// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file pandora/Block.hpp
 * @brief Definition of the class Block.
 */

#ifndef PAN_BOCK_H_INCLUDE
#define PAN_BOCK_H_INCLUDE

#include <vector>
#include <string>

#include <pandora/EntityWithMetadata.hpp>

namespace pandora {

class Section;
class Source;
class DataArray;
class SimpleTag;
class DataTag;

/**
 * Class that represents a pandora Block entity.
 */
class Block : public EntityWithMetadata {

private:

  Group source_group, data_array_group, simple_tag_group, data_tag_group;

public:

  /**
   * Copy constructor.
   *
   * @param block     The block to create the new copy from.
   */
  Block(const Block &block);

  /**
   * Standard constructor for a Block.
   *
   * @param file      The file which contains this block.
   * @param group     The group that represents the block inside the file.
   * @param id        The id of this block.
   */
  Block(File file, Group group, std::string id);

  /**
   * Standard constructor for a Block.
   *
   * @param file      The file which contains this block.
   * @param group     The group that represents the block inside the file.
   * @param id        The id of this block.
   */
  Block(File file, Group group, std::string id, time_t time);

  //--------------------------------------------------
  // Methods concerning sources
  // TODO implement source methods
  //--------------------------------------------------

  /**
   * Checks if this block has a specific root source.
   *
   * @param id        The id of the source.
   *
   * @return True if a source with the given id exists at the root, false
   *         otherwise.
   */
  bool hasSource(std::string id) const;

  /**
   * Retrieves a specific root source.
   *
   * @param id        The id of the source.
   *
   * @return The source with the given id. If it doesn't exist an exception
   *         will be thrown.
   */
  Source getSource(std::string id) const;

  /**
   * Retrieves a specific root source by index.
   *
   * @param index     The index of the source.
   *
   * @return The source at the specified index.
   */
  Source getSource(size_t index) const;

  /**
   * Checks if a source with a specific id exists either as a root source or
   * somewhere in the subtree of all root sources.
   *
   * @param id        The id of the source.
   *
   * @return True if a source with the given id exists, false otherwise.
   */
  bool existsSource(std::string id) const;

  /**
   * Retrieves a specific source by searching all sources and their
   * child sources.
   *
   * @param id        The id of the source.
   *
   * @return The source with the given id. If it doesn't exist an exception
   *         will be thrown.
   */
  Source findSource(std::string id) const;

  /**
   * Returns the number of root sources in this block.
   *
   * @return The number of root sources.
   */
  size_t sourceCount() const;

  /**
   * Returns all root sources in this block as a vector.
   *
   * @return All root sources.
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
  Source createSource(std::string name, std::string type);

  /**
   * Remove a root source and all its child sources from
   * the block.
   *
   * @param id        The id of the source to remove.
   *
   * @return True if the source was removed, false otherwise.
   */
  bool removeSource(std::string id);

  //--------------------------------------------------
  // Methods concerning data arrays
  // TODO implement data array methods
  //--------------------------------------------------

  /**
   * Checks if a specific data array exists in this block.
   *
   * @param id        The id of a data array.
   *
   * @return True if the data array exists, false otherwise.
   */
  bool hasDataArray(std::string id) const;

  /**
   * Retrieves a specific data array from the block.
   *
   * @param id        The id of an existing data array.
   *
   * @return The data array with the specified id. If this
   *         doesn't exist, an exception will be thrown.
   */
  DataArray getDataArray(std::string id) const;

  /**
   * Retrieves a data array by index.
   *
   * @param index     The index of the data array.
   *
   * @return The data array at the specified index.
   */
  DataArray getDataArray(size_t index) const;

  /**
   * Returns all data arrays of this block as a vector.
   *
   * @return All data arrays.
   */
  std::vector<DataArray> dataArrays() const;

  /**
   * Returns the number of all data arrays of the block.
   *
   * @return The number of data arrays of the block.
   */
  size_t dataArrayCount() const;

  /**
   * Create a new data array associated with this block.
   *
   * @param name      The name of the data array to create.
   *
   * @return The newly created data array.
   */
  DataArray createDataArray(std::string name, std::string type);

  /**
   * Remove/delete a data array from this block.
   *
   * @param id        The id of the data array to remove.
   *
   * @return True if the data array was removed, false otherwise.
   */
  bool removeDataArray(std::string id);

  //--------------------------------------------------
  // Methods concerning simple tags.
  // TODO implement simple tag methods.
  //--------------------------------------------------

  /**
   * Checks if a specific simple tag exists in the block.
   *
   * @param id        The id of a simple tag.
   *
   * @return True if the simple tag exists, false otherwise.
   */
  bool hasSimpleTag(std::string id) const;

  /**
   * Retrieves a specific simple tag from the block.
   *
   * @param id        The id of the simple tag.
   *
   * @return The tag with the specified id. If this tag doesn't exist
   *         an exception will be thrown.
   */
  SimpleTag getSimpleTag(std::string id) const;

  /**
   * Retrieves a specific simple tag by index.
   *
   * @param index     The index of the tag.
   *
   * @return The simple tag at the specified index.
   */
  SimpleTag getSimpleTag(size_t index) const;

  /**
   * Get all simple tags associated with this block.
   *
   * @return All simple tags as a vector.
   */
  std::vector<SimpleTag> simpleTags() const;

  /**
   * Returns the number of simple tag associated with
   * this block.
   *
   * @return The number of simple tags.
   */
  size_t simpleTagCount() const;

  /**
   * Create a new simple tag associated with this block.
   *
   * @param name      The name of the simple tag to create.
   * @param type      The type of the tag.
   *
   * @return The newly created tag.
   */
  SimpleTag createSimpleTag(std::string name, std::string type);

  /**
   * Remove a simple tag from the block.
   *
   * @param id        The id of the tag to remove.
   *
   * @return True if the tag was removed, false otherwise.
   */
  bool removeSimpleTag(std::string id);

  //--------------------------------------------------
  // Methods concerning data tags.
  // TODO implement data tag methods.
  //--------------------------------------------------

  /**
   * Checks if a specific data tag exists in the block.
   *
   * @param id        The id of a data tag.
   *
   * @return True if the data tag exists, false otherwise.
   */
  bool hasDataTag(std::string id) const;

  /**
   * Retrieves a specific data tag from the block.
   *
   * @param id        The id of the data tag.
   *
   * @return The tag with the specified id. If this tag doesn't exist
   *         an exception will be thrown.
   */
  DataTag getDataTag(std::string id) const;

  /**
   * Retrieves a specific data tag by index.
   *
   * @param index     The index of the tag.
   *
   * @return The data tag at the specified index.
   */
  DataTag getDataTag(size_t index) const;

  /**
   * Get all simple data associated with this block.
   *
   * @return All simple tags as a vector.
   */
  std::vector<DataTag> dataTags() const;

  /**
   * Returns the number of data tag associated with
   * this block.
   *
   * @return The number of data tags.
   */
  size_t dataTagCount() const;

  /**
   * Create a new data tag associated with this block.
   *
   * @param name      The name of the data tag to create.
   * @param type      The type of the tag.
   *
   * @return The newly created tag.
   */
  DataTag createDataTag(std::string name, std::string type);

  /**
   * Remove a data tag from the block.
   *
   * @param id        The id of the tag to remove.
   *
   * @return True if the tag was removed, false otherwise.
   */
  void removeDataTag(std::string id);

  /**
   * Output operator
   */
  friend std::ostream& operator<<(std::ostream &out, const Block &ent);

  virtual ~Block();

};

}

#endif // HDX_BOCK_H_INCLUDE
