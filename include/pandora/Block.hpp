#ifndef PAN_BOCK_H_INCLUDE
#define PAN_BOCK_H_INCLUDE

#include <vector>
#include <string>
#include <H5Cpp.h>

#include <pandora/Group.hpp>
#include <pandora/Util.hpp>
#include <pandora/File.hpp>
#include <pandora/NamedEntity.hpp>

namespace pandora {

class Source;
class DataArray;

class Block : public NamedEntity {

private:

  const File file;
  Group source_group, data_group;

public:

  Block(const Block &block);

  Block(File file, Group group, std::string id);


  bool hasSource(std::string source_id, std::string type = "", uint depth = 0) const;

  Source getSource(std::string source_id, std::string type = "", uint depth = 0) const;

  size_t sourceCount() const;

  std::vector<Source> sources() const;

  Source createSource(std::string name, std::string type, std::string parent_id = "");

  bool removeSource(std::string source_id);



  bool hasDataArray(std::string dataArray_id) const;

  DataArray getDataArray(std::string data_array_id);

  std::vector<DataArray> dataArrays() const;

  size_t dataArrayCount() const;

  DataArray createDataArray(std::string name, std::string type);

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
