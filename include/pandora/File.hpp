#ifndef PAN_FILE_H_INCLUDED
#define PAN_FILE_H_INCLUDED

#include <iterator>
#include <string>

#include <H5Cpp.h>

#include <pandora/Group.hpp>

namespace pandora {

class Block;

class File
{

private:

  std::string prefix;
  H5::H5File  h5file;
  Group       root, metadata, data;

public:

  static const std::string FORMAT;
  static const std::string VERSION;

  static const size_t READ_ONLY;
  static const size_t READ_WRITE;
  static const size_t OVERWRITE;

  File(std::string name, std::string prefix, int mode = READ_WRITE);

  File(const File &other);

  bool fileExists(std::string name) const;

  size_t blockCount() const;

  bool hasBlock(std::string id) const;

  Block getBlock(std::string id) const;

  Block getBlock(size_t index) const;

  Block createBlock(std::string name, std::string type);

  void deleteBlock(std::string block_id);

  // std::string blockId(size_t index) const;

  // std::string blockName(size_t index);

  /// @todo Iterate by name
  //std::iterator<Block> blocks() const;


  // Section getSection(std::string section_id) const;

  // iterator<Section> sections() const;

  // Section createSection(std::string name, std::string type) const;

  // Section deleteSection(std::string section_id) const;

  // Section deleteSection(Section &section) const;

  std::string createId() const;

  std::string version() const;

  std::string format() const;

  time_t createdAt() const;

  time_t updatedAt() const;

  H5::H5File getH5File() const;

  void close();

  File& operator=(const File &other);

  bool operator==(const File &other) const;

  bool operator!=(const File &other) const;

  virtual ~File();

private:

  bool checkHeader();

};

}

#endif // PAN_FILE_H_INCLUDED
