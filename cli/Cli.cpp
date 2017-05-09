// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

#include <boost/any.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <nix/base/Entity.hpp>
#include <nix/base/NamedEntity.hpp>
#include <nix/base/EntityWithMetadata.hpp>
#include <nix/base/EntityWithSources.hpp>
#include <nix.hpp>

#include <Cli.hpp>
#include <modules/IModule.hpp>
#include <modules/Validate.hpp>
#include <modules/Dump.hpp>

namespace cli {

// A helper function to simplify the main part.
template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " ")); 
    return os;
}

// define all module types
std::unordered_map<std::string, std::shared_ptr<cli::module::IModule>> modules = {
    {std::string(cli::module::Validate::module_name), std::shared_ptr<cli::module::IModule>(new cli::module::Validate())},
    {std::string(cli::module::Dump::module_name), std::shared_ptr<cli::module::IModule>(new cli::module::Dump())}
};

} // namespace cli

int main(int argc, char* argv[]) {
    std::stringstream out;
    po::variables_map vm; // will contain the parsed options
    po::command_line_parser parser1(argc, argv);
    po::command_line_parser parser2(argc, argv);
    po::command_line_parser parser3(argc, argv);
    po::options_description desc("Supported options");
    po::positional_options_description pdesc;
    std::string name;
    size_t i;
      
    try {
        // create string list of all modules
        std::stringstream mod_list;
        mod_list << cli::MODULE_OPTION << ": ";
        mod_list << "use 'module name --help' for more information on the specific tool. Avalibale modules are:\n\t";
           
        i = 0;
        for (auto &mod : cli::modules) {
            i++;
            mod_list << i << ": ";
            mod_list << mod.first << (cli::modules.size() != i ? ",\n\t" : "");
        }
        // declare generally supported options
        desc.add_options()
            (cli::HELP_OPTION, "produce help message")
            (cli::MODULE_OPTION, po::value< std::string >(), mod_list.str().c_str())
            (cli::INPFILE_OPTION, po::value< std::vector<std::string> >(), cli::INPFILE_OPTION)
        ;
        // declare generally supported "positional options"
        pdesc.add(cli::MODULE_OPTION, 1)  // treat first positional option as "module"
        .add(cli::INPFILE_OPTION, -1); // treat every further positional option  as "input-file"s
        
        // parse input once to check for HELP_OPTION, mistaken as pos-option otherwise
        po::store(parser1.options(desc)
                  // WARNING: this should work, instead breaks everything (thanks boost!!)
                  //.style(po::command_line_style::case_insensitive)
                  .allow_unregistered()
                  .run(), vm);
        po::notify(vm);
        
        // parse input again (with pos-options) to get MODULE_OPTION & INPFILE_OPTIONs
        po::store(parser2.options(desc)
                  .positional(pdesc)
                  //.style(po::command_line_style::case_insensitive)
                  .allow_unregistered()
                  .run(), vm);
        po::notify(vm);
        // get name
        name = !vm.count(cli::MODULE_OPTION) ? "" :
                         vm[cli::MODULE_OPTION].as<std::string>();
        
        // load & call module
        auto it = cli::modules.find(name);
        if (it != cli::modules.end()) {
            (*it).second->load(desc);    
            // process the cmd line input
            po::store(parser3.options(desc).positional(pdesc).run(), vm);
            po::notify(vm);
            out << (*it).second->call(vm, desc);
        }
        else {
            nix::FormatVersion myVersion{nix::apiVersion()};

            out << std::endl << "Nix command line tool " << std::endl;
            out << "\tNix version: " << myVersion << std::endl << std::endl;
            out << "\tUse the modules of this tool to dump nix-file contents as yaml to std out\n";
            out << "\tor validate the nix file to detect structural and/or logical errors.\n\n";
            out << "\tUsage: ./nix-tool module [--help] [[module args] input-file] \n\n";
            out << desc << std::endl;
        }
        
        // show output
        std::cout << out.str();
    }
    catch(std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
        std::cout << std::endl << "module " << name << std::endl;
        std::cout << desc << std::endl;
        return 1;
    }
    catch(...) {
        std::cerr << "Exception of unknown type!" << std::endl;
        std::cout << std::endl << desc << std::endl;
    }

    return 0;
}
