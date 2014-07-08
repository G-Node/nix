// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#ifndef NIX_VALIDATE_H
#define NIX_VALIDATE_H

#include <nix/valid/result.hpp>
#include <nix/valid/conditions.hpp>
#include <cstdarg>

namespace nix {
namespace valid {

/**
  * Generic validator
  *
  * @param initializer_list initializer list of conditions
  *
  * @returns object The validation results as {@Result} object
  */
NIXAPI Result validate(std::initializer_list<condition> li);

} // namespace valid
} // namespace nix

#endif // NIX_VALIDATE_H
