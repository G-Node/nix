// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/*#include <nix/base/Entity.hpp>
#include <nix/base/NamedEntity.hpp>
#include <nix/base/EntityWithMetadata.hpp>
#include <nix/base/EntityWithSources.hpp>
#include <nix.hpp>*/

//#include <Cli.hpp>
#include <modules/Dump.hpp>
#include <Exception.hpp>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

namespace cli {
namespace module {

const char* Dump::module_name = "dump";
const char* yamlstream::indent_str = "    ";
const char* yamlstream::sequ_start = ":\n";
const char* yamlstream::sequ_end = "";
const char* yamlstream::scalar_start = ": ";
const char* yamlstream::scalar_end = "\n";
const char* yamlstream::item_str = "- ";

void Dump::load(po::options_description &desc) const {
    // declare purpose
    desc.add(po::options_description(std::string(module_name) + 
                                     ": dump selected contents of given input file\nSupported options"));
    // declare supported options
    po::options_description opt;
    //opt.add_options()
    //    ("", "")
    //;
    desc.add(opt);
}

std::string Dump::call(const po::variables_map &vm, const po::options_description &desc) {
    std::vector<nix::File> files; // opened nix files
    std::stringstream out;
    nix::File tmp_file;
    
    // --help
    if (vm.count(HELP_OPTION)) {
        out << desc << std::endl;
        return out.str();
    }
    // --input-file
    if (vm.count(INPFILE_OPTION)) {
        // open all files
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
        // loop through entities in all files
        for(auto &file : files) {
            yaml << file;
            out << yaml.str();
            /*// Blocks
            auto blcks = nix_file.blocks();
            for(auto &block : blcks) {
                // DataArrays
                auto data_arrays = block.dataArrays();
                for(auto &data_array : data_arrays) {
                    // Dimensions
                    auto dims = data_array.dimensions();
                    std::vector<nix::RangeDimension> range_dims;
                    std::vector<nix::SetDimension> set_dims;
                    std::vector<nix::SampledDimension> sample_dims;
                    for(auto &dim : dims) {
                        if(dim.dimensionType() == DimensionType::Range) {
                            auto d = dim.asRangeDimension();
                            range_dims.push_back(d);
                        }
                        if(dim.dimensionType() == DimensionType::Set) {
                            auto d = dim.asSetDimension();
                            set_dims.push_back(d);
                        }
                        if(dim.dimensionType() == DimensionType::Sample) {
                            auto d = dim.asSampledDimension();
                            sample_dims.push_back(d);
                        }
                    }
                }
                // DataTags
                auto data_tags = block.dataTags();
                for(auto &data_tag : data_tags) {
                    // Features
                    auto features = data_tag.features();
                    for(auto &feature : features) {
                    }
                }
                // SimpleTags
                auto simple_tags = block.simpleTags();
                for(auto &simple_tag : simple_tags) {
                    // Features
                    auto features = simple_tag.features();
                    for(auto &feature : features) {
                    }
                }
                // Sources
                auto sources = block.findSources();
                for(auto &source : sources) {
                }
            }
            // Sections
            auto sections = nix_file.findSections();
            for(auto &section : sections) {
                // Properties
                auto props = section.properties();
                for(auto &prop : props) {
                }
            }*/
        } // for: files
    } // if: INPFILE_OPTION
    else {
        throw NoInputFile();
    }
    
    return out.str();
}

} // namespace module
} // namespace cli
