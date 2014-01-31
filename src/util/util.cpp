// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <string>
#include <cstdlib>
#include <regex>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <nix/util/util.hpp>

using namespace std;

namespace nix {
namespace util {

// default id base (16 or 32)
const int    ID_BASE = 32;
// Base32hex alphabet (RFC 4648)
const char*  ID_ALPHABET = "0123456789abcdefghijklmnopqrstuv";
// Unit scaling, SI only, substitutions for micro and ohm...
const char*  UNIT_PREFIXES = "(Y|Z|E|P|T|G|M|k|h|da|d|c|m|u|n|p|f|a|z|y)";
const char*  UNITS = "(m|g|s|A|K|mol|cd|Hz|N|Pa|J|W|C|V|F|S|Wb|T|H|lm|lx|Bq|Gy|Sv|kat|l|L|Ohm|%)";
const std::map<char, float> PREFIX_FACTORS = {{'y', 1.0e-24}, {'z', 1.0e-21}, {'a', 1.0e-18}, {'f', 1.0e-15},
	{'p', 1.0e-12}, {'n',1.0e-9}, {'u', 1.0e-6}, {'m', 1.0e-3}, {'c', 1.0e-2}, {'d',1.0e-1}, {'da', 1.0e1}, {'h', 1.0e2},
	{'k', 1.0e3}, {'M',1.0e6}, {'G', 1.0e9}, {'T', 1.0e12}, {'P', 1.0e15}, {'E',1.0e18}, {'Z', 1.0e21}, {'Y', 1.0e24}};

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


float getScaling(const string &origin_unit, const string &destination_unit){
	//Method stub  to filled with content....

	return 1.0;
}

} // namespace util
} // namespace nix
