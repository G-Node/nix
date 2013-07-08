#include <pandora/DataArray.hpp>
#include <pandora/DataSet.hpp>

namespace pandora {
DataArray::DataArray(File parentFile, Group thisGroup, std::string identifier)
  : NamedEntityWithSources(&parentFile,thisGroup, identifier)
{

}

DataArray::DataArray(const DataArray &other)
  : NamedEntityWithSources(other.file, other.group, other.id())
{

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
