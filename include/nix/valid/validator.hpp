// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_VALIDATOR_H
#define NIX_VALIDATOR_H

#include <nix/Platform.hpp>
#include <nix/valid/result.hpp>

#include <functional>
#include <cstdarg>
#include <vector>

namespace nix {
namespace valid {
    
/**
  * @brief return type of conditions {@link should} & {@link must}
  * 
  * Actual condition type, return type of conditions functionals
  */
typedef std::function<Result(void)> condition;

/**
  * @brief Generic validator
  * 
  * Function taking a list of conditions that it will all execute and
  * return the results as one merged {@link Result} object
  *
  * @param li initializer list of conditions
  *
  * @returns The validation results as {@link Result} object
  */
NIXAPI Result validator(const std::vector<condition> &li);

}
}

#endif
