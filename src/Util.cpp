// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <string>
#include <cstdlib>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <pandora/Util.hpp>

using namespace std;

namespace pandora {
namespace util {

// default id base (16 or 32)
const int    ID_BASE = 32;
// Base32hex alphabet (RFC 4648)
const char*  ID_ALPHABET = "0123456789abcdefghijklmnopqrstuv";


string createId(string prefix, int length) {
    static bool initialized = false;
    if(!initialized) {
        initialized = true;
        srand(time(NULL));
    }
    string id;
    if (prefix.length() > 0) {
        id.append(prefix);
        id.append("_");
    }
    for (int i = 0; i < length; i++) {
        char c = ID_ALPHABET[(size_t) (((double) (rand())) / RAND_MAX * ID_BASE)];
        id.push_back(c);
    }
    return id;
}


string timeToStr(time_t time) {
    using namespace boost::posix_time;
    ptime timetmp = from_time_t(time);
    return to_iso_string(timetmp);
}


time_t strToTime(const string &time) {
    using namespace boost::posix_time;
    ptime timetmp(from_iso_string(time));
    ptime epoch(boost::gregorian::date(1970, 1, 1));
    return (timetmp - epoch).total_seconds();
}

} // namespace util
} // namespace pandora
