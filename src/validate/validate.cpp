// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/validate/validate.hpp>
#include <nix/validate/checks.hpp>
#include <nix/validate/conditions.hpp>
#include <nix/validate/result.hpp>

using namespace std;

namespace nix {
namespace validate {
    
void test() {
}

Result validator(vector<conditionType> conditions) {
    Result result = Result();

    for (size_t j=0; j<conditions.size(); j++) {
        result = result.concat(
            conditions[j]()
        );
    }

    return result;
}

} // namespace validate
} // namespace nix
