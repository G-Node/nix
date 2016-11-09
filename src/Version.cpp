#include <nix/Version.hpp>

namespace nix {
    
    std::vector<int> apiVersion() {
        return std::vector<int> {VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH};
    }

}
