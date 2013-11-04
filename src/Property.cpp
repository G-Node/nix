#include <iostream>
#include <stdexcept>

#include <pandora/Property.hpp>

using namespace std;

namespace pandora {

Property::Property(const Property &property) :
    NamedEntity(property.group, property.entity_id){
}

Property::Property(Group group, const std::string &id) :
    NamedEntity(group, id){
}

Property::Property(Group group,const std::string &id, time_t time):
  NamedEntity(group,id,time){
}

void Property::dataType(const string &dataType) {
  string dt = this->dataType();
  if (dt.compare(dataType) == 0) {
    return;
  } else {
    if (valueCount() > 0 && this->dataType().length() > 0) {
      throw std::runtime_error("Cannot change data type of a not empty property!");
      return;
    }
  }
  group.setAttr("data_type", dataType);
}

string Property::dataType() const {
  string dataType;
  group.getAttr("data_type", dataType);
  return dataType;
}

void Property::mapping(const string &mapping) {
  group.setAttr("mapping", mapping);
}

string Property::mapping() const {
  string mapping;
  group.getAttr("mapping", mapping);
  return mapping;
}

void Property::unit(const string &unit) {
  if (valueCount() > 0 && this->unit().length() > 0) {
    throw std::runtime_error("Cannot change unit of a not-empty property!");
    return;
  }
  group.setAttr("unit", unit);
}

string Property::unit() const {
  string unit;
  group.getAttr("unit", unit);
  return unit;
}


void Property::removeValue(size_t index){
  if (group.hasData("values")) {
    if (index >= valueCount()) {
      throw std::runtime_error("Property::stringValue(index): Index out of bounds!");
    }
    //TODO
  }
}

void Property::removeValues(){
  this->group.removeData("values");
}

bool Property::checkDataType(const H5::DataSet &dataset, H5T_class_t destType) const {
  H5::DataType type = dataset.getDataType();
  if (type.getClass() != H5T_COMPOUND) {
    return false;
  }
  H5::CompType ct(dataset);
  if (ct.getMemberDataType(ct.getMemberIndex("value")).getClass() != destType) {
    return false;
  }
  return true;
}

size_t Property::valueCount() const {
  size_t count = 0;
  if (group.hasData("values")) {
    DataSet dataset = group.openData("values");
    PSize size = dataset.size();
    return size[0];
  }
  return count;
}

bool Property::operator==(const Property &other) const {
  return entity_id == other.entity_id;
}

bool Property::operator!=(const Property &other) const {
  return entity_id != other.entity_id;
}

Property::~Property() {
  //dtor
}

} // end namespace pandora
