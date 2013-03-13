#include <string>
#include <cstdlib>
#include <ctime>

#include <pandora/Util.hpp>

using namespace std;

namespace pandora {
namespace util {

string createId(string prefix = "", int length = ID_LENGTH) {
  static bool initialized = false;
  if(!initialized) {
    srand(time(NULL));
  }
  string id;
  if (prefix.length() > 0) {
    id.append(prefix);
    id.append("_");
  }
  for (int i = 0; i < length; i++) {
    char c = hex[(size_t) (((double) (rand())) / RAND_MAX * ID_BASE)];
    id.push_back(c);
  }
  return id;
}

} // namespace util
} // namespace pandora
