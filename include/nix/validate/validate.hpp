// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#ifndef NIX_VALIDATE_H
#define NIX_VALIDATE_H

#include <boost/logic/tribool.hpp>
#include <nix/validate/result.hpp>
#include <nix/validate/conditions.hpp>
#include <cstdarg>

namespace nix {
namespace validation {

void test();

/**
  * Generic validator
  *
  * @param vector vector of conditions
  *
  * @returns object The validation results as {@Result} object
  */
Result validate(vector<condition> conditions);

/**
  * Generic validator
  *
  * @param initializer_list initializer list of conditions
  *
  * @returns object The validation results as {@Result} object
  */
Result validate(initializer_list<condition> li);

} // namespace validation
} // namespace nix

#endif // NIX_VALIDATE_H
