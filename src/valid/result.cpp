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

Result::Result(const std::vector<Message> &errs,
               const std::vector<Message> &warns) {
    // assign to member vars
    this->errors = errs;
    this->warnings = warns;
}

Result::Result(none_t t, const std::vector<Message> &warns)
    : Result(std::vector<Message>(), warns) {}

Result::Result(const std::vector<Message> &errs, none_t t)
    : Result(errs, std::vector<Message>()) {}

Result::Result(const Message &err, const Message &warn)
    : Result(std::vector<Message> {err}, std::vector<Message> {warn}) {}

Result::Result(none_t t, const Message &warn)
    : Result(std::vector<Message>(), std::vector<Message> {warn}) {}

Result::Result(const Message &err, none_t t)
    : Result(std::vector<Message> {err}, std::vector<Message>()) {}

Result::Result(none_t t, none_t u)
    : Result(std::vector<Message>(), std::vector<Message>()) {}

void Result::setPrefixes(std::vector<Message> &errs, std::vector<Message> &warns) const {
    for (auto &err : errs) {
        err.msg.insert(0, prefixErr);
    }
    for (auto &warn : warns) {
        warn.msg.insert(0, prefixWarn);
    }
}

void Result::setIdPrefixes(std::vector<Message> &errs, std::vector<Message> &warns) const {
    for (auto &err : errs) {
        // copy prefix in string
        std::string prefixIDcpy = std::string(prefixID);
        // replace placeholder with id
        prefixIDcpy.replace(prefixIDcpy.find("__ID__"),
                            std::string("__ID__").length(),
                            err.id);
        // insert id prefix in msg string
        err.msg.insert(0, prefixIDcpy);
    }
    for (auto &warn : warns) {
        // copy prefix in string
        std::string prefixIDcpy = std::string(prefixID);
        // replace placeholder with id
        prefixIDcpy.replace(prefixIDcpy.find("__ID__"),
                            std::string("__ID__").length(),
                            warn.id);
        // insert id prefix in msg string
        warn.msg.insert(0, prefixIDcpy);
    }
}

std::vector<Message> Result::getWarnings() const {
    return warnings;
}

std::vector<Message> Result::getErrors() const {
    return errors;
}

Result Result::concat(const Result &result) {
    errors.reserve(errors.size() + result.errors.size());
    errors.insert(errors.end(), result.errors.begin(), result.errors.end());
    warnings.reserve(warnings.size() + result.warnings.size());
    warnings.insert(warnings.end(), result.warnings.begin(), result.warnings.end());

    return *this;
}

Result Result::addError(const Message &error) {
    errors.push_back(error);
    return *this;
}

Result Result::addWarning(const Message &warning) {
    warnings.push_back(warning);
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
