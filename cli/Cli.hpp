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
#include <stdexcept>

namespace cli {
    
    const char *const HELP_OPTION = "help";
    const char *const MODULE_OPTION = "module";
    const char *const INPFILE_OPTION = "input-file";

class NoInputFile : public std::invalid_argument {
public:
    NoInputFile() : std::invalid_argument("No input file given") { }
};

class FileNotFound : public std::invalid_argument {
public:
    FileNotFound(std::string file_name) :
            std::invalid_argument("File '" + file_name + "' not found") { }
};

class FileNotOpen : public std::invalid_argument {
public:
    FileNotOpen(std::string file_name) :
            std::invalid_argument("File '" + file_name + "' could not be opened - wrong format?") { }
};

} // namespace cli

#endif
