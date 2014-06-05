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

Result::Result(const std::vector<std::string> &errors, 
               const std::vector<std::string> &warnings) {
    // assign to member vars
    this->errors = errors;
    this->warnings = warnings;
}

Result::Result(none_t t, const std::vector<std::string> &_warnings) 
    : Result(std::vector<std::string>(), _warnings) {}

Result::Result(const std::vector<std::string> &_errors, none_t t)
    : Result(_errors, std::vector<std::string>()) {}

Result::Result(const std::string &_errors, const std::string &_warnings) 
    : Result(std::vector<std::string> {_errors}, std::vector<std::string> {_warnings}) {}

Result::Result(none_t t, const std::string &_warnings) 
    : Result(std::vector<std::string>(), std::vector<std::string> {_warnings}) {}

Result::Result(const std::string &_errors, none_t t)
    : Result(std::vector<std::string> {_errors}, std::vector<std::string>()) {}

Result::Result(none_t t, none_t u)
    : Result(std::vector<std::string>(), std::vector<std::string>()) {}
    
void Result::setPrefixes(std::vector<std::string> &_errors, std::vector<std::string> &_warnings) const {
    for(auto it=_errors.begin(); it!=_errors.end(); ++it) {
        (*it).insert(0, prefixErr);
    }
    for(auto it=_warnings.begin(); it!=_warnings.end(); ++it) {
        (*it).insert(0, prefixWarn);
    }
}

std::vector<std::string> Result::getWarnings() const {
    return warnings;
}

std::vector<std::string> Result::getErrors() const {
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
