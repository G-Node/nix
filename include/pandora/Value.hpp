#ifndef PAN_VALUE_H_INCLUDE
#define PAN_VALUE_H_INCLUDE

#include <string>
#include <pandora/Group.hpp>
#include <pandora/File.hpp>

const size_t STRLENGTH = 512;
const size_t VALUE_STR_LENGTH = 2500;

namespace pandora {

struct StringValue{
  char *value;
  double uncertainty;
  char *filename;
  char *encoder;
  char *checksum;
  char *reference;
};

class Value {

  union panValue {
    int intValue;
    float floatValue;
    double doubleValue;
    std::string *stringValue;
  };



public:

  static H5::CompType stringValueMemType(){

    H5::CompType mtype(sizeof(StringValue));
    hid_t valtype = H5Tcopy (H5T_C_S1);
    H5Tset_size (valtype, H5T_VARIABLE);
    mtype.insertMember("value", HOFFSET(StringValue, value), valtype);
    mtype.insertMember("uncertainty", HOFFSET(StringValue, uncertainty), H5::PredType::NATIVE_DOUBLE);
    hid_t reftype = H5Tcopy (H5T_C_S1);
    H5Tset_size (reftype, H5T_VARIABLE);
    mtype.insertMember("reference", HOFFSET(StringValue, reference), reftype);
    hid_t filetype = H5Tcopy (H5T_C_S1);
    H5Tset_size (filetype, H5T_VARIABLE);
    mtype.insertMember("filename", HOFFSET(StringValue, filename), filetype);
    hid_t enctype = H5Tcopy (H5T_C_S1);
    H5Tset_size (enctype, H5T_VARIABLE);
    mtype.insertMember("encoder", HOFFSET(StringValue, encoder), enctype);
    hid_t chktype = H5Tcopy (H5T_C_S1);
    H5Tset_size (chktype, H5T_VARIABLE);
    mtype.insertMember("checksum", HOFFSET(StringValue, checksum), chktype);
    return mtype;
  };

  Value(const Value &value);

  Value(std::string value, double uncertainty = 0.0, std::string filename = "",
      std::string checksum = "", std::string encoder = "", std::string reference = "");

  Value(int value, double uncertainty = 0.0, std::string filename = "",
      std::string checksum = "", std::string encoder = "", std::string reference = "");

  Value(double value, double uncertainty = 0.0, std::string filename = "",
      std::string checksum = "", std::string encoder = "", std::string reference = "");

  Value(float value, double uncertainty = 0.0, std::string filename = "",
      std::string checksum = "", std::string encoder = "", std::string reference = "");



public:


};

}

#endif /* VALUE_HPP_ */
