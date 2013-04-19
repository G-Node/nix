#ifndef PAN_PROPERTY_H_INCLUDE
#define PAN_PROPERTY_H_INCLUDE

#include <string>
#include <H5Cpp.h>

#include <pandora/Group.hpp>
#include <pandora/Section.hpp>
#include <pandora/Value.hpp>

namespace pandora {



struct DoubleValue{
   double value;
   double uncertainty;
   std::string filename;
   std::string encoder;
   std::string checksum;
   std::string reference;
};

struct IntValue{
   int value;
   double uncertainty;
   std::string filename;
   std::string encoder;
   std::string checksum;
   std::string reference;
};

struct BoolValue{
   bool value;
   std::string filename;
   std::string encoder;
   std::string checksum;
   std::string reference;
};

class Property
{

private:

  Section section;
  Group group;
  std::string property_id;


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

  void addStringValue(const std::string value, const std::string &reference= "", const std::string filename = "", const std::string encoder = "", const std::string checksum = "");

  //void addValue(int value, const std::string &reference, const std::string &filename, const std::string filename, const std::string encoder, const std::string checksum);


  bool operator==(const Property &other) const;

  bool operator!=(const Property &other) const;

  virtual ~Property();

};

}

#endif // HDX_PROPERTY_H_INCLUDE
