#ifndef PAN_PROPERTY_H_INCLUDE
#define PAN_PROPERTY_H_INCLUDE

#include <string>
#include <H5Cpp.h>

#include <pandora/Group.hpp>
#include <pandora/Section.hpp>
#include <pandora/Value.hpp>

namespace pandora {

class Property {

private:

  Section section;
  Group group;
  std::string property_id;

  bool checkDataType(const H5::DataSet &dataset, H5T_class_t type) const;

public:

  Property(const Property &property);

  Property(Section section, Group group, std::string id);

  std::string id() const;

  void name(std::string name);
  std::string name() const;

  void definition(std::string definition);
  std::string definition() const;

  void link(std::string link);
  std::string link() const;

  void include(std::string include);
  std::string include() const;

  void mapping(std::string mapping);
  std::string mapping() const;

  void dataType(std::string dataType);
  std::string dataType() const;

  void unit(std::string unit);
  std::string unit() const;

  template<typename T>
  void addValue(const Value<T> &value);

  template<typename T>
  void addValue(const T value, double uncertainty = 0.0, const std::string &reference = "", const std::string filename = "",
      const std::string encoder = "", const std::string checksum = "");


  template<typename T>
  void value(size_t index, Value<T> &value) const;

  void removeValue(size_t index);

  void removeValues();

  size_t valueCount() const;

  //void addValue(int value, const std::string &reference, const std::string &filename, const std::string filename, const std::string encoder, const std::string checksum);


  bool operator==(const Property &other) const;

  bool operator!=(const Property &other) const;

  virtual ~Property();

};

template<typename T>
void Property::addValue(T value, double uncertainty, const std::string &reference, const std::string filename,
    const std::string encoder, const std::string checksum) {
  ValueInfo<T> info;
  std::string dt = this->dataType();
  if (dt.length() > 0 && dt.compare(info.type) != 0) {
    throw std::runtime_error("Value and data type do not match!");
    return;
  } else {
    dataType(info.type);
  }

  FileValue<typename ValueInfo<T>::inner_type> val;

  val.value = info.get(value);
  val.uncertainty = uncertainty;
  val.reference = (char*) reference.c_str();
  val.encoder = (char*) encoder.c_str();
  val.checksum = (char*) checksum.c_str();
  val.filename = (char*) filename.c_str();

  hsize_t dim[1] = { 1 };
  H5::DataSpace space(1, dim);
  H5::DataSet* dataset;
  dataset = new H5::DataSet(group.h5Group().createDataSet("values", info.memtype, space));
  dataset->write(&val, info.memtype);
}

template<typename T>
void Property::addValue(const Value<T> &value)  {
  ValueInfo<T> info;
  std::string dt = this->dataType();
  if (dt.length() > 0 && dt.compare(info.type) != 0) {
    throw std::runtime_error("Value and data type do not match!");
    return;
  } else {
    dataType(info.type);
  }

  typedef typename ValueInfo<T>::inner_type inner_type;
  FileValue<inner_type> val;
  val = value.toValueBase(val.value);

  hsize_t dim[1] = { 1 };
  H5::DataSpace space(1, dim);
  H5::DataSet* dataset;
  dataset = new H5::DataSet(group.h5Group().createDataSet("values", info.memtype, space));
  dataset->write(&val, info.memtype);
}

template<typename T>
void Property::value(size_t index, Value<T> &value) const {
  if (group.hasData("values")) {
    if (index < 0 || index >= valueCount()) {
      throw std::runtime_error("Property::stringValue(index): Index out of bounds!");
    }
    H5::DataSet dataset = group.openData("values");
    ValueInfo<T> info;
    if (!checkDataType(dataset, info.h5class)) {
      throw std::runtime_error("Property::stringValue(index): Value DataType is not String!");
    }
    FileValue<typename ValueInfo<T>::inner_type> fileValue;
    dataset.read(&fileValue, info.memtype);
    value = fileValue;
    H5::DataSet::vlenReclaim(&fileValue, info.memtype, dataset.getSpace());
  }
}
}

#endif // HDX_PROPERTY_H_INCLUDE
