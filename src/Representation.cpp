
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
  : PandoraEntity(representation.group, representation.entity_id), block(representation.block)
{}


Representation::Representation(Group group, const string &id, Block block)
  : PandoraEntity(group, id), block(block)
{}


Representation::Representation(Group group, const string &id, time_t time, Block block)
  : PandoraEntity(group, id, time), block(block)
{}


void Representation::linkType(LinkType link_type) {
  group.setAttr("link_type", linkTypeToString(link_type));
  forceUpdatedAt();
}


void Representation::data(const DataArray &data){
  group.setAttr("data", data.id());
  forceUpdatedAt();
}

DataArray Representation::data() const{
  string dataId;
  group.getAttr("data", dataId);
  return block.getDataArray(dataId);
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
