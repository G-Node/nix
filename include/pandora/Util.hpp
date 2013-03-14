
#ifndef PAN_UTIL_H_INCLUDED
#define PAN_UTIL_H_INCLUDED

#include <string>

namespace pandora {
namespace util {

std::string createId(std::string prefix = "", int length = 16);

std::string timeToStr(time_t time);

time_t strToTime(std::string time);

} // namespace util
} // namespace pandora


#endif /* PAN_UTIL_H_INCLUDED */
