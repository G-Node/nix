#ifndef PAN_FILE_H_INCLUDED
#define PAN_FILE_H_INCLUDED

#define FORMAT "pandora"
#define VERSION "1.0"

#include <iterator>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <time.h>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <H5Cpp.h>
#include <H5File.h>

#include <pandora/BaseContainer.hpp>

namespace pandora {

class Block;

class File:BaseContainer {

public:
  static const size_t READ_ONLY;
  static const size_t READ_WRITE;
  static const size_t OVERWRITE;

  File( std::string name, std::string prefix, int mode = 1 );

  File( const File &other );

  File& operator=( const File &other );


  /// @todo Iterate by name
  const std::iterator<std::input_iterator_tag, Block> blocks() const;

  bool hasBlock( std::string name ) const;

  size_t blockCount() const;

  Block *getBlock( std::string block_id );

  std::string blockName(int i) const;

  /// @todo Iterate by name
  //std::iterator<Block> blocks() const;

  Block *createBlock( std::string name, std::string type );

  void deleteBlock( std::string block_id );
  /*
   void deleteBlock( Block &block );
   */
  // Section getSection(std::string section_id) const;

  // iterator<Section> sections() const;

  // Section createSection(std::string name, std::string type) const;

  // Section deleteSection(std::string section_id) const;

  // Section deleteSection(Section &section) const;

  std::string createId() const;

  std::string time_stamp() const;

  std::string version() const;

  std::string format() const;

  std::string created_at() const;

  std::string updated_at() const;

  H5::H5File getH5File() const;

  void close();

  virtual ~File();

private:

  std::string prefix;

  H5::H5File h5file;

  void checkAttributes();

  void checkGroups();

  bool fileExists( std::string name ) const;

  void openHDFFile( std::string name, int mode );

  bool checkFormatAndVersion() const;

  void version( std::string version );

  void format( std::string format );

};

}

#endif // PAN_FILE_H_INCLUDED
