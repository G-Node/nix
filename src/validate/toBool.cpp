// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/validate/conditions.hpp>
#include <nix/validate/result.hpp>

using namespace std;

namespace nix {
namespace validate {
    
    template<typename T>
    bool TtoBool(T var) {
        return (bool)var;
    }
    template<>
    bool TtoBool<std::vector<std::string>>(std::vector<std::string> var) {
        return !(var.empty());
    }
    template<>
    bool TtoBool<std::vector<double>>(std::vector<double> var) {
        return !(var.empty());
    }
    template<>
    bool TtoBool<nix::Section>(nix::Section var) {
        return !(var.isNone());
    }
    template<>
    bool TtoBool<nix::DataArray>(nix::DataArray var) {
        return !(var.isNone());
    }
    template<>
    bool TtoBool<nix::LinkType>(nix::LinkType var) {
        return true; // LinkType as enum is always set
    }
    template<>
    bool TtoBool<std::vector<nix::Value>>(std::vector<nix::Value> var) {
        return !(var.empty());
    }
    template<>
    bool TtoBool<std::vector<nix::DataArray>>(std::vector<nix::DataArray> var) {
        return !(var.empty());
    }
    template<>
    bool TtoBool<std::vector<nix::Source>>(std::vector<nix::Source> var) {
        return !(var.empty());
    }
    template<>
    bool TtoBool<std::string>(std::string var) {
        return (bool)var.length();
    }
    
} // namespace validate
} // namespace nix
