#include <pandora/DataArray.hpp>
#include <pandora/DataSet.hpp>

namespace pandora {
DataArray::DataArray(File parentFile, Group thisGroup, std::string identifier) :
          NamedEntityWithSources(&parentFile, thisGroup, identifier) {
  expansionOrigin(0.0);
  double temp[1];
  polynom(0, temp);
}

DataArray::DataArray(const DataArray &other) :
          NamedEntityWithSources(other.file, other.group, other.id()) {
  expansionOrigin(0.0);
  double temp[1];
  polynom(0, temp);
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

void DataArray::expansionOrigin(double expansion_origin) {
  group.setAttr("expansion_origin", expansion_origin);
}

double DataArray::expansionOrigin() const {
  double expansion_origin;
  group.getAttr("expansion_origin", expansion_origin);
  return expansion_origin;
}
/*
void DataArray::polynomOrder(int order) {
  group.setAttr("polynom_order", order);
}
 */

int DataArray::polynomOrder() const {
  int order;
  group.getAttr("polynom_order", order);
  return order;
}

void DataArray::polynomCoefficients(std::vector<double> &coefficients){
  ;
}
std::vector<double> DataArray::polynomCoefficients()const{

}

void DataArray::setCalibration(std::vector<double> &coefficients, double origin){

}

DataSet DataArray::data() {
  return group.openData("data");
}

} //namespace pandora
