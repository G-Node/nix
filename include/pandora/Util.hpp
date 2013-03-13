
#ifndef PAN_UTIL_H_INCLUDED
#define PAN_UTIL_H_INCLUDED

#include <string>

namespace pandora {
namespace util {

// default id length
const int    ID_LENGTH = 16;
// default id base (16 or 32)
const int    ID_BASE = 32;
// Base32hex alphabet (RFC 4648)
const char*  ID_ALPHABET = "0123456789abcdefghijklmnopqrstuv";

std::string createId(std::string prefix = "", int length = ID_LENGTH);

} // namespace util
} // namespace pandora


#endif /* PAN_UTIL_H_INCLUDED */
