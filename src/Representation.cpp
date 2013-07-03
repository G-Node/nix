#include <pandora/Representation.hpp>

using namespace std;

namespace pandora {

Representation::Representation(const Representation &representation) :
  PandoraEntity(representation.file, representation.group, representation.entity_id) {
  rdata = representation.rdata;
  rtype = representation.rtype;
}

Representation::Representation(File *file, H5::Group group, std::string id,
    std::string data_array_id, RepresentationType type) :
  PandoraEntity(file, group, id), rdata(data_array_id), rtype(type) {

}

Representation::~Representation() {
  //dtor
}

} // end namespace pandora
