// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/valid/validator.hpp>
#include <nix/valid/result.hpp>

namespace nix {
namespace valid {

Result validator(const std::vector<condition> &li) {
    Result result = Result();

    for (auto &sub : li) {
         result = result.concat(
            sub()
        );
    }

    return result;
}

}
}
