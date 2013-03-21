#include <pandora/Property.hpp>
#include <pandora/Util.hpp>

using namespace std;

namespace pandora {

Property::Property(const Property &property) :
  file(property.file), group(property.group), val(property.val), property_id(property.property_id) {
  // nothing to do
}

Property::Property(File file, Group group, string id) :
  file(file), group(group), property_id(id) {
  val = group.openGroup("values");
}

string Property::id() const {
  return property_id;
}

void Property::name(string name) {
  group.setAttr("name", name);
}

string Property::name() const {
  string name;
  group.getAttr("name", name);
  return name;
}

void Property::dataType(string dataType) {
  group.setAttr("data_type", dataType);
}

string Property::dataType() const {
  string dataType;
  group.getAttr("data_type", dataType);
  return dataType;
}

void Property::definition(string definition) {
  group.setAttr("definition", definition);
}

string Property::definition() const {
  string definition;
  group.getAttr("definition", definition);
  return definition;
}

void Property::mapping(string mapping) {
  group.setAttr("mapping", mapping);
}

string Property::mapping() const {
  string mapping;
  group.getAttr("mapping", mapping);
  return mapping;
}

void Property::unit(string unit) {
  group.setAttr("unit", unit);
}

string Property::unit() const {
  string unit;
  group.getAttr("unit", unit);
  return unit;
}


bool Property::operator==(const Property &other) const {
  return property_id == other.property_id;
}

bool Property::operator!=(const Property &other) const {
  return property_id != other.property_id;
}

Property::~Property() {
  //dtor
}

} // end namespace pandora
