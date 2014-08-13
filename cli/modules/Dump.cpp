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

void yamlstream::indent_if() {
    // if endl
    if(!strcmp(&*sstream.str().rbegin(), "\n")) {
        (*this)[level];
    }
}

void yamlstream::endl_if() {
    // if _not_ endl
    if(strcmp(&*sstream.str().rbegin(), "\n")) {
        sstream << "\n";
    }
}

std::string yamlstream::item() {
    return std::string(level ? item_str : "");
}

yamlstream& yamlstream::operator++() {
    sstream << sequ_start;
    level++;
    return *this;
}

yamlstream yamlstream::operator++(int) {
    yamlstream tmp(*this);
    operator++(); // prefix-increment this instance
    return tmp;   // return value before increment
}

yamlstream yamlstream::operator--() {
    endl_if();
    level--;
    return *this;
}

yamlstream yamlstream::operator--(int) {
    yamlstream tmp(*this);
    operator--(); // prefix-decrement this instance
    return tmp;   // return value before increment
}

yamlstream& yamlstream::operator[](const int &n_indent) {
    endl_if();
    for(int i = 0; i < n_indent; i++) {
        sstream << indent_str;
    }
    return *this;
}

std::string yamlstream::t(const time_t &tm) {
    char* a = asctime(localtime(&tm));       // get time as char*
    a[std::strlen(a)-1] = a[std::strlen(a)]; // remove "\n"
    return std::string(a);
}

std::string yamlstream::str() {
    return sstream.str();
}

yamlstream& yamlstream::operator<<(const nix::Property &property) {
    if(!property) {
        return *this; // unset entity protection
    }
        
    (*this)[level] << item() << "property " << property.id();
    ++(*this)
        << static_cast<nix::base::Entity<nix::base::IProperty>>(property)
        << item() << "dataType" << scalar_start << property.dataType() << scalar_end
        << item() << "definition" << scalar_start << property.definition() << scalar_end
        << item() << "mapping" << scalar_start << property.mapping() << scalar_end
        << item() << "name" << scalar_start << property.name() << scalar_end
        << item() << "unit" << scalar_start << property.unit() << scalar_end
        << item() << "valueCount" << scalar_start << property.valueCount() << scalar_end;

        // Values
        auto values = property.values();
        for(auto &value : values) {
            // value is handled by its own ostream, thus do context here
            *this << item() << "value" << scalar_start << value << scalar_end; 
        }
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::Section &section) {
    if(!section) {
        return *this; // unset entity protection
    }

    (*this)[level] << item() << "section " << section.id();
    ++(*this)
        << static_cast<nix::base::NamedEntity<nix::base::ISection>>(section)
        << item() << "propertyCount" << scalar_start << section.propertyCount() << scalar_end
        << item() << "sectionCount" << scalar_start << section.sectionCount() << scalar_end
        << item() << "mapping" << scalar_start << section.mapping() << scalar_end
        << item() << "repository" << scalar_start << section.repository() << scalar_end
        << item() << "link"; ++(*this) << section.link(); --(*this);

        // Properties
        auto properties = section.properties();
        for(auto &property : properties) {
            *this << property;
        }
        // Sections
        auto sections = section.sections();
        for(auto &section : sections) {
            *this << section;            
        }
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::SetDimension &dim) {
    if(!dim) {
        return *this; // unset entity protection
    }
    
    (*this)[level] << item() << "dimension " << dim.index();
    ++(*this)
        << item() << "index" << scalar_start << dim.index() << scalar_end
        << item() << "dimensionType" << scalar_start << dim.dimensionType() << scalar_end
        << item() << "labels" << scalar_start << dim.labels() << scalar_end;
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::SampledDimension &dim) {
    if(!dim) {
        return *this; // unset entity protection
    }
    
    (*this)[level] << item() << "dimension " << dim.index();
    ++(*this)
        << item() << "index" << scalar_start << dim.index() << scalar_end
        << item() << "dimensionType" << scalar_start << dim.dimensionType() << scalar_end
        << item() << "label" << scalar_start << dim.label() << scalar_end
        << item() << "offset" << scalar_start << dim.offset() << scalar_end
        << item() << "samplingInterval" << scalar_start << dim.samplingInterval() << scalar_end
        << item() << "unit" << scalar_start << dim.unit() << scalar_end;
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::RangeDimension &dim) {
    if(!dim) {
        return *this; // unset entity protection
    }
    
    (*this)[level] << item() << "dimension " << dim.index();
    ++(*this)
        << item() << "index" << scalar_start << dim.index() << scalar_end
        << item() << "dimensionType" << scalar_start << dim.dimensionType() << scalar_end
        << item() << "label" << scalar_start << dim.label() << scalar_end
        << item() << "ticks" << scalar_start << dim.ticks() << scalar_end
        << item() << "unit" << scalar_start << dim.unit() << scalar_end;
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::Dimension &dim) {
    if(!dim) {
        return *this; // unset entity protection
    }
    
    if(dim.dimensionType() == nix::DimensionType::Range) {
        (*this) << dim.asRangeDimension();
    }
    if(dim.dimensionType() == nix::DimensionType::Set) {
        (*this) << dim.asSetDimension();
    }
    if(dim.dimensionType() == nix::DimensionType::Sample) {
        (*this) << dim.asSampledDimension();
    }
    
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::DataArray &data_array) {
    if(!data_array) {
        return *this; // unset entity protection
    }
    
    (*this)[level] << item() << "data_array " << data_array.id();
    ++(*this)
        << static_cast<nix::base::EntityWithSources<nix::base::IDataArray>>(data_array)
        << item() << "dataType" << scalar_start << data_array.dataType() << scalar_end
        << item() << "dataExtent" << scalar_start << data_array.dataExtent() << scalar_end
        << item() << "expansionOrigin" << scalar_start << data_array.expansionOrigin() << scalar_end
        << item() << "polynomCoefficients" << scalar_start << data_array.polynomCoefficients() << scalar_end
        << item() << "label" << scalar_start << data_array.label() << scalar_end
        << item() << "unit" << scalar_start << data_array.unit() << scalar_end
        << item() << "dimensionCount" << scalar_start << data_array.dimensionCount() << scalar_end;
        // Dimensions
        auto dims = data_array.dimensions();
        for(auto &dim : dims) {
            *this << dim;
        }
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::Feature &feature) {
    if(!feature) {
        return *this; // unset entity protection
    }
    
    (*this)[level] << item() << "feature " << feature.id();
    ++(*this)
        << static_cast<nix::base::Entity<nix::base::IFeature>>(feature)
        << item() << "linkType" << scalar_start << feature.linkType() << scalar_end;
        ++(*this)
            << item() << "data" << sequ_start << feature.data();
        --(*this);
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::Tag &tag) {
    if(!tag) {
        return *this; // unset entity protection
    }
    
    (*this)[level] << item() << "tag " << tag.id();
    ++(*this)
        << static_cast<nix::base::EntityWithSources<nix::base::ITag>>(tag)
        << item() << "units" << scalar_start << tag.units() << scalar_end
        << item() << "featureCount" << scalar_start << tag.featureCount() << scalar_end
        << item() << "referenceCount" << scalar_start << tag.referenceCount() << scalar_end
        << item() << "extent" << scalar_start << tag.extent() << scalar_end
        << item() << "position" << scalar_start << tag.position() << scalar_end;
        // References
        auto refs = tag.references();
        for(auto &ref : refs) {
            *this << ref;
        }
        // Features
        auto features = tag.features();
        for(auto &feature : features) {
            *this << feature;
        }
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::MultiTag &multi_tag) {
    if(!multi_tag) {
        return *this; // unset entity protection
    }
    
    (*this)[level] << item() << "multi_tag " << multi_tag.id();
    ++(*this)
        << static_cast<nix::base::EntityWithSources<nix::base::IMultiTag>>(multi_tag)
        << item() << "units" << scalar_start << multi_tag.units() << scalar_end
        << item() << "featureCount" << scalar_start << multi_tag.featureCount() << scalar_end
        << item() << "referenceCount" << scalar_start << multi_tag.referenceCount() << scalar_end
        << item() << "extents"; ++(*this) << multi_tag.extents(); --(*this)
        << item() << "positions"; ++(*this) << multi_tag.positions(); --(*this);
        // References
        auto refs = multi_tag.references();
        for(auto &ref : refs) {
            *this << ref;
        }
        // Features
        auto features = multi_tag.features();
        for(auto &feature : features) {
            *this << feature;
        }
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::Block &block) {
    if(!block) {
        return *this; // unset entity protection
    }
    
    (*this)[level] << item() << "block " << block.id();
    ++(*this)
        << static_cast<nix::base::EntityWithMetadata<nix::base::IBlock>>(block)
        << item() << "sourceCount" << scalar_start << block.sourceCount() << scalar_end
        << item() << "simpleTagCount" << scalar_start << block.tagCount() << scalar_end
        << item() << "multiTagCount" << scalar_start << block.multiTagCount() << scalar_end
        << item() << "dataArrayCount" << scalar_start << block.dataArrayCount() << scalar_end;
        // DataArrays
        auto data_arrays = block.dataArrays();
        for(auto &data_array : data_arrays) {
            *this << data_array;
        }
        // MultiTags
        auto multi_tags = block.multiTags();
        for(auto &multi_tag : multi_tags) {
            *this << multi_tag;
        }
        // Tags
        auto tags = block.tags();
        for(auto &tag : tags) {
            *this << tag;
        }
        // Sections
        auto sources = block.sources();
        for(auto &source : sources) {
            *this << source;            
        }
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::File &file) {
    if(!file) {
        return *this; // unset entity protection
    }
    
    (*this)[level] << item() << "file " << file.location();
    ++(*this)
        << item() << "location" << scalar_start << file.location() << scalar_end
        << item() << "createdAt" << scalar_start << t(file.createdAt()) << scalar_end
        << item() << "updatedAt" << scalar_start << t(file.updatedAt()) << scalar_end
        << item() << "format" << scalar_start << file.format() << scalar_end
        << item() << "version" << scalar_start << file.version() << scalar_end
        << item() << "isOpen" << scalar_start << file.isOpen() << scalar_end
        << item() << "blockCount" << scalar_start << file.blockCount() << scalar_end
        << item() << "sectionCount" << scalar_start << file.sectionCount() << scalar_end;
        // Blocks
        auto blocks = file.blocks();
        for(auto &block : blocks) {
            *this << block;
        }
        // Sections
        auto sections = file.sections();
        for(auto &section : sections) {
            *this << section;            
        }
    --(*this);
    
    return *this;
}


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
                // MultiTags
                auto multi_tags = block.multiTags();
                for(auto &multi_tag : multi_tags) {
                    // Features
                    auto features = multi_tag.features();
                    for(auto &feature : features) {
                    }
                }
                // Tags
                auto tags = block.tags();
                for(auto &tag : tags) {
                    // Features
                    auto features = tag.features();
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
