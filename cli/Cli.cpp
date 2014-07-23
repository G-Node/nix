// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>
#include <stdexcept>
#include <string>
#include <iterator>
#include <algorithm>

#include <nix.hpp>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

// A helper function to simplify the main part.
template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " ")); 
    return os;
}

int main(int argc, char* argv[]) {
    std::vector<nix::File> files; // opened nix files
    
    try {
        // declare supported options
        po::options_description desc("Supported options");
        desc.add_options()
            ("help", "produce help message")
            ("input-file", po::value< std::vector<std::string> >(), "input file")
            ("validate", "operation: run the validation on the given input file")
        ;

        // declare that all "positional options" are to be regarded input files
        po::positional_options_description p;
        p.add("input-file", -1);
        // process the cmd line input
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).
                  options(desc).positional(p).run(), vm);
        po::notify(vm);   

        // --help
        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }

        // --input-file
        if (vm.count("input-file")) {
            for(auto &file_path : vm["input-file"].as< std::vector<std::string> >()) {
                files.push_back(nix::File::open(file_path, nix::FileMode::ReadWrite)); // ReadOnly, ReadWrite, Overwrite
            }
            // --validate
            if (vm.count("validate")) {
                for(auto &nix_file : files) {
                    std::cout << "validating file " << nix_file.location() << std::endl;
                    std::cout << nix_file.validate();
                }
            }
            else {
                std::cout << "No operations specified" << std::endl;
            }
        }
        else {
            std::cout << "No input file given" << std::endl;
        }
    }
    catch(std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
        return 1;
    }
    catch(...) {
        std::cerr << "Exception of unknown type!" << std::endl;
    }

    return 0;
}
