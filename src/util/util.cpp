// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <string>
#include <cstdlib>
#include <mutex>
#include <random>
#include <math.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/regex.hpp>
#include <nix/util/util.hpp>
#include <boost/random.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <nix/base/IDimensions.hpp>


using namespace std;

namespace nix {
namespace util {

// Base32hex alphabet (RFC 4648)
const char*  ID_ALPHABET = "0123456789abcdefghijklmnopqrstuv";
// Unit scaling, SI only, substitutions for micro and ohm...
const string  PREFIXES = "(Y|Z|E|P|T|G|M|k|h|da|d|c|m|u|n|p|f|a|z|y)";
const string  UNITS = "(m|g|s|A|K|mol|cd|Hz|N|Pa|J|W|C|V|F|S|Wb|T|H|lm|lx|Bq|Gy|Sv|kat|l|L|Ohm|%|dB|rad)";
const string  POWER = "(\\^[+-]?[1-9]\\d*)";

const map<string, double> PREFIX_FACTORS = {{"y", 1.0e-24}, {"z", 1.0e-21}, {"a", 1.0e-18}, {"f", 1.0e-15},
    {"p", 1.0e-12}, {"n",1.0e-9}, {"u", 1.0e-6}, {"m", 1.0e-3}, {"c", 1.0e-2}, {"d",1.0e-1}, {"da", 1.0e1}, {"h", 1.0e2},
    {"k", 1.0e3}, {"M",1.0e6}, {"G", 1.0e9}, {"T", 1.0e12}, {"P", 1.0e15}, {"E",1.0e18}, {"Z", 1.0e21}, {"Y", 1.0e24}};


string createId() {
    static boost::mt19937 ran(std::time(0));
    static boost::uuids::basic_random_generator<boost::mt19937> gen(&ran);
    boost::uuids::uuid u = gen();
    return boost::uuids::to_string(u);
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


time_t getTime() {
    return time(NULL);
}


void deblankString(std::string &str) {
    typedef std::string::value_type char_type;

    str.erase(std::remove_if(str.begin(),
                             str.end(),
                             [](char_type c) { return std::isblank(c); }),
              str.end());
}

std::string deblankString(const std::string &str) {
    std::string str_copy = str;
    deblankString(str_copy);
    return str_copy;
}

bool nameCheck(const std::string &name) {
    return name.find("/") == std::string::npos;
}

std::string nameSanitizer(const std::string &name) {
    std::string str_copy = name;
    size_t pos = str_copy.find("/");
    
    while(pos != std::string::npos) {
        str_copy = str_copy.replace(pos, 1, "_");
        pos = str_copy.find("/");
    }
    
    return str_copy;    
}

std::string unitSanitizer(const std::string &unit) {
     std::string new_unit = deblankString(unit);
     while (new_unit.find("mu") != string::npos) {
          size_t pos = new_unit.find("mu");
          new_unit = new_unit.replace(pos, 2, "u");
     }
     while (new_unit.find("µ") != string::npos) {
          size_t pos = new_unit.find("µ");
          new_unit = new_unit.replace(pos, 2, "u");
     }
     return new_unit;
}

void splitUnit(const string &combinedUnit, string &prefix, string &unit, string &power) {
    boost::regex prefix_and_unit_and_power(PREFIXES + UNITS + POWER);
    boost::regex prefix_and_unit(PREFIXES + UNITS);
    boost::regex unit_and_power(UNITS + POWER);
    boost::regex unit_only(UNITS);
    boost::regex prefix_only(PREFIXES);

    if (boost::regex_match(combinedUnit, prefix_and_unit_and_power)) {
        boost::match_results<std::string::const_iterator> m;
        boost::regex_search(combinedUnit, m, prefix_only);
        prefix = m[0];
        string suffix = m.suffix();
        boost::regex_search(suffix, m, unit_only);
        unit = m[0];
        power = m.suffix();
        power = power.substr(1);
    } else if (boost::regex_match(combinedUnit, unit_and_power)) {
        prefix = "";
        boost::match_results<std::string::const_iterator> m;
        boost::regex_search(combinedUnit, m, unit_only);
        unit = m[0];
        power = m.suffix();
        power = power.substr(1);
    } else if (boost::regex_match(combinedUnit, prefix_and_unit)) {
        boost::match_results<std::string::const_iterator> m;
        boost::regex_search(combinedUnit, m, prefix_only);
        prefix = m[0];
        unit = m.suffix();
        power = "";
    } else {
        unit = combinedUnit;
        prefix = "";
        power = "";
    }
}


void splitCompoundUnit(const std::string &compoundUnit, std::vector<std::string> &atomicUnits) {
    string s = compoundUnit;
    boost::regex opt_prefix_and_unit_and_power(PREFIXES + "?" + UNITS + POWER + "?");
    boost::regex separator("(\\*|/)");
    boost::match_results<std::string::const_iterator> m;

    while (boost::regex_search(s, m, opt_prefix_and_unit_and_power) && (m.suffix().length() > 0)) {
        string suffix = m.suffix();
        atomicUnits.push_back(m[0]);
        s = suffix.substr(1);
    }
    atomicUnits.push_back(m[0]);
}


bool isSIUnit(const string &unit) {
    boost::regex opt_prefix_and_unit_and_power(PREFIXES + "?" + UNITS + POWER + "?");
    return boost::regex_match(unit, opt_prefix_and_unit_and_power);
}


bool isCompoundSIUnit(const string &unit) {
    string atomic_unit = PREFIXES + "?" + UNITS + POWER + "?";
    boost::regex compound_unit("(" + atomic_unit + "(\\*|/))+"+ atomic_unit);
    return boost::regex_match(unit, compound_unit);
}


std::string dimTypeToStr(const nix::DimensionType &dtype) {
    std::stringstream s;
    s << dtype;
    return s.str();
}


bool isScalable(const string &unitA, const string &unitB) {
    if (!(isSIUnit(unitA) && isSIUnit(unitB))) {
        return false;
    }
    string a_unit, a_prefix, a_power;
    string b_unit, b_prefix, b_power;
    splitUnit(unitA, a_prefix, a_unit, a_power);
    splitUnit(unitB, b_prefix, b_unit, b_power);
    if (!(a_unit == b_unit) || !(a_power == b_power) ) {
        return false;
    }
    return true;
}


bool isScalable(const vector<string> &unitsA, const vector<string> &unitsB) {
    bool scalable = true;
    
    if (unitsA.size() != unitsB.size()) {
        return false;
    }
    
    auto itA = unitsA.begin();
    auto itB = unitsB.begin();
    while (scalable && itA != unitsA.end()) {
        scalable = isScalable(*itA, *itB);
        ++itA; 
        ++itB;
    }
    
    return scalable;
}


bool isSetAtSamePos(const vector<string> &unitsA, const vector<string> &unitsB) {
    bool set_same = true;
    
    if (unitsA.size() != unitsB.size()) {
        return false;
    }
    
    auto itA = unitsA.begin();
    auto itB = unitsB.begin();
    while (set_same && itA != unitsA.end()) {
        set_same = (*itA).empty() == (*itB).empty();
        ++itA; 
        ++itB;
    }
    
    return set_same;
}


double getSIScaling(const string &originUnit, const string &destinationUnit) {
    double scaling = 1.0;
    if (!isScalable(originUnit, destinationUnit)) {
        throw nix::InvalidUnit("Origin unit and destination unit are not scalable versions of the same SI unit!",
                               "nix::util::getSIScaling");
    }
    
    string org_unit, org_prefix, org_power;
    string dest_unit, dest_prefix, dest_power;
    splitUnit(originUnit, org_prefix, org_unit, org_power);
    splitUnit(destinationUnit, dest_prefix, dest_unit, dest_power);

    if ((org_prefix == dest_prefix) && (org_power == dest_power)) {
        return scaling;
    }
    if (dest_prefix.empty() && !org_prefix.empty()) {
        scaling = PREFIX_FACTORS.at(org_prefix);
    } else if (org_prefix.empty() && !dest_prefix.empty()) {
        scaling = 1.0 / PREFIX_FACTORS.at(dest_prefix);
    } else if (!org_prefix.empty() && !dest_prefix.empty()) {
        scaling = PREFIX_FACTORS.at(org_prefix) / PREFIX_FACTORS.at(dest_prefix);
    }
    if (!org_power.empty()) {
        int power = std::stoi(org_power);
        scaling = pow(scaling, power);
    }
    return scaling;
}

double applyPolynomial(std::vector<double> &coefficients, double origin, double input) {
    double value = 0.0;
    double term = 1.0;
    for (size_t i = 0; i < coefficients.size(); i++) {
        value += coefficients[i] * term;
        term *= input - origin;
    }
    return value;
}

} // namespace util
} // namespace nix
