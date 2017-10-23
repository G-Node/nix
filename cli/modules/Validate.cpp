// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <Cli.hpp>
#include <modules/Validate.hpp>
#include <nix/None.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

namespace cli {
namespace module {
    
const char* Validate::module_name = "validate";

void Validate::load(po::options_description &desc) const {
    desc.add(po::options_description("nix-tool " + std::string(module_name) + ":\n\n\t" + 
                                     "Validates a given nix-file for structural an logical correctness.\n\nSupported options"));
    po::options_description opt;
    opt.add_options()
        (NOWARN_OPTION, "ignore any warnings")
        (NOERR_OPTION, "ignore any errors")
    ;
    desc.add(opt);
}


std::ostream &print(std::ostream &out,
                    const std::vector<nix::valid::Message> &msgs,
                    bool isError) {

    for (const auto &msg : msgs) {
        if (!msg.id.empty())
            out << "ID " << msg.id << " ";
        if (msg.name)
            out << "['" << *msg.name << "'] ";
        out << (isError ? "ERROR:" : "WARNING: ");
        out << msg.msg << std::endl;
    }

    return out;
}

std::string Validate::call(const po::variables_map &vm, const po::options_description &desc) {
    std::vector<nix::File> files; // opened nix files
    std::stringstream out;

    // --help
    if (vm.count(HELP_OPTION)) {
        po::options_description temp;
        load(temp);
        out << temp << std::endl;
        return out.str();
    }

    // --input-file
    if (! vm.count(INPFILE_OPTION)) {
        throw NoInputFile();
    }

    for (auto &file_path : vm[INPFILE_OPTION].as< std::vector<std::string> >()) {
        nix::File tmp_file;

        if (!boost::filesystem::exists(file_path)) {
            throw FileNotFound(file_path);
        }

        tmp_file = nix::File::open(file_path, nix::FileMode::ReadOnly);

        if (!tmp_file.isOpen()) {
            throw FileNotOpen(file_path);
        }

        files.push_back(tmp_file);
    }

    for (auto &nix_file : files) {
        out << "validating file " << nix_file.location() << std::endl;
        const nix::valid::Result &res = nix_file.validate();

        if (res.hasWarnings() && ! vm.count(NOWARN_OPTION)) {
            const std::vector<nix::valid::Message> &warnings = res.getWarnings();
            print(out, warnings, false);
        }

        if (res.hasErrors() && ! vm.count(NOERR_OPTION)) {
            const std::vector<nix::valid::Message> &errors = res.getErrors();
            print(out, errors, true);

        }
    }

    std::cout << std::endl;

    return out.str();
}

} // namespace module
} // namespace cli
