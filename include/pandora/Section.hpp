#ifndef PAN_SECTION_H_INCLUDE
#define PAN_SECTION_H_INCLUDE

#include <string>
#include <H5Cpp.h>

#include <pandora/Group.hpp>
#include <pandora/File.hpp>

namespace pandora {
class Property;
class PropertyIterator;
class SectionIterator;

class Section
{

private:

  File *file;
  Group group, props;
  std::string section_id;

public:

  Section(const Section &section);

  Section(File *file, Group group, std::string id);

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

  size_t childCount();

  SectionIterator children();

  bool hasChildren();

  Section addSection(std::string name, std::string type);

  bool removeSection(std::string id, bool cascade = true);

  PropertyIterator properties() const;

  Property addProperty(std::string name);

  void removeProperty(std::string id);

  bool operator==(const Section &other) const;

  bool operator!=(const Section &other) const;

  virtual ~Section();

};

}

#endif // HDX_SECTION_H_INCLUDE
