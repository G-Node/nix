#include <pandora/Section.hpp>
#include <pandora/SectionIterator.hpp>
#include <pandora/Util.hpp>
#include <pandora/Property.hpp>
#include <pandora/PropertyIterator.hpp>
#include <iostream>

using namespace std;

namespace pandora {

Section::Section(const Section &section) :
  file(section.file), group(section.group), section_id(section.section_id) {
  // nothing to do
  props = section.props;
}

Section::Section(File file, Group group, string id) :
  file(file), group(group), section_id(id) {
  // nothing to do
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
  group.setAttr("link", link);
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

Section Section::addSection(std::string name, std::string type){
  string new_id = util::createId("subsection");
  while(group.hasObject(new_id))
    new_id = util::createId("subsection");
  Section s = file.createSection(name,type);
  s.parent(id());
  cout << s.id() << "\t" << s.name() << "\t" << s.type() << "\t" << s.parent() << endl;
  return s;
}

SectionIterator Section::children() const {
  SectionIterator iter(this->file, group, id());
  return iter;
}

PropertyIterator Section::properties() const {
  PropertyIterator iter(this->file, props);
  return iter;
}

bool Section::operator==(const Section &other) const {
  return section_id == other.section_id;
}

bool Section::operator!=(const Section &other) const {
  return section_id != other.section_id;
}

Section::~Section() {
  //dtor
}

} // end namespace pandora
