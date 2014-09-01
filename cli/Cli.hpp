// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef CLI_H
#define CLI_H

#include <nix/base/Entity.hpp>
#include <nix/base/NamedEntity.hpp>
#include <nix/base/EntityWithMetadata.hpp>
#include <nix/base/EntityWithSources.hpp>
#include <nix.hpp>

#include <string>
#include <utility>
#include <unordered_map>

namespace cli {
    
    const char *const HELP_OPTION = "help";
    const char *const MODULE_OPTION = "module";
    const char *const INPFILE_OPTION = "input-file";
    
} // namespace cli

#endif
