// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/valid/result.hpp>
#include <vector>
#include <string>

namespace nix {
namespace valid {
    
const char* Result::prefixErr = "ERROR: ";
const char* Result::prefixWarn = "WARNING: ";
const char* Result::prefixID = "ID __ID__: ";

Result::Result(const std::vector<strPair> &errs, 
               const std::vector<strPair> &warns) {
    // assign to member vars
    this->errors = errs;
    this->warnings = warns;
}

Result::Result(none_t t, const std::vector<strPair> &warns) 
    : Result(std::vector<strPair>(), warns) {}

Result::Result(const std::vector<strPair> &errs, none_t t)
    : Result(errs, std::vector<strPair>()) {}

Result::Result(const strPair &err, const strPair &warn) 
    : Result(std::vector<strPair> {err}, std::vector<strPair> {warn}) {}

Result::Result(none_t t, const strPair &warn) 
    : Result(std::vector<strPair>(), std::vector<strPair> {warn}) {}

Result::Result(const strPair &err, none_t t)
    : Result(std::vector<strPair> {err}, std::vector<strPair>()) {}

Result::Result(none_t t, none_t u)
    : Result(std::vector<strPair>(), std::vector<strPair>()) {}
    
void Result::setPrefixes(std::vector<strPair> &errs, std::vector<strPair> &warns) const {
    for(auto it=errs.begin(); it!=errs.end(); ++it) {
        (*it).second.insert(0, prefixErr);
    }
    for(auto it=warns.begin(); it!=warns.end(); ++it) {
        (*it).second.insert(0, prefixWarn);
    }
}
    
void Result::setIdPrefixes(std::vector<strPair> &errs, std::vector<strPair> &warns) const {
    for(auto it=errs.begin(); it!=errs.end(); ++it) {
        // copy prefix in string
        std::string prefixIDcpy = std::string(prefixID);
        // replace placeholder with id
        prefixIDcpy.replace(prefixIDcpy.find("__ID__"), 
                            std::string("__ID__").length(),
                            (*it).first);
        // insert id prefix in msg string
        (*it).second.insert(0, prefixIDcpy);
    }
    for(auto it=warns.begin(); it!=warns.end(); ++it) {
        // copy prefix in string
        std::string prefixIDcpy = std::string(prefixID);
        // replace placeholder with id
        prefixIDcpy.replace(prefixIDcpy.find("__ID__"), 
                            std::string("__ID__").length(),
                            (*it).first);
        // insert id prefix in msg string
        (*it).second.insert(0, prefixIDcpy);
    }
}

std::vector<strPair> Result::getWarnings() const {
    return warnings;
}

std::vector<strPair> Result::getErrors() const {
    return errors;
}

Result Result::concat(const Result &result) {
    errors.reserve(errors.size() + result.errors.size());
    errors.insert(errors.end(), result.errors.begin(), result.errors.end());
    warnings.reserve(warnings.size() + result.warnings.size());
    warnings.insert(warnings.end(), result.warnings.begin(), result.warnings.end());
            
    return *this;
}

bool Result::ok() const {
    return (errors.size() == 0) && (warnings.size() == 0);
}

bool Result::hasErrors() const {
    return errors.size() > 0;
}

bool Result::hasWarnings() const {
    return warnings.size() > 0;
}

} // namespace valid
} // namespace nix
