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
class TreeIterator;

class Section {

private:
  mutable File *file;
  Group group, props, sections;
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
/* TODO: how to support includes?!
  void include(std::string include);
  std::string include() const;
*/
  void mapping(std::string mapping);
  std::string mapping() const;

  void parent(std::string parent);
  std::string parent() const;

  size_t childCount() const;

  SectionIterator children() const;

  bool hasChildren() const;

  TreeIterator treeIterator(uint depth = 0) const;

  Section addSection(std::string name, std::string type);
  /**
   * Performs a search on the tree starting at this section and returns whether a section with
   * the specified id exists.
   *
   * @param id: string the id of requested section
   * @param depth: uint (default 0). The depth of the search. 0 indicates unlimited depth.
   *
   * @return bool
   */
  bool hasSection(std::string id, uint depth = 0) const;

  Section findSection(std::string id, uint depth = 0) const;

  bool removeSection(std::string id, bool cascade = true);

  PropertyIterator properties() const;

  PropertyIterator inheritedProperties() const;

  Property getProperty(std::string id) const;

  Property getPropertyByName(std::string name) const;

  Property addProperty(std::string name);

  void removeProperty(std::string id);

  size_t propertyCount() const;

  bool operator==(const Section &other) const;

  bool operator!=(const Section &other) const;

  virtual ~Section();

  bool hasProperty(std::string id) const;

  bool hasPropertyByName(std::string name) const;
};

}

#endif // HDX_SECTION_H_INCLUDE
