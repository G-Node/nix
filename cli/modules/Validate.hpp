// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <Cli.hpp>
#include <modules/IModule.hpp>
#include <iostream>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#ifndef CLI_VALIDATE_H
#define CLI_VALIDATE_H

namespace cli {
namespace module {

class Validate : virtual public IModule {
    
public:

    static const char* module_name;

    std::string name() const {
        return std::string(module_name);
    }

    void load(po::options_description &desc) const;

    std::string call(const po::variables_map &vm, const po::options_description &desc) const;

};

} // namespace module
} // namespace cli

#endif
