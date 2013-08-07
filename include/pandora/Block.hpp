#ifndef PAN_BOCK_H_INCLUDE
#define PAN_BOCK_H_INCLUDE

#include <string>
#include <H5Cpp.h>

#include <pandora/Group.hpp>
#include <pandora/Util.hpp>
#include <pandora/File.hpp>
#include <pandora/NamedEntity.hpp>
#include <pandora/Source.hpp>
#include <pandora/SourceIterator.hpp>
#include <pandora/SourceTreeIterator.hpp>
#include <pandora/DataArray.hpp>
#include <pandora/DataArrayIterator.hpp>

namespace pandora {

class Block : public NamedEntity{
private:

  Group source_group, data_group;
public:

  Block(const Block &block);

  Block(File *file, Group group, std::string id);

  Source createSource(std::string name, std::string type, std::string parent_id = "");

  Source findSource(std::string source_id, std::string type = "", uint depth = 0);

  bool hasSource(std::string source_id, std::string type = "", uint depth = 0);

  SourceIterator sources();

  bool removeSource(std::string source_id);

  /**
   * Returns the number of Sources stored in the File.
   *
   * @return size_t   The number of sources.
   */
  size_t sourceCount() const;

  DataArray createDataArray(std::string name, std::string type);

  bool hasDataArray(std::string dataArray_id);

  DataArrayIterator  dataArrays();

  size_t dataArrayCount() const;

  DataArray getDataArray(std::string data_array_id);

  void removeDataArray(std::string data_array_id);


  // Tag getTag(std::string tag_id) const;

  /// @todo Iterate by name
  //iterator<Tag> tags() const;

  //Tag createTag(std::string name, std::string type) const;

  //void deleteTag(std::string tag_id) const;

  //void deleteTag(Tag &tag) const;

  //iterator<Source> sources() const;

  virtual ~Block();

};

}

#endif // HDX_BOCK_H_INCLUDE
