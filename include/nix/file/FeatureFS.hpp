// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_FEATUREFS_HPP
#define NIX_FEATUREFS_HPP

#include <nix/base/IBlock.hpp>
#include <nix/base/IFeature.hpp>
#include <nix/file/EntityFS.hpp>

#include <string>
#include <vector>

namespace nix {
namespace file {


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
 * Class that represents a NIX feature entity
 */
class FeatureFS : virtual public base::IFeature, public EntityFS {

private:

    std::shared_ptr<base::IBlock> block;

public:

    /**
     * Standard constructor for existing Feature
     */
    FeatureFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc);

    /**
     * Standard constructor for new Feature
     */
    FeatureFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc,
              const std::string &id, DataArray data, LinkType link_type);

    /**
     * Standard constructor for new Feature with time
     */
    FeatureFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc,
              const std::string &id, DataArray data, LinkType link_type, time_t time);


    void linkType(LinkType type);


    LinkType linkType() const;


    void data(const std::string &name_or_id);


    std::shared_ptr<base::IDataArray> data() const;


    virtual ~FeatureFS();

};


} // namespace file
} // namespace nix

#endif //NIX_FEATUREFS_HPP
