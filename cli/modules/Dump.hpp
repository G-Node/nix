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

#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <regex>
#include <stdlib.h>
#include <cmath>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

namespace cli {
namespace module {

const char *const DATA_OPTION = "data";
const char *const PLOT_OPTION = "plot";

class plot_script {
private:
    char* script;

public:
    template<typename T1, typename T2>
    plot_script(T1 minval, T2 maxval, size_t nx, size_t ny, std::string file_path) {
        std::string str_script = (std::string() + 
            "#!/usr/bin/gnuplot" + "\n\n" +
            "set terminal x11" + "\n" +
            "set title \"2D data plot\"" + "\n" +
            "unset key" + "\n" +
            "set tic scale 0" + "\n\n" +
            "# Color runs from white to green" + "\n" +
            "set palette rgbformula -7,-7,2" + "\n" +
            "set cbrange [MINVAL:MAXVAL]" + "\n" +
            "set cblabel \"Score\"" + "\n" +
            "unset cbtics" + "\n\n" +
            "set xrange [-0.5:NX.5]" + "\n"
            "set yrange [-0.5:NY.5]" + "\n\n"
            "set view map" + "\n"
            "splot 'FILE' matrix with image" + "\n"
            "pause -1" + "\n");

        // round min & max to precision of two digits beyond magnitude of their difference,
        // if their difference is < 2. Round them to ints otherwise.
        double multiplier = abs( round(1 / (maxval - minval)) ) * 100;
        minval = (multiplier != 0) ? round(minval * multiplier) / multiplier : round(minval);
        maxval = (multiplier != 0) ? round(maxval * multiplier) / multiplier : round(maxval);
        str_script.replace(str_script.find("MINVAL"), std::string("MINVAL").length(), nix::util::numToStr(minval));
        str_script.replace(str_script.find("MAXVAL"), std::string("MAXVAL").length(), nix::util::numToStr(maxval));
        str_script.replace(str_script.find("NX"), std::string("NX").length(), nix::util::numToStr(nx));
        str_script.replace(str_script.find("NY"), std::string("NY").length(), nix::util::numToStr(ny));
        str_script.replace(str_script.find("FILE"), std::string("FILE").length(), file_path);
        
        script = (char*) malloc (str_script.size());
        std::strcpy(script, str_script.c_str());
    }
    
    std::string str() {
        return std::string(script);
    }
    
    ~plot_script() {
        free(script);
    }
};

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
    void indent_if();
    
    /**
     * @brief apply indentation on sstream if last char is "\n"
     *
     * Apply indentation on sstream if last char is "\n"
     *
     * @return void
     */
    void endl_if();
    
    /**
     * @brief return item_str if and only if level is not zero
     *
     * Return the item string if and only if we are not outputting to
     * the base level, but to some sub level
     *
     * @return string item_str
     */
    std::string item();
    
    /**
     * @brief start yaml sequence & increase indent level
     *
     * Put the defined sequ_start into the stream and increase
     * indentation level.
     *
     * @return self
     */
    yamlstream& operator++();
    
    /**
     * @brief start yaml sequence & increase indent level
     *
     * Put the defined sequ_start into the stream and increase
     * indentation level.
     *
     * @return self
     */
    yamlstream operator++(int);

    /**
     * @brief end yaml sequence & decrease indent level
     *
     * Put the defined sequ_end into the stream and decrease 
     * indentation level.
     *
     * @return self
     */
    yamlstream operator--();

    /**
     * @brief end yaml sequence & decrease indent level
     *
     * Put the defined sequ_end into the stream and decrease 
     * indentation level.
     *
     * @return self
     */
    yamlstream operator--(int);

    /**
     * @brief put yaml indentation into stream
     *
     * Put n_indent times the defined indent into the stream
     *
     * @return self
     */
    yamlstream& operator[](const int &n_indent);
    
    /**
     * @brief convert unix epoch time to local time string
     *
     * Convert unix epoch time to local time string
     *
     * @return string with the given time as local time
     */
    std::string t(const time_t &tm);
    
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
    std::string str();

    /**
     * @brief default output into stringstream
     *
     * Use the default stringstream output.
     *
     * @param t parameter of any given type T
     * @return self
     */
    template<typename T>
    yamlstream& operator<<(const T &t) {
        indent_if();
        sstream << t;
        return *this;
    }
    
    /**
     * @brief vector output into stringstream
     *
     * Output vector elements in inline yaml sequence style.
     *
     * @param t vector of any given type T
     * @return self
     */
    template<typename T>
    yamlstream& operator<<(const std::vector<T> &t) {
        indent_if();
        if(t.size()) {
            sstream << "[";
            for(auto &el : t) {
                sstream << el << ((*t.rbegin()) != el ? ", " : "");            
            }
            sstream << "]";
        }
        return *this;
    }
    
    /**
     * @brief NDSize output into stringstream
     *
     * Build vector of sizes and output them as vector.
     *
     * @param t NDSize class
     * @return self
     */
    yamlstream& operator<<(const nix::NDSize &t) {
        indent_if();
        std::vector<double> extent;
        for(size_t i = 0; i < t.size(); i++) {
            extent.push_back(t[i]);
        }
        (*this) << extent;
        return *this;
    }
    
    /**
     * @brief boost::optional output into stringstream
     *
     * De-referene boost::optional if and only if it is set and output
     * content (or empty string if not set) to stream.
     *
     * @param t boost::optional
     * @return self
     */
    template<typename T>
    yamlstream& operator<<(const boost::optional<T> &t) {
        indent_if();
        auto opt = nix::util::deRef(t);
        sstream << opt;
        return *this;
    }
    
    /**
     * @brief pointer to stringstream output into stringstream
     *
     * Output via pointer to stringstream.
     *
     * @param ps pointer to stringstream
     * @return self
     */
	yamlstream& operator<<(std::stringstream& (*ps)(std::stringstream&))
	{
        indent_if();
		sstream << ps;
		return *this;
	}
    
    /**
     * @brief Entity output into stringstream
     *
     * Output base Entity to stringstream.
     *
     * @param entity nix base Entity
     * @return self
     */
    template<typename T>
    yamlstream& operator<<(const nix::base::Entity<T> &entity) {
        (*this)
        << "id" << scalar_start << "" << entity.id() << scalar_end
        << "createdAt" << scalar_start << t(entity.createdAt()) << scalar_end
        << "updatedAt" << scalar_start << t(entity.updatedAt()) << scalar_end;
        
        return *this;
    }

    /**
     * @brief NamedEntity output into stringstream
     *
     * Output base NamedEntity to stringstream.
     *
     * @param entity nix base NamedEntity
     * @return self
     */
    template<typename T>
    yamlstream& operator<<(const nix::base::NamedEntity<T> &namedEntity) {
        (*this)
        << static_cast<nix::base::Entity<T>>(namedEntity)
        << "name" << scalar_start << namedEntity.name() << scalar_end
        << "type" << scalar_start << namedEntity.type() << scalar_end
        << "definition" << scalar_start << namedEntity.definition() << scalar_end;
        
        return *this;
    }

    /**
     * @brief EntityWithMetadata output into stringstream
     *
     * Output base EntityWithMetadata to stringstream.
     *
     * @param entity nix base EntityWithMetadata
     * @return self
     */
    template<typename T>
    yamlstream& operator<<(const nix::base::EntityWithMetadata<T> &entityWithMetadata) {
        (*this)
        << static_cast<nix::base::NamedEntity<T>>(entityWithMetadata)
        << "metadata"; ++(*this) << entityWithMetadata.metadata(); --(*this);
        
        return *this;
    }
    
    /**
     * @brief EntityWithSources output into stringstream
     *
     * Output base EntityWithSources to stringstream.
     *
     * @param entity nix base EntityWithSources
     * @return self
     */
    template<typename T>
    yamlstream& operator<<(const nix::base::EntityWithSources<T> &entityWithSources) {
        (*this)
        << static_cast<nix::base::EntityWithMetadata<T>>(entityWithSources)
        << "sourceCount" << scalar_start << entityWithSources.sourceCount() << scalar_end;
        // NOTE: dont output sources as those are handled by derived frontend entity
        
        return *this;
    }

    /**
     * @brief Value output into stringstream
     *
     * Output Value to stringstream.
     *
     * @param entity nix value
     * @return self
     */
    yamlstream& operator<<(const nix::Value &value);

    /**
     * @brief Property output into stringstream
     *
     * Output Property to stringstream.
     *
     * @param entity nix Property
     * @return self
     */
    yamlstream& operator<<(const nix::Property &property);
    
    /**
     * @brief Source output into stringstream
     *
     * Output Source to stringstream.
     *
     * @param entity nix Source
     * @return self
     */
    yamlstream& operator<<(const nix::Source &source);
    
    /**
     * @brief Section output into stringstream
     *
     * Output Section to stringstream.
     *
     * @param entity nix Section
     * @return self
     */
    yamlstream& operator<<(const nix::Section &section);

    /**
     * @brief SetDimension output into stringstream
     *
     * Output SetDimension to stringstream.
     *
     * @param entity nix SetDimension
     * @return self
     */
    yamlstream& operator<<(const nix::SetDimension &dim);

    /**
     * @brief SampledDimension output into stringstream
     *
     * Output SampledDimension to stringstream.
     *
     * @param entity nix SampledDimension
     * @return self
     */
    yamlstream& operator<<(const nix::SampledDimension &dim);

    /**
     * @brief RangeDimension output into stringstream
     *
     * Output RangeDimension to stringstream.
     *
     * @param entity nix RangeDimension
     * @return self
     */
    yamlstream& operator<<(const nix::RangeDimension &dim);

    /**
     * @brief Dimension output into stringstream
     *
     * Output Dimension to stringstream.
     *
     * @param entity nix Dimension
     * @return self
     */
    yamlstream& operator<<(const nix::Dimension &dim);

    /**
     * @brief DataArray output into stringstream
     *
     * Output DataArray to stringstream.
     *
     * @param entity nix DataArray
     * @return self
     */
    yamlstream& operator<<(const nix::DataArray &data_array);

    /**
     * @brief Feature output into stringstream
     *
     * Output Feature to stringstream.
     *
     * @param entity nix Feature
     * @return self
     */
    yamlstream& operator<<(const nix::Feature &feature);

    /**
     * @brief Tag output into stringstream
     *
     * Output Tag to stringstream.
     *
     * @param entity nix Tag
     * @return self
     */
    yamlstream& operator<<(const nix::Tag &tag);

    /**
     * @brief MultiTag output into stringstream
     *
     * Output MultiTag to stringstream.
     *
     * @param entity nix MultiTag
     * @return self
     */
    yamlstream& operator<<(const nix::MultiTag &multi_tag);

    /**
     * @brief Block output into stringstream
     *
     * Output Block to stringstream.
     *
     * @param entity nix Block
     * @return self
     */
    yamlstream& operator<<(const nix::Block &block);

    /**
     * @brief File output into stringstream
     *
     * Output File to stringstream.
     *
     * @param entity nix File
     * @return self
     */
    yamlstream& operator<<(const nix::File &file);
    
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
