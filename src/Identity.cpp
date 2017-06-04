#include <nix/Identity.hpp>
#include <nix/util/util.hpp>

namespace nix {

Identity::Identity(const std::string &name_or_id, ObjectType type)
    : myType(type) {

    if (util::looksLikeUUID(name_or_id)) {
        myId = name_or_id;
    } else {
        myName = name_or_id;
    }
}

} //nix::
