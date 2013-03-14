#ifndef PAN_BOCK_H_INCLUDE
#define PAN_BOCK_H_INCLUDE

#include <string>
#include <H5Cpp.h>

#include <pandora/Group.hpp>
#include <pandora/File.hpp>

namespace pandora {

class Block
{

private:

  File file;
  Group group;
  std::string block_id;

public:

  Block(const Block &block);

  Block(File file, Group group, std::string id);

  std::string id() const;

  void type(std::string type);
  std::string type() const;

  void name(std::string name);
  std::string name() const;

  void definition(std::string definition);
  std::string definition() const;

  void date(time_t date);
  time_t date() const;

  // Tag getTag(std::string tag_id) const;

  /// @todo Iterate by name
  //iterator<Tag> tags() const;

  //Tag createTag(std::string name, std::string type) const;

  //void deleteTag(std::string tag_id) const;

  //void deleteTag(Tag &tag) const;

  //iterator<Source> sources() const;
  
  bool operator==(const Block &other) const;

  bool operator!=(const Block &other) const;

  virtual ~Block();

};
  
}

#endif // HDX_BOCK_H_INCLUDE
