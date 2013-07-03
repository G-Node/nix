#include <pandora/DataArray.hpp>
#include <pandora/DataSet.hpp>

namespace pandora {
DataArray::DataArray(File parentFile, Group thisGroup, std::string identifier)
  : fd(parentFile), group(thisGroup), myid(identifier)
{

}

DataArray::DataArray(const DataArray &other)
  : fd(other.fd), group(other.group), myid(other.myid)
{

}

std::string DataArray::type() const
{
  std::string value;
  group.getAttr("type", value);
  return value;
}

void DataArray::type(const std::string &value)
{
  group.setAttr("type", value);
}


std::string DataArray::name() const
{
  std::string value;
  group.getAttr("name", value);
  return value;
}

void DataArray::name(const std::string &value)
{
  group.setAttr("name", value);
}


std::string DataArray::definition() const
{
  std::string value;
  group.getAttr("definition", value);
  return value;
}

void DataArray::definition(const std::string &value)
{
  group.setAttr("definition", value);
}


std::string DataArray::label() const
{
  std::string value;
  group.getAttr("label", value);
  return value;
}

void DataArray::label(const std::string &value)
{
  group.setAttr("label", value);
}


std::string DataArray::unit() const
{
  std::string value;
  group.getAttr("unit", value);
  return value;
}

void DataArray::unit(const std::string &value)
{
  group.setAttr("unit", value);
}


DataSet DataArray::data()
{
  return group.openData("data");
}

} //namespace pandora