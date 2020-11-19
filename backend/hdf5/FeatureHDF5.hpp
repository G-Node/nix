// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_FEATURE_HDF5_H
#define NIX_FEATURE_HDF5_H

#include <nix/base/IBlock.hpp>
#include <nix/base/IFeature.hpp>
#include "EntityHDF5.hpp"

#include <string>
#include <vector>

namespace nix {
namespace hdf5 {


/**
 * Converts a LinkType into a string.
 *
 * @param link_type   The type to convert.
 */
std::string linkTypeToString(LinkType link_type);

/**
 * Create a link type from a string. If no matching type
 * exists, an exception will be thrown.
 *
 * @return The link type that matches the string.
 */
LinkType linkTypeFromString(const std::string &str);

/**
 * Converts a TargetType into a string.
 *
 * @param target_type   The type to convert.
 */
std::string targetTypeToString(TargetType target_type);

/**
 * Create a target type from a string. If no matching type
 * exists, an exception will be thrown.
 *
 * @return The target type that matches the string.
 */
TargetType targetTypeFromString(const std::string &str);


/**
 * Class that represents a NIX feature entity
 */
class FeatureHDF5 : virtual public base::IFeature, public EntityHDF5 {

private:

    std::shared_ptr<base::IBlock> block;
    void targetType(TargetType type);
    
public:

    /**
     * Standard constructor for existing Feature
     */
    FeatureHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const H5Group &group);

    /**
     * Standard constructor for new Feature
     */
    FeatureHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const H5Group &group,
                const std::string &id, DataArray data, LinkType link_type);

    /**
     * Standard constructor for new Feature with time
     */
    FeatureHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const H5Group &group,
                const std::string &id, DataArray data, LinkType link_type, time_t time);


    void linkType(LinkType type);


    LinkType linkType() const;

    
    TargetType targetType() const;


    void data(const std::string &name_or_id);


    std::shared_ptr<base::IDataArray> data() const;


    virtual ~FeatureHDF5();

};


} // namespace hdf5
} // namespace nix

#endif // NIX_FEATURE_HDF5_H
