#include <pandora/Section.hpp>
#include <pandora/SectionIterator.hpp>
#include <pandora/Util.hpp>
#include <pandora/Property.hpp>
#include <pandora/PropertyIterator.hpp>
#include <pandora/TreeIterator.hpp>

#include <iostream>

using namespace std;

namespace pandora {

Section::Section(const Section &section) :
    file(section.file), group(section.group), section_id(section.section_id) {
  props = section.props;
}

Section::Section(File *file, Group group, string id) :
    file(file), group(group), section_id(id) {
  props = group.openGroup("properties");
}

string Section::id() const {
  return section_id;
}

void Section::type(string type) {
  group.setAttr("type", type);
}

string Section::type() const {
  string type;
  group.getAttr("type", type);
  return type;
}

void Section::name(string name) {
  group.setAttr("name", name);
}

string Section::name() const {
  string name;
  group.getAttr("name", name);
  return name;
}

void Section::definition(string definition) {
  group.setAttr("definition", definition);
}

string Section::definition() const {
  string definition;
  group.getAttr("definition", definition);
  return definition;
}

void Section::repository(string repository) {
  group.setAttr("repository", repository);
}

string Section::repository() const {
  string repository;
  group.getAttr("repository", repository);
  return repository;
}

void Section::link(string link) {
  if (this->file->hasSection(link)) {
    if (this->file->getSection(link).type().compare(this->type()) == 0) {
      group.setAttr("link", link);
    } else {
      throw std::runtime_error(
          "Cannot create link to a section of deviating type!");
    }
  } else {
    throw std::runtime_error(

    "Cannot create link! Linked section does not exist!");
  }
}

string Section::link() const {
  string link;
  group.getAttr("link", link);
  return link;
}
void Section::include(string include) {
  group.setAttr("include", include);
}

string Section::include() const {
  string include;
  group.getAttr("include", include);
  return include;
}

void Section::mapping(string mapping) {
  group.setAttr("mapping", mapping);
}

string Section::mapping() const {
  string mapping;
  group.getAttr("mapping", mapping);
  return mapping;
}

void Section::parent(string parent) {
  group.setAttr("parent", parent);
}

string Section::parent() const {
  string parent;
  group.getAttr("parent", parent);
  return parent;
}

Section Section::addSection(std::string name, std::string type) {
  Section s = (*file).createSection(name, type, id());
  return s;
}

bool Section::removeSection(std::string id, bool cascade) {
  return (*file).removeSection(id, cascade);
}

bool Section::hasChildren() const {
  SectionIterator iter = this->children();
  return iter != iter.end();
}

SectionIterator Section::children() const {
  SectionIterator iter(file, (*file).metadataGroup(), id());
  return iter;
}

TreeIterator Section::treeIterator() const {
  TreeIterator iter(*this);
  return iter;
}

size_t Section::childCount() const {
  size_t childCount = 0;
  for (SectionIterator iter = this->children(); iter != iter.end(); ++iter) {
    childCount++;
  }
  return childCount;
}

PropertyIterator Section::properties() const {
  PropertyIterator iter(*this, props);
  return iter;
}

PropertyIterator Section::inheritedProperties() const {
  if(this->link().length() > 0){
    return this->file->getSection(this->link()).properties();
  }
  else{
    throw std::runtime_error(
           "Section has no link, cannot retrieve inherited Properties!");
  }
}

Property Section::getProperty(std::string id) const {
  if (props.hasGroup(id)) {
    return Property(*this, props.openGroup(id, false), id);
  } else {
    throw std::runtime_error(
        "Requested Property does not exist! Always check with hasProperty!");
  }
}

Property Section::getPropertyByName(std::string name) const {
  for (PropertyIterator iter = properties(); iter != iter.end(); ++iter) {
    Property p = *iter;
    if (p.name() == name)
      return p;
  }
  if (this->link().length() > 0) {
    if (this->file->hasSection(this->link())) {
      Section linked = this->file->getSection(this->link());
      if (linked.hasPropertyByName(name)) {
        return linked.getPropertyByName(name);
      }
    }
  }

  throw std::runtime_error(
      "Requested Property does not exist! Always check with hasPropertyByName!");
}

Property Section::addProperty(std::string name) {
  if (hasPropertyByName(name)) {
    throw std::runtime_error("Attempt to add a property that already exists!");
  }
  string new_id = util::createId("property");
  while (props.hasObject(new_id))
    new_id = util::createId("property");
  Property p(*this, props.openGroup(new_id, true), new_id);
  p.name(name);
  return p;
}

void Section::removeProperty(std::string id) {
  if (props.hasObject(id)) {
    props.removeGroup(id);
  }
}

size_t Section::propertyCount() const {
  size_t count = props.objectCount();
  if (group.hasGroup("values"))
    count--;
  return count;
}

bool Section::operator==(const Section &other) const {
  return section_id == other.section_id;
}

bool Section::operator!=(const Section &other) const {
  return section_id != other.section_id;
}

bool Section::hasProperty(std::string id) const {
  return props.hasGroup(id);
}

bool Section::hasPropertyByName(string name) const {
  for (PropertyIterator iter = properties(); iter != iter.end(); ++iter) {
    Property p = *iter;
    if (p.name() == name)
      return true;
  }
  return false;
}

Section::~Section() {
  //dtor
}

} // end namespace pandora
