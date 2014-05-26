// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/valid/toBool.hpp>
#include <nix/valid/conditions.hpp>
#include <nix/valid/result.hpp>

using namespace std;

namespace nix {
namespace valid {
    
    template<typename T>
    bool toBool(T var) {
        return (bool)var;
    }
    template<>
    bool toBool<std::vector<std::string>>(std::vector<std::string> var) {
        return !(var.empty());
    }
    template<>
    bool toBool<std::vector<double>>(std::vector<double> var) {
        return !(var.empty());
    }
    template<>
    bool toBool<nix::Section>(nix::Section var) {
        return !(var.isNone());
    }
    template<>
    bool toBool<nix::DataArray>(nix::DataArray var) {
        return !(var.isNone());
    }
    template<>
    bool toBool<nix::LinkType>(nix::LinkType var) {
        return true; // LinkType as enum is always set
    }
    template<>
    bool toBool<std::vector<nix::Value>>(std::vector<nix::Value> var) {
        return !(var.empty());
    }
    template<>
    bool toBool<std::vector<nix::DataArray>>(std::vector<nix::DataArray> var) {
        return !(var.empty());
    }
    template<>
    bool toBool<std::vector<nix::Source>>(std::vector<nix::Source> var) {
        return !(var.empty());
    }
    template<>
    bool toBool<std::string>(std::string var) {
        return (bool)var.length();
    }
    template<>
    bool toBool<none_t>(none_t var) {
        return false;
    }
    
} // namespace valid
} // namespace nix
