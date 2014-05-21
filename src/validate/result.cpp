// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/validate/result.hpp>
#include <vector>
#include <string>

using namespace std;

namespace nix {
namespace validate {

Result::Result(const vector<string> &_errors, const vector<string> &_warnings) {
    errors = _errors;
    warnings = _warnings;
}

Result::Result(none_t t, const vector<string> &_warnings) 
    : Result(vector<string>(), _warnings) {}

Result::Result(const vector<string> &_errors, none_t t)
    : Result(_errors, vector<string>()) {}

Result::Result(const string &_errors, const string &_warnings) 
    : Result(vector<string> {_errors}, vector<string> {_warnings}) {}

Result::Result(none_t t, const string &_warnings) 
    : Result(vector<string>(), vector<string> {_warnings}) {}

Result::Result(const string &_errors, none_t t)
    : Result(vector<string> {_errors}, vector<string>()) {}

Result::Result(none_t t, none_t u)
    : Result(vector<string>(), vector<string>()) {}

vector<string> Result::all() const {
    vector<string> all;
    
    all.reserve(errors.size() + warnings.size());
    all.insert(all.end(), errors.begin(), errors.end());
    all.insert(all.end(), warnings.begin(), warnings.end());

    return all;
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

} // namespace validate
} // namespace nix
