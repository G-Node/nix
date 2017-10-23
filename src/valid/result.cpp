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

Result::Result(none_t t, const Message &warn)
    : Result(std::vector<Message>(), std::vector<Message> {warn}) {}

Result::Result(const Message &err, none_t t)
    : Result(std::vector<Message> {err}, std::vector<Message>()) {}

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
