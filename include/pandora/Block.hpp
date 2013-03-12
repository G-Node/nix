#ifndef PAN_BOCK_H_INCLUDE
#define PAN_BOCK_H_INCLUDE

#include <string>
#include <H5Cpp.h>

#include <pandora/BaseContainer.hpp>
#include <pandora/File.hpp>

namespace pandora {

class Block : BaseContainer
{
  public:

    Block(const Block &block);
    Block(File &file, std::string id, H5::Group h5group);

    virtual ~Block();

    void type(std::string type);
    std::string type() const;
  
    void name(std::string name);
    std::string name() const;

    void definition(std::string definition);
    std::string definition() const;

    std::string blockId() const;
    // Tag getTag(std::string tag_id) const;

    /// @todo Iterate by name
    //iterator<Tag> tags() const;

    //Tag createTag(std::string name, std::string type) const;

    //void deleteTag(std::string tag_id) const;

    //void deleteTag(Tag &tag) const;

    //iterator<Source> sources() const;
  
private:  
  std::string _name;
  std::string block_id;
  File &file;
};
  
}

#endif // HDX_BOCK_H_INCLUDE
