#ifndef PAN_SECTION_H_INCLUDE
#define PAN_SECTION_H_INCLUDE

#include <string>
#include <H5Cpp.h>
#include <functional>
#include <pandora/NamedEntity.hpp>
#include <pandora/Group.hpp>
#include <pandora/File.hpp>

namespace pandora {
class Property;
class PropertyIterator;
class SectionIterator;
class SectionTreeIterator;

class Section : public NamedEntity{

private:
  Group property_group, section_group;

protected:
  File file;

public:

  /**
   * Copy constructor
   */
  Section(const Section &section);

    /**
     * Standard constructor
     */
  Section(File file, Group group, const std::string &id);

    /**
     * Standard constructor that preserves the creation time.
     */
  Section(File file, Group group, const std::string &id, time_t time);

  void repository(const std::string &repository);
  std::string repository() const;

  void link(const std::string &link);
  std::string link() const;
/* TODO: how to support includes?!
  void include(std::string include);
  std::string include() const;
*/
  void mapping(const std::string &mapping);
  std::string mapping() const;

  void parent(const std::string &parent);
  std::string parent() const;

  size_t sectionCount() const;

  Section getSection(size_t index) const;

  SectionIterator children(const std::string &type = "") const;

  std::vector<Section> sections() const;

  bool hasChildren() const;

  SectionTreeIterator treeIterator(const std::string &type = "", uint depth = 0) const;

  Section addSection(const std::string &name, const std::string &type);
  /**
   * Performs a search on the tree starting at this section and returns whether a section with
   * the specified id exists.
   *
   * @param id: string the id of requested section
   * @param depth: uint (default 0). The depth of the search. 0 indicates unlimited depth.
   *
   * @return bool
   */
  bool hasSection(const std::string &id) const;

  /**
   * Return a vector of sections for which the predicate evaluates to true.
   *
   * @param predicate     Predicate function that will be called for each section. Return true to collect.
   * @param exclude_root  Whether or not to include the root source.
   * @param max_depth     The maximum recursion depth.
   *
   * @return The source for which predicate was true.
   */
  std::vector<Section> findSections(std::function<bool(const Section &)> predicate, bool exclude_root = false, int max_depth = -1) const;



  bool hasRelatedSection(const std::string &type) const;

  std::vector<std::string> getRelatedSections(const std::string &type) const;

  bool removeSection(const std::string &id);

  PropertyIterator properties() const;

  PropertyIterator inheritedProperties() const;

  Property getProperty(const std::string &id) const;

  Property getPropertyByName(const std::string &name) const;

  Property addProperty(const std::string &name);

  void removeProperty(const std::string &id);

  size_t propertyCount() const;

  bool operator==(const Section &other) const;

  bool operator!=(const Section &other) const;

  virtual ~Section();

  bool hasProperty(const std::string &id) const;

  bool hasPropertyByName(const std::string &name) const;

private:
  bool hasParent() const;

  Section findParent() const;

  std::string findUpstream(const std::string &type) const;

  std::vector<std::string> findSideways(const std::string &type) const;

  std::vector<std::string> findDownstream(const std::string &type) const;

  //void findSectionRec(const std::string &id, std::vector<Section> &sec) const;
  void findSectionsRec(const Section &cur_section,
                        std::vector<Section> &results,
                        std::function<bool(const Section &)> predicate,
                        int level,
                        int max_depth) const;

};

}

#endif // HDX_SECTION_H_INCLUDE
