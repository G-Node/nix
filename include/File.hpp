#ifndef HDX_FILE_H_INCLUDED
#define PAN_FILE_H_INCLUDED

#include <iterator>
#include <string>

#include <H5Cpp.h>
#include <H5File.h>
#include "Block.hpp"

namespace pandora {

class File {

public:
  File(std::string name, std::string prefix, std::string mode = "w");

  File(const File &other);

  Block getBlock(std::string block_id) const;

  /// @todo Iterate by name
  //std::iterator<Block> blocks() const;

  Block createBlock(std::string name, std::string type) const;

  void deleteBlock(std::string block_id) const;

  void deleteBlock(Block &block) const;

  // Section getSection(std::string section_id) const;

  // iterator<Section> sections() const;

  // Section createSection(std::string name, std::string type) const;

  // Section deleteSection(std::string section_id) const;

  // Section deleteSection(Section &section) const;

  std::string createId() const;

  H5::H5File getH5File() const;

  virtual ~File();

private:

  std::string prefix;

  H5::H5File h5file;

};

}

#endif // PAN_FILE_H_INCLUDED
