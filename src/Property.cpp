#include <pandora/Property.hpp>
#include <pandora/Util.hpp>
#include <iostream>
#include <stdexcept>

using namespace std;

namespace pandora {

Property::Property(const Property &property) :
          section(property.section), group(property.group), property_id(property.property_id) {
  // nothing to do
}

Property::Property(Section section, Group group, string id) :
          section(section), group(group), property_id(id) {
}

string Property::id() const {
  return property_id;
}

void Property::name(string name) {
  if (valueCount() > 0 && this->name().length() > 0) {
    throw std::runtime_error("Cannot change name of a property that contains values!");
    return;
  }
  group.setAttr("name", name);
}

string Property::name() const {
  string name;
  group.getAttr("name", name);
  return name;
}

void Property::dataType(string dataType) {
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

void Property::addStringValue(const std::string value, const std::string &reference,
    const std::string filename, const std::string encoder, const std::string checksum) {
  string dt = this->dataType();
  if (dt.length() > 0 && dt.compare("string") != 0) {
    throw std::runtime_error("Value and data type do not match!");
    return;
  } else {
    dataType("string");
  }

  StringValue v1[1];
  v1[0].value = (char*)value.c_str();
  v1[0].uncertainty = 0.0;
  v1[0].reference = (char*)reference.c_str();
  v1[0].encoder = (char*)encoder.c_str();
  v1[0].checksum = (char*)checksum.c_str();
  v1[0].filename = (char*)filename.c_str();

  hsize_t dim[1] = { 1 };
  H5::DataSpace space(1, dim);
  H5::CompType mtype = Value::stringValueMemType();
  H5::DataSet* dataset;
  dataset = new H5::DataSet(group.h5Group().createDataSet("values", mtype, space));
  dataset->write(v1, mtype);
}

string Property::stringValue(size_t index) const {
  StringValue value[1];
  stringValue(index, value[0]);
  return value[0].value;
}

void Property::stringValue(size_t index, StringValue &value) const {
  if (group.hasData("values")) {
    if (index < 0 || index >= valueCount()) {
      throw std::runtime_error("Property::stringValue(index): Index out of bounds!");
    }
    H5::DataSet dataset = group.openData("values");
    if (!checkDataType(dataset, H5T_STRING)) {
      throw std::runtime_error("Property::stringValue(index): Value DataType is not String!");
    }
    H5::CompType mtype = Value::stringValueMemType();
    StringValue temp[1];
    dataset.read(temp, mtype);
    value = temp[0];
  }
}

void Property::removeValue(size_t index){
  if (group.hasData("values")) {
    if (index < 0 || index >= valueCount()) {
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
    H5::DataSet dataset = group.openData("values");
    H5::DataSpace space = dataset.getSpace();
    int dims = space.getSimpleExtentNdims();
    hsize_t dimSize[dims];
    space.getSimpleExtentDims(dimSize);
    count = dimSize[0];
  }
  return count;
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
