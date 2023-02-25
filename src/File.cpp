// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/File.hpp>
#include <nix/util/util.hpp>
#include "hdf5/FileHDF5.hpp"

#ifdef ENABLE_FS_BACKEND
#include "fs/FileFS.hpp"
#endif

#include <nix/valid/validate.hpp>
#include <filesystem>

namespace fs = std::filesystem;

namespace nix {

File File::open(const std::string &name,
                FileMode mode,
                const std::string &impl,
                Compression compression,
                OpenFlags flags) {
    if (mode == nix::FileMode::ReadOnly && !fs::exists(fs::path{name})) {
        throw std::runtime_error("Cannot open non-existent file in ReadOnly mode!");
    }
    if (compression == Compression::Auto) {
         compression = Compression::None;
    }
    if (impl == "hdf5") {
        return File(std::make_shared<hdf5::FileHDF5>(name, mode, compression, flags));
    }
#ifdef  ENABLE_FS_BACKEND
    else if (impl == "file") {
         return File(std::make_shared<file::FileFS>(name, mode, compression));
    }
#endif
    else {
        throw std::runtime_error("Unknown implementation!");
    }
}


bool File::flush() {
    return backend()->flush();
}


Block File::createBlock(const std::string &name, const std::string &type) {
    util::checkEntityNameAndType(name, type);
    if (backend()->hasBlock(name)) {
        throw DuplicateName("Block with the given name already exists!");
    }
    return backend()->createBlock(name, type);
}


bool File::hasBlock(const Block &block) const {
    if(!util::checkEntityInput(block, false)) {
        return false;
    }
    return backend()->hasBlock(block.id());
}


bool File::deleteBlock(const Block &block) {
    if(!util::checkEntityInput(block, false)) {
        return false;
    }
    return backend()->deleteBlock(block.id());
}


std::vector<Block> File::blocks(const util::Filter<Block>::type &filter) const
{
    auto f = [this] (ndsize_t i) { return getBlock(i); };
    return getEntities<Block>(f, blockCount(), filter);
}


Section File::createSection(const std::string &name, const std::string &type) {
    util::checkEntityNameAndType(name, type);
    if (backend()->hasSection(name)) {
        throw DuplicateName("Section with the given name already exists!");
    }
    return backend()->createSection(name, type);
}


bool File::hasSection(const Section &section) const {
    if(!util::checkEntityInput(section, false)) {
        return false;
    }
    return backend()->hasSection(section.id());
}


std::vector<Section> File::sections(const util::Filter<Section>::type &filter) const
{
    auto f = [this] (ndsize_t i) { return getSection(i); };
    return getEntities<Section>(f, sectionCount(), filter);
}


bool File::deleteSection(const Section &section) {
    if(!util::checkEntityInput(section, false)) {
        return false;
    }
    return deleteSection(section.id());
}


std::vector<Section> File::findSections(const util::Filter<Section>::type &filter, size_t max_depth) const {
    std::vector<Section> results;
    std::vector<Section> roots = sections();
    if (max_depth == 0) {
        return results;
    }
    for (auto root : roots) {
        if (filter(root))
            results.push_back(root);
        std::vector<Section> secs = root.findSections(filter, max_depth - 1);
        results.insert(results.end(), secs.begin(), secs.end());
    }
    return results;
}


valid::Result File::validate() const {
    valid::Result result;
    // now get all entities from the file: use the multi-getter for each type of entity
    // (the multi-getters use size_t-getter which in the end use H5Lget_name_by_idx
    // to get each file objects name - the count is determined by H5::Group::getNumObjs
    // so that in the end really all file objects are retrieved)

    // Blocks
    auto blcks = blocks();
    for (auto &block : blcks) {
        result.concat(valid::validate(block));
        // DataArrays
        auto data_arrays = block.dataArrays();
        for (auto &data_array : data_arrays) {
            result.concat(valid::validate(data_array));
            // Dimensions
            auto dims = data_array.dimensions();
            for (auto &dim : dims) {
                if (dim.dimensionType() == DimensionType::Range) {
                    auto d = dim.asRangeDimension();
                    result.concat(valid::validate(d));
                }
                if (dim.dimensionType() == DimensionType::Set) {
                    auto d = dim.asSetDimension();
                    result.concat(valid::validate(d));
                }
                if (dim.dimensionType() == DimensionType::Sample) {
                    auto d = dim.asSampledDimension();
                    result.concat(valid::validate(d));
                }
            }
        }
        // MultiTags
        auto multi_tags = block.multiTags();
        for (auto &multi_tag : multi_tags) {
            result.concat(valid::validate(multi_tag));
            // Features
            auto features = multi_tag.features();
            for (auto &feature : features) {
                result.concat(valid::validate(feature));
            }
        }
        // Tags
        auto tags = block.tags();
        for (auto &tag : tags) {
            result.concat(valid::validate(tag));
            // Features
            auto features = tag.features();
            for (auto &feature : features) {
                result.concat(valid::validate(feature));
            }
        }
        // Sources
        auto sources = block.findSources();
        for (auto &source : sources) {
            result.concat(valid::validate(source));
        }
    }
    // Sections
    auto sections = findSections();
    for (auto &section : sections) {
        result.concat(valid::validate(section));
        // Properties
        auto props = section.properties();
        for (auto &prop : props) {
            result.concat(valid::validate(prop));
        }
    }

    return result;
}


void File::close() {
    if (!isNone()) {
        backend()->close();
        nullify();
    }
}

}
