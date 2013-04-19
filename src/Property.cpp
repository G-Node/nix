#include <pandora/Property.hpp>
#include <pandora/Util.hpp>
#include <iostream>

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

void Property::addStringValue(const std::string value, const std::string &reference, const std::string filename, const std::string encoder, const std::string checksum){
  StringValue v1[1];
  const char *ptr = value.c_str();
  for(size_t i = 0; i < value.length(); i++){
    v1[0].value[i] = *ptr;
    ptr++;
  }
  v1[0].uncertainty = 0.0;
  std::cerr << v1[0].value << std::endl;
    /*
  v1[0].reference = reference;
  v1[0].checksum = checksum;
  v1[0].encoder = encoder;
  v1[0].filename = filename;
   */
  hsize_t dim[] = {1};
  H5::DataSpace space( 1, dim );
  hid_t longstrtype = H5Tcopy (H5T_C_S1);
  hid_t strtype = H5Tcopy (H5T_C_S1);
  H5Tset_size (strtype, 257);
  H5Tset_size (strtype, 1251);

  H5::CompType mtype( sizeof(StringValue) );
  mtype.insertMember( "value", HOFFSET(StringValue, value), longstrtype);
  mtype.insertMember( "uncertainty", HOFFSET(StringValue, uncertainty), H5::PredType::NATIVE_DOUBLE );
  mtype.insertMember( "reference", HOFFSET(StringValue, reference), strtype);
  mtype.insertMember( "filename", HOFFSET(StringValue, filename), strtype);
  mtype.insertMember( "encoder", HOFFSET(StringValue, encoder), strtype);
  mtype.insertMember( "checksum", HOFFSET(StringValue, checksum), strtype);

  H5::DataSet* dataset;
  dataset = new H5::DataSet(group.h5Group().createDataSet("values", mtype, space));
  dataset->write( v1, mtype );
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
