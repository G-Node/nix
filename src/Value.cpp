#include <pandora/Util.hpp>
#include <pandora/Value.hpp>
#include <pandora/Property.hpp>
#include <pandora/PropertyIterator.hpp>
#include <iostream>

using namespace std;

namespace pandora {

namespace hades {
template<>
char *get_inner<char*, std::string>(const std::string &outer) {
  return const_cast<char *>(outer.c_str());
}

template<>
int8_t get_inner<int8_t, bool>(const bool &outer) {
  return static_cast<int8_t>(outer);
}
}


}
