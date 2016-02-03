// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BASETAGFS_HPP
#define NIX_BASETAGFS_HPP

#include "EntityWithSourcesFS.hpp"
#include "Directory.hpp"
#include <nix/base/IBaseTag.hpp>

namespace nix {
namespace file {


/**
* Class that represents a NIX tag.
*/
class BaseTagFS : public EntityWithSourcesFS, virtual public base::IBaseTag {

private:

    Directory feature_group;
    Directory refs_group;

    void createSubFolders(const std::shared_ptr<base::IFile> &file);

public:

    /**
    * Standard constructor for existing Tag
    */
    BaseTagFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc);

    /**
    * Standard constructor for new Tag
    */
    BaseTagFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc, const std::string &id,
              const std::string &type, const std::string &name);

    /**
    * Standard constructor for new Tag that preserves the creation time.
    */
    BaseTagFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc, const std::string &id,
              const std::string &type, const std::string &name, const time_t time);

    //--------------------------------------------------
    // Methods concerning references.
    //--------------------------------------------------

    virtual bool hasReference(const std::string &name_or_id) const;


    virtual ndsize_t referenceCount() const;


    virtual std::shared_ptr<base::IDataArray> getReference(const std::string &name_or_id) const;


    virtual std::shared_ptr<base::IDataArray> getReference(ndsize_t index) const;


    virtual void addReference(const std::string &name_or_id);


    virtual bool removeReference(const std::string &name_or_id);

    // TODO evaluate if DataArray can be replaced by shared_ptr<IDataArray>
    virtual void references(const std::vector<DataArray> &references);

    //--------------------------------------------------
    // Methods concerning features.
    //--------------------------------------------------

    virtual bool hasFeature(const std::string &name_or_id) const;


    virtual ndsize_t featureCount() const;


    virtual std::shared_ptr<base::IFeature> getFeature(const std::string &name_or_id) const;


    virtual std::shared_ptr<base::IFeature> getFeature(ndsize_t index) const;


    virtual std::shared_ptr<base::IFeature> createFeature(const std::string &name_or_id, LinkType link_type);


    virtual bool deleteFeature(const std::string &name_or_id);


    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------

    /**
    * Destructor.
    */
    virtual ~BaseTagFS();

};


} // namespace file
} // namespace nix

#endif //NIX_BASETAGFS_HPP