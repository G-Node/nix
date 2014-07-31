// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef CLI_DUMP_H
#define CLI_DUMP_H

#include <nix/base/types.hpp>
#include <nix/base/Entity.hpp>
#include <nix/base/NamedEntity.hpp>
#include <nix/base/EntityWithMetadata.hpp>
#include <nix/base/EntityWithSources.hpp>
#include <nix.hpp>

#include <Cli.hpp>
#include <modules/IModule.hpp>

#include <iostream>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

namespace cli {
namespace module {
    
class yamlstream {
    static const char* indent_str;
    static const char* scalar_start;
    static const char* scalar_end;
    static const char* sequ_start;
    static const char* sequ_end;
    static const char* item_str;
    
    size_t level;
    std::stringstream &sstream;
    
    /**
     * @brief apply indentation on sstream if last char is "\n"
     *
     * Apply indentation on sstream if last char is "\n"
     *
     * @return void
     */
    void indent_if() {
        // if endl
        if(!strcmp(&*sstream.str().rbegin(), "\n")) {
            (*this)[level];
        }
    }
    
    /**
     * @brief apply indentation on sstream if last char is "\n"
     *
     * Apply indentation on sstream if last char is "\n"
     *
     * @return void
     */
    void endl_if() {
        // if _not_ endl
        if(strcmp(&*sstream.str().rbegin(), "\n")) {
            sstream << "\n";
        }
    }
    
    /**
     * @brief return item_str if and only if level is not zero
     *
     * Return the item string if and only if we are not outputting to
     * the base level, but to some sub level
     *
     * @return string item_str
     */
    std::string item() {
        return std::string(level ? item_str : "");
    }
    
    /**
     * @brief start yaml sequence & increase indent level
     *
     * Put the defined sequ_start into the stream and increase
     * indentation level.
     *
     * @return self
     */
    yamlstream& operator++() {
        sstream << sequ_start;
        level++;
        return *this;
    };
    
    /**
     * @brief start yaml sequence & increase indent level
     *
     * Put the defined sequ_start into the stream and increase
     * indentation level.
     *
     * @return self
     */
    yamlstream operator++(int) {
        yamlstream tmp(*this);
        operator++(); // prefix-increment this instance
        return tmp;   // return value before increment
    };

    /**
     * @brief end yaml sequence & decrease indent level
     *
     * Put the defined sequ_end into the stream and decrease 
     * indentation level.
     *
     * @return self
     */
    yamlstream operator--() {
        endl_if();
        level--;
        return *this;
    };

    /**
     * @brief end yaml sequence & decrease indent level
     *
     * Put the defined sequ_end into the stream and decrease 
     * indentation level.
     *
     * @return self
     */
    yamlstream operator--(int) {
        yamlstream tmp(*this);
        operator--(); // prefix-decrement this instance
        return tmp;   // return value before increment
    };

    /**
     * @brief put yaml indentation into stream
     *
     * Put n_indent times the defined indent into the stream
     *
     * @return self
     */
    yamlstream& operator[](const int &n_indent) {
        endl_if();
        for(int i = 0; i < n_indent; i++) {
            sstream << indent_str;
        }
        return *this;
    };
    
    /**
     * @brief convert unix epoch time to local time string
     *
     * Convert unix epoch time to local time string
     *
     * @return string with the given time as local time
     */
    std::string t(const time_t &tm) {
        char* a = asctime(localtime(&tm));       // get time as char*
        a[std::strlen(a)-1] = a[std::strlen(a)]; // remove "\n"
        return std::string(a);
    };    
    
public:
    /**
     * @brief default ctor
     *
     * The default constructor.
     */
    yamlstream(std::stringstream &sstream) : level(0), sstream(sstream) {};

    /**
     * @brief return stringstream as string
     *
     * Return string from stringstream
     *
     * @return string string content of the stream
     */
    std::string str() {
        return sstream.str();
    };

    /**
     * @brief default output into stringstream
     *
     * Use the default stringstream output.
     *
     * @return self
     */
    template<typename T>
    yamlstream& operator<<(const T &t) {
        indent_if();
        sstream << t;
        return *this;
    };
    template<typename T>
    yamlstream& operator<<(const std::vector<T> &t) {
        indent_if();
        if(t.size()) {
            sstream << "{";
            for(auto &el : t) {
                sstream << el << ((*t.rbegin()) != el ? ", " : "");            
            }
            sstream << "}";
        }
        return *this;
    };
    yamlstream& operator<<(const nix::NDSize &t) {
        indent_if();
        std::vector<double> extent;
        for(size_t i = 0; i < t.size(); i++) {
            extent.push_back(t[i]);
        }
        (*this) << extent;
        return *this;
    };
    template<typename T>
    yamlstream& operator<<(const boost::optional<T> &t) {
        indent_if();
        auto opt = nix::util::deRef(t);
        sstream << opt;
        return *this;
    };
	yamlstream& operator<<(std::stringstream& (*ps)(std::stringstream&))
	{
        indent_if();
		sstream << ps;
		return *this;
	};
    
    template<typename T>
    yamlstream& operator<<(const nix::base::Entity<T> &entity) {
        (*this)
        << item() << "id" << scalar_start << entity.id() << scalar_end
        << item() << "createdAt" << scalar_start << t(entity.createdAt()) << scalar_end
        << item() << "updatedAt" << scalar_start << t(entity.updatedAt()) << scalar_end;
        
        return *this;
    };

    template<typename T>
    yamlstream& operator<<(const nix::base::NamedEntity<T> &namedEntity) {
        (*this)
        << static_cast<nix::base::Entity<T>>(namedEntity)
        << item() << "name" << scalar_start << namedEntity.name() << scalar_end
        << item() << "type" << scalar_start << namedEntity.type() << scalar_end
        << item() << "definition" << scalar_start << namedEntity.definition() << scalar_end;
        
        return *this;
    };

    template<typename T>
    yamlstream& operator<<(const nix::base::EntityWithMetadata<T> &entityWithMetadata) {
        (*this)
        << static_cast<nix::base::NamedEntity<T>>(entityWithMetadata)
        << item() << "metadata"; ++(*this) << entityWithMetadata.metadata(); --(*this);
        
        return *this;
    };
    
    template<typename T>
    yamlstream& operator<<(const nix::base::EntityWithSources<T> &entityWithSources) {
        (*this)
        << static_cast<nix::base::EntityWithMetadata<T>>(entityWithSources)
        << item() << "sourceCount" << scalar_start << entityWithSources.sourceCount() << scalar_end;
        // NOTE: dont output sources as those are handled by derived frontend entity
        
        return *this;
    };
    
    yamlstream& operator<<(const nix::Property &property) {
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
    };
    
    yamlstream& operator<<(const nix::Section &section) {
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
    };

    yamlstream& operator<<(const nix::SetDimension &dim) {
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
    };

    yamlstream& operator<<(const nix::SampledDimension &dim) {
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
    };

    yamlstream& operator<<(const nix::RangeDimension &dim) {
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
    };
    
    yamlstream& operator<<(const nix::Dimension &dim) {
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
    };
    
    yamlstream& operator<<(const nix::DataArray &data_array) {
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
    };

    yamlstream& operator<<(const nix::Feature &feature) {
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
    };
    
    yamlstream& operator<<(const nix::SimpleTag &simple_tag) {
        if(!simple_tag) {
            return *this; // unset entity protection
        }
        
        (*this)[level] << item() << "simple_tag " << simple_tag.id();
        ++(*this)
            << static_cast<nix::base::EntityWithSources<nix::base::ISimpleTag>>(simple_tag)
            << item() << "units" << scalar_start << simple_tag.units() << scalar_end
            << item() << "featureCount" << scalar_start << simple_tag.featureCount() << scalar_end
            << item() << "referenceCount" << scalar_start << simple_tag.referenceCount() << scalar_end
            << item() << "extent" << scalar_start << simple_tag.extent() << scalar_end
            << item() << "position" << scalar_start << simple_tag.position() << scalar_end;
            // References
            auto refs = simple_tag.references();
            for(auto &ref : refs) {
                *this << ref;
            }
            // Features
            auto features = simple_tag.features();
            for(auto &feature : features) {
                *this << feature;
            }
        --(*this);
        return *this;
    };
    
    yamlstream& operator<<(const nix::DataTag &data_tag) {
        if(!data_tag) {
            return *this; // unset entity protection
        }
        
        (*this)[level] << item() << "data_tag " << data_tag.id();
        ++(*this)
            << static_cast<nix::base::EntityWithSources<nix::base::IDataTag>>(data_tag)
            << item() << "units" << scalar_start << data_tag.units() << scalar_end
            << item() << "featureCount" << scalar_start << data_tag.featureCount() << scalar_end
            << item() << "referenceCount" << scalar_start << data_tag.referenceCount() << scalar_end
            << item() << "extents"; ++(*this) << data_tag.extents(); --(*this)
            << item() << "positions"; ++(*this) << data_tag.positions(); --(*this);
            // References
            auto refs = data_tag.references();
            for(auto &ref : refs) {
                *this << ref;
            }
            // Features
            auto features = data_tag.features();
            for(auto &feature : features) {
                *this << feature;
            }
        --(*this);
        return *this;
    };
    
    yamlstream& operator<<(const nix::Block &block) {
        if(!block) {
            return *this; // unset entity protection
        }
        
        (*this)[level] << item() << "block " << block.id();
        ++(*this)
            << static_cast<nix::base::EntityWithMetadata<nix::base::IBlock>>(block)
            << item() << "sourceCount" << scalar_start << block.sourceCount() << scalar_end
            << item() << "simpleTagCount" << scalar_start << block.simpleTagCount() << scalar_end
            << item() << "dataTagCount" << scalar_start << block.dataTagCount() << scalar_end
            << item() << "dataArrayCount" << scalar_start << block.dataArrayCount() << scalar_end;
            // DataArrays
            auto data_arrays = block.dataArrays();
            for(auto &data_array : data_arrays) {
                *this << data_array;
            }
            // DataTags
            auto data_tags = block.dataTags();
            for(auto &data_tag : data_tags) {
                *this << data_tag;
            }
            // SimpleTags
            auto simple_tags = block.simpleTags();
            for(auto &simple_tag : simple_tags) {
                *this << simple_tag;
            }
            // Sections
            auto sources = block.sources();
            for(auto &source : sources) {
                *this << source;            
            }
        --(*this);
        return *this;
    };

    yamlstream& operator<<(const nix::File &file) {
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
    };
    
};

class Dump : virtual public IModule {
    
    yamlstream yaml;
    std::stringstream sstream;
    
public:
    Dump() : yaml(yamlstream(sstream)) {}

    static const char* module_name;

    std::string name() const {
        return std::string(module_name);
    }

    void load(po::options_description &desc) const;

    std::string call(const po::variables_map &vm, const po::options_description &desc);

};

} // namespace module
} // namespace cli

#endif
