#ifndef PAN_VALUE_H_INCLUDE
#define PAN_VALUE_H_INCLUDE

#include <string>
#include <pandora/Group.hpp>
#include <pandora/Property.hpp>
#include <pandora/File.hpp>

namespace pandora {

class Value
{
private:
  Property property;
  Group group;
  std::string value_id;

public:

  Value(const Value &value);

  Value(Property property, Group group, std::string id);

  std::string id() const;

  std::string filename() const;
  void filename(std::string filename);

  std::string checksum() const;
  void checksum(std::string checksum);

  std::string encoder() const;
  void encoder(std::string);

  /*
   * attribute:uncertainty = ???
       attribute:uncertainty = ???
   */
};

}

#endif /* VALUE_HPP_ */
