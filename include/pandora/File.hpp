#ifndef PAN_FILE_H_INCLUDED
#define PAN_FILE_H_INCLUDED

#define FORMAT "pandora"
#define VERSION "1.0"

#include <iterator>
#include <string>
#include <vector>
#include <utility>

#include <H5Cpp.h>
#include <H5File.h>

#include <pandora/BaseContainer.hpp>

class Block;

namespace pandora {

class File:BaseContainer {

public:
  static const size_t READ_ONLY;
  static const size_t READ_WRITE;
  static const size_t OVERWRITE;

  File( std::string name, std::string prefix, int mode = 1 );

  File( const File &other );

  File& operator= (const File &other);
  
  Block getBlock(std::string block_id) const;

  /// @todo Iterate by name
  //std::iterator<Block> blocks() const;

  Block createBlock(std::string name, std::string type);

  void deleteBlock( std::string block_id ) const;

  void deleteBlock( Block &block ) const;

  // Section getSection(std::string section_id) const;

  // iterator<Section> sections() const;

  // Section createSection(std::string name, std::string type) const;

  // Section deleteSection(std::string section_id) const;

  // Section deleteSection(Section &section) const;

  std::string createId() const;

  std::string time_stamp() const;

  void version( std::string version );
  std::string version() const;

  void format( std::string format );
  std::string format() const;

  std::string created_at() const;

  std::string updated_at() const;

  H5::H5File getH5File();

  void close();

  virtual ~File();

private:

  std::string prefix;

  H5::H5File h5file;

  void checkAttributes( );

  void checkGroups( );

  bool fileExists( std::string name ) const;

  void openHDFFile( std::string name, int mode );

  bool checkFormatAndVersion() const;

};

}

#endif // PAN_FILE_H_INCLUDED
