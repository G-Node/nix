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

  Value() {}
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
  if(value)
    base.value = (int32_t) 1;
  else
    base.value = (int32_t) 0;
  return base;
}

typedef Value<std::string> StringValue;
typedef Value<double> DoubleValue;
typedef Value<int64_t> LongValue;
typedef Value<int32_t> IntValue;
typedef Value<bool> BoolValue;

template<typename T>
struct ValueInfo {

};

template<typename T, typename U = T>
struct MemoryType {
  MemoryType() :
    memtype(sizeof(FileValue<U> )) {
    valtype = H5Tcopy(H5T_C_S1);
    H5Tset_size(valtype, H5T_VARIABLE);
    hades::TypeSpec<T> spec;
    //memtype.insertMember("value", HOFFSET(StringValue, value), valtype);
    memtype.insertMember("value", HOFFSET(StringValue, value), spec.memType);
    memtype.insertMember("uncertainty", HOFFSET(StringValue, uncertainty),
        H5::PredType::NATIVE_DOUBLE);
    reftype = H5Tcopy(H5T_C_S1);
    H5Tset_size(reftype, H5T_VARIABLE);
    memtype.insertMember("reference", HOFFSET(StringValue, reference), reftype);
    filetype = H5Tcopy(H5T_C_S1);
    H5Tset_size(filetype, H5T_VARIABLE);
    memtype.insertMember("filename", HOFFSET(StringValue, filename), filetype);
    enctype = H5Tcopy(H5T_C_S1);
    H5Tset_size(enctype, H5T_VARIABLE);
    memtype.insertMember("encoder", HOFFSET(StringValue, encoder), enctype);
    chktype = H5Tcopy(H5T_C_S1);
    H5Tset_size(chktype, H5T_VARIABLE);
    memtype.insertMember("checksum", HOFFSET(StringValue, checksum), chktype);
  }
  H5::CompType memtype;
  hid_t chktype, enctype, reftype, valtype, filetype;
  ~MemoryType() {
    //H5Tclose(chktype);
    //H5Tclose(enctype);
    //H5Tclose(reftype);
    //H5Tclose(valtype);
    //H5Tclose(filetype);
  }

  U get(U d) {
    return d;
  }

  typedef U inner_type;
};

template<>
struct ValueInfo<std::string> :public MemoryType<std::string, char *> {
  std::string type = "string";
  H5T_class_t h5class = H5T_STRING;
  char* get(std::string s) {
    return (char*) s.c_str();
  }
};

template<>
struct ValueInfo<bool> :public MemoryType<bool, int32_t> {
  std::string type = "bool";
  H5T_class_t h5class = H5T_INTEGER;
  int32_t get(bool b) {
    if(b)
      return (int32_t)1;
    return (int32_t)0;
  }
};

template<>
struct ValueInfo<int16_t> {
  std::string type = "int16";
  H5T_class_t h5class = H5T_INTEGER;
};

template<>
struct ValueInfo<int32_t> :public MemoryType<int32_t> {
  std::string type = "int32";
  H5T_class_t h5class = H5T_INTEGER;
};


template<>
struct ValueInfo<int64_t> :public MemoryType<int64_t> {
  std::string type = "int64";
  H5T_class_t h5class = H5T_INTEGER;
};


template<>
struct ValueInfo<double> :public MemoryType<double> {
  std::string type = "double";
  H5T_class_t h5class = H5T_FLOAT;
};

}

#endif /* VALUE_HPP_ */
