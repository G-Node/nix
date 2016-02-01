// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
#ifndef NIX_GROUPFS_HPP
#define NIX_GROUPFS_HPP


#include "EntityWithSourcesFS.hpp"
#include "Directory.hpp"
#include <nix/util/util.hpp>
#include <nix/Group.hpp>
#include "hdf5/DataArrayHDF5.hpp"


namespace nix {
namespace file {

/**
 * Class that represents a NIX Group entity.
 */
class GroupFS : virtual public base::IGroup, public  EntityWithSourcesFS,
                public std::enable_shared_from_this<GroupFS> {

private:

    Directory data_array_group, tag_group, multi_tag_group;

    void createSubFolders(const std::shared_ptr<base::IFile> &file);


public:

    /**
    * Standard constructor for an existing Group
    */
    GroupFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc);

    /**
    * Standard constructor for a new Group Tag
    */
    GroupFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc, const std::string &id,
              const std::string &type, const std::string &name);

    /**
    * Standard constructor for new Group that preserves the creation time.
    */
    GroupFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc, const std::string &id,
              const std::string &type, const std::string &name, const time_t time);


    //--------------------------------------------------
    // Methods concerning data arrays.
    //--------------------------------------------------

    virtual bool hasDataArray(const std::string &name_or_id) const;


    virtual ndsize_t dataArrayCount() const;


    virtual std::shared_ptr<base::IDataArray> getDataArray(const std::string &name_or_id) const;


    virtual std::shared_ptr<base::IDataArray> getDataArray(ndsize_t index) const;


    virtual void addDataArray(const std::string &name_or_id);


    virtual bool removeDataArray(const std::string &name_or_id);

    // TODO evaluate if DataArray can be replaced by shared_ptr<IDataArray>
    virtual void dataArrays(const std::vector<DataArray> &data_arrays);

    //--------------------------------------------------
    // Methods concerning tags.
    //--------------------------------------------------

    virtual bool hasTag(const std::string &name_or_id) const;


    virtual ndsize_t tagCount() const;


    virtual std::shared_ptr<base::ITag> getTag(const std::string &name_or_id) const;


    virtual std::shared_ptr<base::ITag> getTag(ndsize_t index) const;


    virtual void addTag(const std::string &name_or_id);


    virtual bool removeTag(const std::string &name_or_id);


    virtual void tags(const std::vector<Tag> &tags);

    //--------------------------------------------------
    // Methods concerning multi tags.
    //--------------------------------------------------

    virtual bool hasMultiTag(const std::string &name_or_id) const;


    virtual ndsize_t multiTagCount() const;


    virtual std::shared_ptr<base::IMultiTag> getMultiTag(const std::string &name_or_id) const;


    virtual std::shared_ptr<base::IMultiTag> getMultiTag(ndsize_t index) const;


    virtual void addMultiTag(const std::string &name_or_id);


    virtual bool removeMultiTag(const std::string &name_or_id);


    virtual void multiTags(const std::vector<MultiTag> &multi_tags);
};
}
}

#endif //NIX_GROUPFS_HPP