#ifndef PAN_VALUE_H_INCLUDE
#define PAN_VALUE_H_INCLUDE

#include <string>
#include <pandora/Group.hpp>
#include <pandora/Property.hpp>
#include <pandora/File.hpp>

const size_t STRLENGTH = 512;
const size_t VALUE_STR_LENGTH = 2500;

namespace pandora {

struct StringValue{
   char value[1251];
   double uncertainty;
   char filename[257];
   char encoder[257];
   char checksum[257];
   char reference[257];
} val;

class Value {

  union panValue {
    int intValue;
    float floatValue;
    double doubleValue;
    std::string *stringValue;
  };



public:

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
  const std::string filename;
  const std::string encoder;
  const std::string checksum;
  const std::string reference;
  const double uncertainty;
  const std::string dataType;
  const panValue value;

};

}

#endif /* VALUE_HPP_ */
