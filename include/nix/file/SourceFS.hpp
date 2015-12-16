// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_SOURCEFS_HPP
#define NIX_SOURCEFS_HPP

#include <nix/base/ISource.hpp>
#include <nix/file/EntityWithMetadataFS.hpp>
#include <vector>
#include <string>
#include <boost/optional.hpp>

namespace nix {
namespace file {

/**
 * An entity that implements ISource for the filesystem back-end.
 */
class SourceFS : virtual public base::ISource, public EntityWithMetadataFS  {

private:
    Directory sources_dir;

    void createSubFolders(const std::shared_ptr<base::IFile> &file);

public:

    /**
     * Standard constructor for existing Source
     */
    SourceFS(const std::shared_ptr<base::IFile> &file, const std::string &loc);

    /**
     * Default constructor.
     */
    SourceFS(const std::shared_ptr<base::IFile> &file, const std::string &loc, const std::string &id, const std::string &type,
               const std::string &name);

    /**
     * Default constructor that preserves the creation time.
     */
    SourceFS(const std::shared_ptr<base::IFile> &file, const std::string &loc, const std::string &id, const std::string &type,
               const std::string &name, time_t time);

    //--------------------------------------------------
    // Methods concerning child sources
    //--------------------------------------------------


    bool hasSource(const std::string &name_or_id) const;


    std::shared_ptr<base::ISource> getSource(const std::string &name_or_id) const;


    std::shared_ptr<base::ISource> getSource(ndsize_t index) const;


    ndsize_t sourceCount() const;


    std::shared_ptr<base::ISource> createSource(const std::string &name, const std::string &type);


    bool deleteSource(const std::string &name_or_id);

    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------


    virtual ~SourceFS();
};

} // namestace file
} // namespace nix

#endif //NIX_SOURCEFS_HPP