#ifndef PAN_PROPERTY_H_INCLUDE
#define PAN_PROPERTY_H_INCLUDE

#include <string>
#include <H5Cpp.h>

#include <pandora/Group.hpp>
#include <pandora/File.hpp>

namespace pandora {

class Property
{

private:

  File file;
  Group group, val;
  std::string property_id;


public:

  Property(const Property &property);

  Property(File file, Group group, std::string id);

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


  bool operator==(const Property &other) const;

  bool operator!=(const Property &other) const;

  virtual ~Property();

};

}

#endif // HDX_PROPERTY_H_INCLUDE
