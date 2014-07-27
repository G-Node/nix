// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <Cli.hpp>
#include <modules/Validate.hpp>
#include <Exception.hpp>
#include <nix.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

namespace cli {
namespace module {
    
const char* Validate::module_name = "validate";

void Validate::load(po::options_description &desc) const {
    // declare supported options
    desc.add_options()
        ("validate", "operation: run the validation on the given input file")
    ;
}

std::string Validate::call(const po::variables_map &vm, const po::options_description &desc) const {
    std::vector<nix::File> files; // opened nix files
    std::stringstream out;
    nix::File tmp_file;
    
    // --input-file
    if (vm.count(INPFILE_OPTION)) {
        for(auto &file_path : vm[INPFILE_OPTION].as< std::vector<std::string> >()) {
            // file exists?
            if (!boost::filesystem::exists(file_path)) {
                throw FileNotFound(file_path);
            }
            // try to open!
            tmp_file = nix::File::open(file_path, nix::FileMode::ReadWrite);
            // file opened?
            if (!tmp_file.isOpen()) {
                throw FileNotOpen(file_path);
            }
            // save it!
            files.push_back(tmp_file); // ReadOnly, ReadWrite, Overwrite
        }
        for(auto &nix_file : files) {
            out << "validating file " << nix_file.location() << std::endl;
            out << nix_file.validate();
        }
    }
    else {
        throw NoInputFile();
    }
    
    if (vm.count(cli::HELP_OPTION)) {
        out << desc << std::endl;
    }
    
    return out.str();
}

} // namespace module
} // namespace cli
