// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file pandora/File.hpp
 * @brief Definition of the class file.
 */

#ifndef PAN_FILE_H_INCLUDED
#define PAN_FILE_H_INCLUDED

#include <iterator>
#include <string>
#include <H5Cpp.h>

#include <pandora/Group.hpp>

namespace pandora {

class Block;
class Section;

enum class FileMode {
  ReadOnly = 0,
      ReadWrite,
      Overwrite
};

/**
 * Class that represents a pandora file.
 */
class File {

private:

  /* the opened HDF5 file */
  H5::H5File h5file;

  /* groups representing different sections of the file */
  Group root, metadata, data;

public:

  /** Indicator for the HDF5 file being a pandora file */
  static const std::string FORMAT;
  /** The version of the pandora format */
  static const std::string VERSION;

  /**
   * Constructor that is used to open the file.
   *
   * @param name    The name of the file to open.
   * @param prefix  The prefix used for IDs.
   * @param mode    File open mode ReadOnly, ReadWrite or Overwrite.
   */
  File(std::string name, const FileMode mode = FileMode::ReadWrite);

  /**
   * Copy constructor.
   *
   * @param other   The file to copy.
   */
  File(const File &other);

  /**
   * Get the number of blocks in in the file.
   *
   * @return The number of blocks.
   */
  size_t blockCount() const;

  /**
   * Check if a block exists in the file.
   *
   * @param id    The ID of the block.
   *
   * @return True if the block exists, false otherwise.
   */
  bool hasBlock(std::string id) const;

  /**
   * Read an existing block from the file.
   *
   * @param id    The ID of the block.
   *
   * @return The block with the given id.
   */
  Block getBlock(std::string id) const;

  /**
   * Read an existing with block from the file, addressed by index.
   *
   * @param index   The index of the block to read.
   *
   * @return The block at the given index.
   */
  Block getBlock(size_t index) const;

  /**
   * Create an new block, that is immediately persisted in the file.
   *
   * @param name    The name of the block.
   * @param type    The type of the block.
   *
   * @return The created block.
   */
  Block createBlock(std::string name, std::string type);

  /**
   * Delete a block from the file.
   *
   * @param id    The id of the block to delete.
   */
  void deleteBlock(std::string id);

  /**
   * Returns all blocks in this file.
   *
   * @return All blocks in this file as a vector.
   */
  std::vector<Block> blocks() const;

  /**
   * Check if a section exists in the file.
   *
   * @param id    The ID of the section.
   * @param type The type of Section. Default is ""
   * @param depth The depth of the search. default: 0 stands for unlimited search.
   *
   * @return True if the section exists, false otherwise.
   */
  bool hasSection(std::string id, std::string type = "", uint depth = 0) const;

  /**
   * Get root section with a given id.
   *
   * @param id      The id of the section.
   *
   * @return The section with the specified id.
   */
  Section getSection(std::string id) const;

  /**
   * Get root section with a given index/position.
   *
   * @param index      The index of the section.
   *
   * @return The section with the specified index.
   */
  Section getSection(size_t index) const;

  /**
   * Returns the number of Sections stored in the File.
   *
   * @return size_t   The number of sections.
   */
  size_t sectionCount() const;

  /**
   * Returns all root sections in this file.
   *
   * @returns All root sections as vector.
   */
  std::vector<Section> sections() const;

  /**
   * Return the Section specified by the id.
   *
   * @param id  The id of the Section
   * @param type The type of Section. Default is ""
   * @param depth The depth of the search. 0 stands for unlimited search, 1 for direct children only.
   *
   * @return The section with the given id.
   */
  Section findSection(std::string id, std::string type = "", uint depth = 0) const;

  /**
   * Creates a new Section with a given name and type. Both must not be empty.
   *
   * @param std::string the given name of the section.
   * @param std::string the type of the section.
   *
   * @return   the created Section.
   */
  Section createSection(std::string name, std::string type, std::string parent = "");

  /**
   * Deletes the Section that is specified with the id.
   */
  bool removeSection(std::string id);

  /**
   * Read the pandora version from the file.
   *
   * @return The verion of the pandora file.
   */
  std::string version() const;

  /**
   * Read the format hint from the file.
   *
   * @return The format hint.
   */
  std::string format() const;

  /**
   * Get the creation date of the file.
   *
   * @return The creation date of the file.
   */
  time_t createdAt() const;

  /**
   * Get the date of the last update.
   *
   * @return The date of the last update.
   */
  time_t updatedAt() const;


  /**
   * Comparator.
   */
  bool operator==(const File &other) const;

  /**
   * Comparator.
   */
  bool operator!=(const File &other) const;

  /**
   * Assignment operator.
   *
   * @param The file to assign.
   *
   * @return A reference to the assigned file.
   */
  File& operator=(const File &other);

  /**
   * Destructor.
   */
  virtual ~File();

private:

  // check for existence
  bool fileExists(std::string name) const;

  // check if the header of the file is valid
  bool checkHeader() const;

};

}

#endif // PAN_FILE_H_INCLUDED
