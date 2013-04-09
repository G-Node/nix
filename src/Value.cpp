#include <pandora/Util.hpp>
#include <pandora/Value.hpp>
#include <pandora/Property.hpp>
#include <pandora/PropertyIterator.hpp>
#include <iostream>

using namespace std;

namespace pandora {

Value::Value(const Value &other) :
  property(other.property), group(other.group), value_id(other.value_id) {
}

Value::Value(Property property, Group group, string id) :
  property(property), group(group), value_id(id) {
}

string Value::id() const {
  return value_id;
}

string Value::filename() const {
  string filename;
  group.getAttr("filename", filename);
  return filename;
}

void Value::filename(string filename) {
  group.setAttr("filename", filename);
}

string Value::checksum() const {
  string checksum;
  group.getAttr("checksum", checksum);
  return checksum;
}

void Value::checksum(std::string checksum) {
  group.setAttr("checksum", checksum);
}

string Value::encoder() const {
  string encoder;
  group.getAttr("encoder", encoder);
  return encoder;
}

void Value::encoder(string encoder) {
  group.setAttr("encoder", encoder);
}
}
