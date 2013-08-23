
#include <pandora/Group.hpp>
#include <pandora/Representation.hpp>

using namespace std;

namespace pandora {

string linkTypeToString(LinkType link_type) {
  static vector<string> type_names = {"tagged", "untagged", "indexed"};
  return type_names[static_cast<int>(link_type)];
}


LinkType linkTypeFromString(const string &str) {
  if (str == "tagged")
    return LinkType::TAGGED;
  else if (str == "untagged")
    return LinkType::UNTAGGED;
  else if (str == "indexed")
    return LinkType::INDEXED;
  else
    throw runtime_error("Unable to create a LinkType from the string: " + str);
}


Representation::Representation(const Representation &representation)
  : PandoraEntity(representation.group, representation.entity_id)
{}


Representation::Representation(Group group, string id)
  : PandoraEntity(group, id)
{}


Representation::Representation(Group group, string id, time_t time)
  : PandoraEntity(group, id, time)
{}


void Representation::linkType(LinkType link_type) {
  group.setAttr("link_type", linkTypeToString(link_type));
  forceUpdatedAt();
}


LinkType Representation::linkType() const {
  string link_type;
  group.getAttr("link_type", link_type);
  return linkTypeFromString(link_type);
}


Representation::~Representation() {
  //dtor
}

} // end namespace pandora
