#include <pandora/Representation.hpp>

using namespace std;

namespace pandora {

Representation::Representation(const Representation &representation) :
    PandoraEntity(representation.file, representation.group,
        representation.entity_id) {
  data(representation.data());
  type(representation.type());
}

Representation::Representation(File *file, H5::Group group, std::string id,
    std::string data_array_id, RepresentationType representationType) :
    PandoraEntity(file, group, id) {
  data(data_array_id);
  type(representationType);
}

void Representation::data(std::string data_array_id) {
  group.setAttr("data", data_array_id);
  forceUpdatedAt();
}

void Representation::data(const DataArray &data_array) {
  data(data_array.id());
}

std::string Representation::data() const {
  string rdata;
  group.getAttr("data", rdata);
  return rdata;
}

void Representation::type(RepresentationType rtype) {
  std::string repType;
  switch (rtype) {
  case RepresentationType::tagged:
    repType = "tagged";
    break;
  case RepresentationType::untagged:
    repType = "untagged";
    break;
  case RepresentationType::indexed:
    repType = "indexed";
    break;
  default:
    break;
  }
  group.setAttr("type", repType);
  forceUpdatedAt();
}

RepresentationType Representation::type() const {
  string rtype;
  group.getAttr("type", rtype);
  if (rtype.compare("indexed") == 0)
    return RepresentationType::indexed;
  else if (rtype.compare("untagged") == 0)
    return RepresentationType::indexed;
  else
    return RepresentationType::tagged;
}

Representation::~Representation() {
  //dtor
}

} // end namespace pandora
