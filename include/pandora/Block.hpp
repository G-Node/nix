#ifndef PAN_BOCK_H_INCLUDE
#define PAN_BOCK_H_INCLUDE

#include <string>
#include <H5Cpp.h>

#include <pandora/Group.hpp>
#include <pandora/File.hpp>
#include <pandora/NamedEntity.hpp>

namespace pandora {

class Block : public NamedEntity
{

public:

  Block(const Block &block);

  Block(File *file, Group group, std::string id);

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
