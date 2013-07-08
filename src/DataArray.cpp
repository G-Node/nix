#include <pandora/DataArray.hpp>
#include <pandora/DataSet.hpp>

namespace pandora {
DataArray::DataArray(File parentFile, Group thisGroup, std::string identifier) :
  NamedEntityWithSources(&parentFile, thisGroup, identifier) {
  scaling(1.0);
  offset(0.0);
}

DataArray::DataArray(const DataArray &other) :
  NamedEntityWithSources(other.file, other.group, other.id()) {
  scaling(1.0);
  offset(0.0);
}

std::string DataArray::label() const {
  std::string value;
  group.getAttr("label", value);
  return value;
}

void DataArray::label(const std::string &value) {
  group.setAttr("label", value);
}

std::string DataArray::unit() const {
  std::string value;
  group.getAttr("unit", value);
  return value;
}

void DataArray::unit(const std::string &value) {
  group.setAttr("unit", value);
}

void DataArray::scaling(double scaling) {
  group.setAttr("scaling", scaling);
}

double DataArray::scaling() const {
  double scaling;
  group.getAttr("scaling", scaling);
  return scaling;
}

void DataArray::offset(double offset) {
  group.setAttr("offset", offset);
}

double DataArray::offset() const {
  double offset;
  group.getAttr("offset", offset);
  return offset;
}

DataSet DataArray::data() {
  return group.openData("data");
}

} //namespace pandora
