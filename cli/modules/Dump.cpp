// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

#include <algorithm>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <modules/Dump.hpp>
#include <limits>
#include <cstddef>

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
const char* plot_script::plot_file = "dump_plot.gnu";

void yamlstream::indent_if() {
    // if endl
    if (!strcmp(&*sstream.str().rbegin(), "\n")) {
        (*this)[level];
    }
}

void yamlstream::endl_if() {
    // if _not_ endl
    if (strcmp(&*sstream.str().rbegin(), "\n")) {
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

yamlstream& yamlstream::operator[](const size_t n_indent) {
    endl_if();
    for (size_t i = 0; i < n_indent; i++) {
        sstream << indent_str;
    }
    return *this;
}

std::string yamlstream::t(const time_t &tm) {
    char tbuff[100];
    std::tm t_local;

#ifndef _MSC_VER
    t_local = *std::localtime(&tm);
#else
    errno_t err = localtime_s(&t_local, &tm);
    if (err) {
	    return "NA";
    }
#endif

    size_t res = strftime(tbuff, sizeof(tbuff), "%c", &t_local);
    if (res == 0) {
        return "NA";
    }

    return std::string(tbuff);
}

std::string yamlstream::str() {
    return sstream.str();
}

yamlstream& yamlstream::operator<<(const nix::NDSize &t)
{
    indent_if();
    std::vector<double> extent(t.size());
    std::transform(t.begin(), t.end(), extent.begin(), [](nix::ndsize_t val){
        return static_cast<double>(val);
    });
    (*this) << extent;
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::Value &value) {
    // get value
    std::stringstream val;
    switch(value.type()) {
        case nix::DataType::Bool:
            val << value.get<bool>();
            break;
        case nix::DataType::String:
            val << value.get<std::string>();
            break;
        case nix::DataType::Int32:
            val << value.get<int32_t>();
            break;
        case nix::DataType::UInt32:
            val << value.get<uint32_t>();
            break;
        case nix::DataType::Int64:
            val << value.get<int64_t>();
            break;
        case nix::DataType::UInt64:
            val << value.get<uint64_t>();
            break;
        case nix::DataType::Double:
            val << value.get<double>();
            break;
        case nix::DataType::Nothing:
            break;
        default:
            val << "UNKNOWN TYPE";
            break;
    }

    (*this)[level] << item() << "value " << value.type() << scalar_start << val.str() << scalar_end;

    return *this;
}

yamlstream& yamlstream::operator<<(const nix::Property &property) {
    if (!property) {
        return *this; // unset entity protection
    }

    (*this)[level] << item() << "property &" << property.id();
    ++(*this)
        << static_cast<nix::base::Entity<nix::base::IProperty>>(property)
        << "dataType" << scalar_start << property.dataType() << scalar_end
        << "definition" << scalar_start << property.definition() << scalar_end
        << "name" << scalar_start << property.name() << scalar_end
        << "unit" << scalar_start << property.unit() << scalar_end
        << "uncertainty" << scalar_start << property.uncertainty() << scalar_end
        << "valueCount" << scalar_start << property.valueCount() << scalar_end;

        // Values
        *this << "values";
        ++(*this);
            auto values = property.values();
            for (auto &value : values) {
                *this << value;
            }
        --(*this);
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::Source &source) {
    if (!source) {
        return *this; // unset entity protection
    }

    (*this)[level] << item() << "source &" << source.id();
    ++(*this)
        << static_cast<nix::base::EntityWithMetadata<nix::base::ISource>>(source)
        << "sourceCount" << scalar_start << source.sourceCount() << scalar_end;

        // Sources
        *this << "sources";
        ++(*this);
            auto sources = source.sources();
            for (auto &s : sources) {
                *this << s;
            }
        --(*this);
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::Section &section) {
    if (!section) {
        return *this; // unset entity protection
    }

    (*this)[level] << item() << "section &" << section.id();
    ++(*this)
        << static_cast<nix::base::NamedEntity<nix::base::ISection>>(section)
        << "propertyCount" << scalar_start << section.propertyCount() << scalar_end
        << "sectionCount" << scalar_start << section.sectionCount() << scalar_end
        << "repository" << scalar_start << section.repository() << scalar_end
        << "link"; ++(*this) << section.link(); --(*this);

        // Properties
        *this << "properties";
        ++(*this);
            auto properties = section.properties();
            for (auto &property : properties) {
                *this << property;
            }
        --(*this);
        // Sections
        *this << "sections";
        ++(*this);
            auto sections = section.sections();
            for (auto &s : sections) {
                *this << s;
            }
        --(*this);
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::SetDimension &dim) {
    if (!dim) {
        return *this; // unset entity protection
    }

    (*this)[level] << item() << "dimension " << dim.index();
    ++(*this)
        << "index" << scalar_start << dim.index() << scalar_end
        << "dimensionType" << scalar_start << dim.dimensionType() << scalar_end
        << "labels" << scalar_start << dim.labels() << scalar_end;
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::SampledDimension &dim) {
    if (!dim) {
        return *this; // unset entity protection
    }

    (*this)[level] << item() << "dimension " << dim.index();
    ++(*this)
        << "index" << scalar_start << dim.index() << scalar_end
        << "dimensionType" << scalar_start << dim.dimensionType() << scalar_end
        << "label" << scalar_start << dim.label() << scalar_end
        << "offset" << scalar_start << dim.offset() << scalar_end
        << "samplingInterval" << scalar_start << dim.samplingInterval() << scalar_end
        << "unit" << scalar_start << dim.unit() << scalar_end;
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::RangeDimension &dim) {
    if (!dim) {
        return *this; // unset entity protection
    }

    (*this)[level] << item() << "dimension " << dim.index();
    ++(*this)
        << "index" << scalar_start << dim.index() << scalar_end
        << "dimensionType" << scalar_start << dim.dimensionType() << scalar_end
        << "label" << scalar_start << dim.label() << scalar_end
        << "ticks" << scalar_start << dim.ticks() << scalar_end
        << "unit" << scalar_start << dim.unit() << scalar_end;
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::Dimension &dim) {
    if (!dim) {
        return *this; // unset entity protection
    }

    if (dim.dimensionType() == nix::DimensionType::Range) {
        (*this) << dim.asRangeDimension();
    }
    if (dim.dimensionType() == nix::DimensionType::Set) {
        (*this) << dim.asSetDimension();
    }
    if (dim.dimensionType() == nix::DimensionType::Sample) {
        (*this) << dim.asSampledDimension();
    }

    return *this;
}

yamlstream& yamlstream::operator<<(const nix::DataArray &data_array) {
    if (!data_array) {
        return *this; // unset entity protection
    }

    (*this)[level] << item() << "data_array &" << data_array.id();
    ++(*this)
        << static_cast<nix::base::EntityWithSources<nix::base::IDataArray>>(data_array)
        << "dataType" << scalar_start << data_array.dataType() << scalar_end
        << "dataExtent" << scalar_start << data_array.dataExtent() << scalar_end
        << "expansionOrigin" << scalar_start << data_array.expansionOrigin() << scalar_end
        << "polynomCoefficients" << scalar_start << data_array.polynomCoefficients() << scalar_end
        << "label" << scalar_start << data_array.label() << scalar_end
        << "unit" << scalar_start << data_array.unit() << scalar_end
        << "dimensionCount" << scalar_start << data_array.dimensionCount() << scalar_end;
        // Dimensions
        *this << "dimensions";
        ++(*this);
            auto dims = data_array.dimensions();
            for (auto &dim : dims) {
                *this << dim;
            }
        --(*this);
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::Feature &feature) {
    if (!feature) {
        return *this; // unset entity protection
    }

    (*this)[level] << item() << "feature &" << feature.id();
    ++(*this)
        << static_cast<nix::base::Entity<nix::base::IFeature>>(feature)
        << "linkType" << scalar_start << feature.linkType() << scalar_end
        << "data" << sequ_start << feature.data();
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::Tag &tag) {
    if (!tag) {
        return *this; // unset entity protection
    }

    (*this)[level] << item() << "tag &" << tag.id();
    ++(*this)
        << static_cast<nix::base::EntityWithSources<nix::base::ITag>>(tag)
        << "units" << scalar_start << tag.units() << scalar_end
        << "featureCount" << scalar_start << tag.featureCount() << scalar_end
        << "referenceCount" << scalar_start << tag.referenceCount() << scalar_end
        << "extent" << scalar_start << tag.extent() << scalar_end
        << "position" << scalar_start << tag.position() << scalar_end;
        // References
        *this << "references";
        ++(*this);
            auto refs = tag.references();
            for (auto &ref : refs) {
                *this << ref;
            }
        --(*this);
        // Features
        *this << "features";
        ++(*this);
            auto features = tag.features();
            for (auto &feature : features) {
                *this << feature;
            }
        --(*this);
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::MultiTag &multi_tag) {
    if (!multi_tag) {
        return *this; // unset entity protection
    }

    (*this)[level] << item() << "multi_tag &" << multi_tag.id();
    ++(*this)
        << static_cast<nix::base::EntityWithSources<nix::base::IMultiTag>>(multi_tag)
        << "units" << scalar_start << multi_tag.units() << scalar_end
        << "featureCount" << scalar_start << multi_tag.featureCount() << scalar_end
        << "referenceCount" << scalar_start << multi_tag.referenceCount() << scalar_end
        << "extents"; ++(*this) << multi_tag.extents(); --(*this)
        << "positions"; ++(*this) << multi_tag.positions(); --(*this);
        // References
        *this << "references";
        ++(*this);
            auto refs = multi_tag.references();
            for (auto &ref : refs) {
                *this << ref;
            }
        --(*this);
        // Features
        *this << "features";
        ++(*this);
            auto features = multi_tag.features();
            for (auto &feature : features) {
                *this << feature;
            }
        --(*this);
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::Block &block) {
    if (!block) {
        return *this; // unset entity protection
    }

    (*this)[level] << item() << "block &" << block.id();
    ++(*this)
        << static_cast<nix::base::EntityWithMetadata<nix::base::IBlock>>(block)
        << "sourceCount" << scalar_start << block.sourceCount() << scalar_end
        << "tagCount" << scalar_start << block.tagCount() << scalar_end
        << "multiTagCount" << scalar_start << block.multiTagCount() << scalar_end
        << "dataArrayCount" << scalar_start << block.dataArrayCount() << scalar_end;
        // DataArrays
        *this << "data_arrays";
        ++(*this);
            auto data_arrays = block.dataArrays();
            for (auto &data_array : data_arrays) {
                *this << data_array;
            }
        --(*this);
        // MultiTags
        *this << "multi_tags";
        ++(*this);
            auto multi_tags = block.multiTags();
            for (auto &multi_tag : multi_tags) {
                *this << multi_tag;
            }
        --(*this);
        // Tags
        *this << "tags";
        ++(*this);
            auto tags = block.tags();
            for (auto &tag : tags) {
                *this << tag;
            }
        --(*this);
        // Sources
        *this << "sources";
        ++(*this);
            auto sources = block.sources();
            for (auto &source : sources) {
                *this << source;
            }
        --(*this);
    --(*this);
    return *this;
}

yamlstream& yamlstream::operator<<(const nix::File &file) {
    if (!file) {
        return *this; // unset entity protection
    }

    (*this)[level] << item() << "file &" << file.location();
    ++(*this)
        << "location" << scalar_start << file.location() << scalar_end
        << "createdAt" << scalar_start << t(file.createdAt()) << scalar_end
        << "updatedAt" << scalar_start << t(file.updatedAt()) << scalar_end
        << "format" << scalar_start << file.format() << scalar_end
        << "version" << scalar_start << file.version() << scalar_end
        << "isOpen" << scalar_start << file.isOpen() << scalar_end
        << "blockCount" << scalar_start << file.blockCount() << scalar_end
        << "sectionCount" << scalar_start << file.sectionCount() << scalar_end;
        // Blocks
        *this << "blocks";
        ++(*this);
            auto blocks = file.blocks();
            for (auto &block : blocks) {
                *this << block;
            }
        --(*this);
        // Sections
        *this << "sections";
        ++(*this);
            auto sections = file.sections();
            for (auto &section : sections) {
                *this << section;
            }
        --(*this);
    --(*this);

    return *this;
}


void Dump::load(po::options_description &desc) const {
    // declare purpose
    desc.add(po::options_description("nix-tool " + std::string(module_name) + ":\n\n\t" +
                                     "Dump data-contents of a given nix file as yaml to std out.\n\nSupported options"));
    // declare supported options
    po::options_description opt;
    opt.add_options()
        (DATA_OPTION, "dump data from all 2D DataArrays")
        (PLOT_OPTION, ("dump & plot (only) data from all 2D DataArrays (linux only, invokes --" + std::string(DATA_OPTION) + ")").c_str())
    ;
    desc.add(opt);
}

std::string Dump::call(const po::variables_map &vm, const po::options_description &desc) {
    std::vector<nix::File> files; // opened nix files
    std::stringstream out;
    std::ofstream fout;
    nix::File tmp_file;
    std::string file_name;
    double A_min = std::numeric_limits<double>::max();
    double A_max = std::numeric_limits<double>::min();
    typedef boost::multi_array<double, 2> array_type;
    typedef array_type::index index;

    // --help
    if (vm.count(HELP_OPTION)) {
        po::options_description temp;
        load(temp);
        out << temp << std::endl;
        return out.str();
    }
    // --input-file
    if (vm.count(INPFILE_OPTION)) {
        // open all files
        for (auto &file_path : vm[INPFILE_OPTION].as< std::vector<std::string> >()) {
            // file exists?
            if (!boost::filesystem::exists(file_path)) {
                throw FileNotFound(file_path);
            }
            // try to open!
            tmp_file = nix::File::open(file_path, nix::FileMode::ReadOnly);
            // file opened?
            if (!tmp_file.isOpen()) {
                throw FileNotOpen(file_path);
            }
            // save it!
            files.push_back(tmp_file); // ReadOnly, ReadWrite, Overwrite
        }
        // loop through entities in all files
        for (auto &file : files) {
            if ( ! (vm.count(DATA_OPTION) || vm.count(PLOT_OPTION)) ) {
                yaml << file;
                out << yaml.str();
            }
            else {
                // loop through all data_arrays
                auto blcks = file.blocks();
                for (auto &block : blcks) {
                    auto data_arrays = block.dataArrays();
                    for (auto &data_array : data_arrays) {
                        // if we have a 2D data_array, output data & plot script
                        if (data_array.dataExtent().size() == 2) {
                            file_name = "data_array_" + data_array.id();
                            fout.open(file_name + ".txt");
                            index dim1 = static_cast<index>(data_array.dataExtent()[0]);
                            index dim2 = static_cast<index>(data_array.dataExtent()[1]);
                            array_type A(boost::extents[ dim1 ]
                                                       [ dim2 ]);
                            data_array.getData(A);
                            // loop through data_array values
                            for (index i = 0; i < dim1; i++) {
                                for (index j = 0; j < dim2; j++) {
                                    fout << A[i][j] << ((j != dim2-1) ? " " : "");
                                    if (A[i][j] < A_min) A_min = A[i][j];
                                    if (A[i][j] > A_max) A_max = A[i][j];
                                }
                                fout << ((i != dim1-1) ? "\n" : "");
                            }
                            fout.close();

                            #ifndef _WIN32
                            if (vm.count(PLOT_OPTION)) {
                                std::cout << "press ctrl+c for next plot" << std::endl;
                                plot_script script(A_min, A_max, static_cast<size_t>(dim1), static_cast<size_t>(dim2), file_name + ".txt");
                                fout.open(file_name + ".gnu");
                                fout << script.str();
                                fout.close();
                                std::system(("chmod 755 " + file_name + ".gnu").c_str());
                                std::system(("./" + file_name + ".gnu").c_str());
                            }
                            #endif
                        } // if data_array.dataExtent().size() == 2
                    } // for data_arrays
                } // for blcks
            } // if vm.count(DATA_OPTION) || vm.count(PLOT_OPTION)
        } // for: files
    } // if: INPFILE_OPTION
    else {
        throw NoInputFile();
    }

    return out.str();
}

} // namespace module
} // namespace cli
