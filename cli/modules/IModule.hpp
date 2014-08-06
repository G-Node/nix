// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef CLI_IMODULE_H
#define CLI_IMODULE_H

#include <string>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <nix/base/Entity.hpp>
#include <nix/base/NamedEntity.hpp>
#include <nix/base/EntityWithMetadata.hpp>
#include <nix/base/EntityWithSources.hpp>
#include <nix.hpp>

namespace cli {
namespace module {

class IModule {
public:

    /**
     * @brief Get the module name.
     *
     * Constant string with the module name set only once upon creation
     * of each module, by the module. 
     *
     * @return string name of the module
     */
    virtual std::string name() const = 0;

    /**
     * @brief Loader for the module.
     *
     * Module loader that has to be called with an 
     * boost::program_options::options_description object that it
     * will add itself to. 
     *
     * @param desc boost::program_options::options_description object
     * @return void
     */
    virtual void load(po::options_description &desc) const = 0;
    
    /**
     * @brief Call the module.
     *
     * Module caller that will execute all the tasks the module is
     * meant to execute, depending on the options supplied via the
     * boost::program_options::variables_map object.
     *
     * @param vm boost::program_options::variables_map object
     * @return string with the module response for the user
     */
    virtual std::string call(const po::variables_map &vm, const po::options_description &desc) = 0;
    
};

} // namespace module
} // namespace cli

#endif
