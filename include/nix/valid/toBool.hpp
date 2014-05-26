// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#ifndef NIX_TOBOOL_H
#define NIX_TOBOOL_H

#include <string>
#include <vector>
#include <nix.hpp>

namespace nix {
namespace valid {
    
    template<typename T>
    bool toBool(T var);
    template<>
    bool toBool<std::vector<std::string>>(std::vector<std::string> var);
    template<>
    bool toBool<std::vector<double>>(std::vector<double> var);
    template<>
    bool toBool<nix::Section>(nix::Section var);
    template<>
    bool toBool<nix::DataArray>(nix::DataArray var);
    template<>
    bool toBool<nix::LinkType>(nix::LinkType var);
    template<>
    bool toBool<std::vector<nix::Value>>(std::vector<nix::Value> var);
    template<>
    bool toBool<std::vector<nix::DataArray>>(std::vector<nix::DataArray> var);
    template<>
    bool toBool<std::vector<nix::Source>>(std::vector<nix::Source> var);
    template<>
    bool toBool<std::string>(std::string var);
    template<>
    bool toBool<none_t>(none_t var);
    
} // namespace valid
} // namespace nix

#endif // NIX_TOBOOL_H
