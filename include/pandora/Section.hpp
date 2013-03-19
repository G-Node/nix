#ifndef PAN_SECTION_H_INCLUDE
#define PAN_SECTION_H_INCLUDE

#include <string>
#include <H5Cpp.h>

#include <pandora/Group.hpp>
#include <pandora/File.hpp>

namespace pandora {

class Section
{

private:

  File file;
  Group group;
  std::string section_id;

public:

  Section(const Section &section);

  Section(File file, Group group, std::string id);

  std::string id() const;

  void type(std::string type);
  std::string type() const;

  void name(std::string name);
  std::string name() const;

  void definition(std::string definition);
  std::string definition() const;

  void repository(std::string repository);
  std::string repository() const;

  void link(std::string link);
  std::string link() const;

  void include(std::string include);
  std::string include() const;

  void mapping(std::string mapping);
  std::string mapping() const;

  void parent(std::string parent);
  std::string parent() const;

  bool operator==(const Section &other) const;

  bool operator!=(const Section &other) const;

  virtual ~Section();

};

}

#endif // HDX_SECTION_H_INCLUDE
