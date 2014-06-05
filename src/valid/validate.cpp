// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/valid/validate.hpp>
#include <nix/valid/checks.hpp>
#include <nix/valid/conditions.hpp>
#include <nix/valid/result.hpp>

namespace nix {
namespace valid {

Result validate(std::initializer_list<condition> li) {
    Result result = Result();
    
    for (auto it = li.begin(); it != li.end(); ++it) {
         result = result.concat(
            (*it)()
        );
    }
    
    return result;
}

} // namespace valid
} // namespace nix
