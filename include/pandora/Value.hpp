#ifndef PAN_VALUE_H_INCLUDE
#define PAN_VALUE_H_INCLUDE

#include <string>
#include <pandora/Group.hpp>
#include <pandora/File.hpp>

namespace pandora {

template<typename T>
struct FileValue {
  T value;
  double uncertainty;
  char *filename;
  char *encoder;
  char *checksum;
  char *reference;
};

template<typename T>
struct Value {
  T value;
  double uncertainty;
  std::string filename;
  std::string encoder;
  std::string checksum;
  std::string reference;

  Value() {
  }
  Value(const T &val) :
    value(val) {
  }
  Value(const T &val, double uc, std::string fn, std::string en, std::string ck, std::string rf) :
    value(val), uncertainty(uc), filename(fn), encoder(en), checksum(ck), reference(rf) {
  }
  Value(const FileValue<T> &that) :
    value(that.value), uncertainty(that.uncertainty), filename(that.filename),
        encoder(that.encoder), checksum(that.checksum), reference(that.reference) {
  }

  template<typename U>
  Value& operator=(const FileValue<U> &that) {
    value = that.value;
    uncertainty = that.uncertainty;
    filename = that.filename;
    reference = that.reference;
    encoder = that.encoder;
    checksum = that.checksum;
    return *this;
  }

  template<typename U>
  FileValue<U> toValueBase(U) const {
    FileValue<U> base;
    base.filename = const_cast<char *> (filename.c_str());
    base.encoder = const_cast<char *> (encoder.c_str());
    base.checksum = const_cast<char *> (checksum.c_str());
    base.reference = const_cast<char *> (reference.c_str());
    base.value = value;
    return base;
  }
};

template<> template<typename U>
FileValue<U> Value<std::string>::toValueBase(const U) const {
  FileValue<U> base;
  base.filename = const_cast<char *> (filename.c_str());
  base.encoder = const_cast<char *> (encoder.c_str());
  base.checksum = const_cast<char *> (checksum.c_str());
  base.reference = const_cast<char *> (reference.c_str());
  base.value = const_cast<char *> (value.c_str());
  return base;
}

template<> template<typename U>
FileValue<U> Value<bool>::toValueBase(const U) const {
  FileValue<U> base;
  base.filename = const_cast<char *> (filename.c_str());
  base.encoder = const_cast<char *> (encoder.c_str());
  base.checksum = const_cast<char *> (checksum.c_str());
  base.reference = const_cast<char *> (reference.c_str());
  if (value)
    base.value = (int8_t) 1;
  else
    base.value = (int8_t) 0;
  return base;
}

typedef Value<std::string> StringValue;
typedef Value<double> DoubleValue;
typedef Value<int64_t> LongValue;
typedef Value<int32_t> IntValue;
typedef Value<bool> BoolValue;

/* **** */
namespace hades {

template<typename T>
H5::DataType ValueToMemtype() {
  TypeSpec<T> spec;
  H5::CompType memtype(sizeof(FileValue<typename TypeSpec<T>::inner_type> ));
  memtype.insertMember("value", HOFFSET(StringValue, value), spec.memType);

  memtype.insertMember("uncertainty", HOFFSET(StringValue, uncertainty),
      H5::PredType::NATIVE_DOUBLE);
  memtype.insertMember("reference", HOFFSET(StringValue, reference), H5::StrType(
      H5::PredType::C_S1, H5T_VARIABLE));
  memtype.insertMember("filename", HOFFSET(StringValue, filename), H5::StrType(H5::PredType::C_S1,
      H5T_VARIABLE));
  memtype.insertMember("encoder", HOFFSET(StringValue, encoder), H5::StrType(H5::PredType::C_S1,
      H5T_VARIABLE));
  memtype.insertMember("checksum", HOFFSET(StringValue, checksum), H5::StrType(H5::PredType::C_S1,
      H5T_VARIABLE));
  return memtype;
}

template<typename T>
struct TypeSpec<Value<T> > {

  static const bool is_valid = true;
  typedef typename TypeSpec<T>::inner_type inner_type;

  TypeSpec() :
    fileType(ValueToMemtype<T> ()), memType(ValueToMemtype<T> ()) {
  }

  const H5::DataType fileType;
  const H5::DataType memType;

};

template<typename T>
class TypeInfo<std::vector<Value<T> > > {
public:
  typedef Value<T> element_type;
  typedef std::vector<Value<T> > myType;

  static PSize shape(const myType &value) {
return  PSize( {value.size()});}

static size_t num_elements(const myType &value) {
  return value.size();
}

static const element_type* getData(const myType &value) {
  return &value[0];
}

static element_type* getData(myType &value) {
  return &value[0];
}

static void resize(myType &value, const PSize &dims) {
  if (dims.size() != 1) {
    throw InvalidRankException("Cannot resize scalar");
  }
  value.resize(dims[0]);
}
};


template<typename Inner, typename Outer>
Inner get_inner(const Outer &outer) {
  return outer;
}

template<> char *get_inner<char*, std::string>(const std::string &outer);
template<> int8_t get_inner<int8_t, bool>(const bool &outer);

template<
typename T,
template <typename> class ValueBox,
typename U
>
class DataBox<const T, ValueBox, Value<U> > {
public:
typedef ValueBox<const T> &vbox_ref;
typedef typename ValueBox<T>::inner_type inner_type;
typedef FileValue<inner_type> data_type;
typedef data_type *data_ptr;

DataBox(vbox_ref val) : value(val) {
  size_t nelms = value.size();
  data = new data_type[nelms];
  const Value<U> *vptr = value.get_data();
  for (size_t i = 0; i < nelms; i++) {
    FileValue<inner_type> val;
    val.value = get_inner<inner_type,U>(vptr[i].value);
    val.uncertainty = vptr[i].uncertainty;
    val.reference = get_inner<char *,std::string>(vptr[i].reference);
    val.encoder = get_inner<char *,std::string>(vptr[i].encoder);
    val.checksum = get_inner<char *,std::string>(vptr[i].checksum);
    val.filename = get_inner<char *,std::string>(vptr[i].filename);
    data[i] = val;
  }
}

data_ptr operator *() {return get();}
data_ptr get() const {return data;}
void finish(const H5::DataSpace *space = nullptr) {}

~DataBox() {
  delete[] data;
}

private:



data_ptr data;
vbox_ref value;
};


template<
typename T,
template <typename> class ValueBox,
typename U
>
class DataBox<T, ValueBox, Value<U> > {
public:
typedef ValueBox<T> &vbox_ref;
typedef typename ValueBox<T>::inner_type inner_type;
typedef FileValue<inner_type> data_type;
typedef data_type *data_ptr;

DataBox(vbox_ref val) : value(val) {
  size_t nelms = value.size();
  data = new data_type[nelms];
}

data_ptr operator *() {return get();}
data_ptr get() {return data;}

void finish(const H5::DataSpace *space = nullptr) {
  size_t nelms = value.size();
  Value<U> *vptr = value.get_data();

  for (size_t i = 0; i < nelms; i++) {
    vptr[i] = data[i];
  }

  if (space) {
    H5::DataSet::vlenReclaim(data, value.memType, *space);
  }
}

~DataBox() {
  delete[] data;
}

private:
data_ptr data;
vbox_ref value;
};

} //namespace hades
} //namespace pandora

#endif /* VALUE_HPP_ */
