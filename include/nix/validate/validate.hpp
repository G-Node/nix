// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#ifndef NIX_VALIDATE_H
#define NIX_VALIDATE_H

#include <nix/validate/result.hpp>
#include <nix/validate/conditions.hpp>

namespace nix {
namespace validate {

void test();

/**
  * Generic validator
  *
  * @param vector vector of conditions
  *
  * @returns object The validation results as {@Result} object
  */
Result validator(vector<nix::validate::conditionType> conditions);

} // namespace validate
} // namespace nix

#endif // NIX_VALIDATE_H
