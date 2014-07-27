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

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <nix.hpp>
#include <modules/IModule.hpp>
#include <Cli.hpp>
#include <modules/Validate.hpp>
#include <Exception.hpp>

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
    {std::string(cli::module::Validate::module_name), std::shared_ptr<cli::module::IModule>(new cli::module::Validate())}
};

} // namespace cli

int main(int argc, char* argv[]) {
    std::stringstream out;
    po::variables_map vm; // will contain the parsed options
    po::command_line_parser parser(argc, argv); // parser to use
    po::options_description desc("Supported options");
    po::positional_options_description pdesc;
      
    try {
        
        // declare supported options
        desc.add_options()
            (cli::HELP_OPTION, "produce help message")
            (cli::MODULE_OPTION, po::value< std::string >(), cli::MODULE_OPTION)
            (cli::INPFILE_OPTION, po::value< std::vector<std::string> >(), cli::INPFILE_OPTION)
        ;
        // declare supported "positional options"
        pdesc.add(cli::MODULE_OPTION, 1)  // treat first positional option as "module"
        .add(cli::INPFILE_OPTION, -1); // treat every further positional option  as "input-file"s
        // process the cmd line input once to get module name
        po::store(parser.options(desc)
                  .positional(pdesc)
                  .style(po::command_line_style::case_insensitive)
                  .allow_unregistered()
                  .run(), vm);
        po::notify(vm);
        
        // load & call module
        std::string name = !vm.count(cli::MODULE_OPTION) ? "" :
                            vm[cli::MODULE_OPTION].as<std::string>();
        auto it = cli::modules.find(name);
        if(it != cli::modules.end()) {
            (*it).second->load(desc);
            // process the cmd line input
            po::store(parser.options(desc).positional(pdesc).run(), vm);
            po::notify(vm);
            out << (*it).second->call(vm, desc);
        }
        else {
            out << std::endl << "unknown module" << std::endl << std::endl;
            // handle help
            //if (vm.count(cli::HELP_OPTION)) {
                out << desc << std::endl;
            //}
        }
        
        // show output
        std::cout << out.str();
    }
    catch(std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
        std::cout << std::endl << desc << std::endl;
        return 1;
    }
    catch(...) {
        std::cerr << "Exception of unknown type!" << std::endl;
        std::cout << std::endl << desc << std::endl;
    }

    return 0;
}
