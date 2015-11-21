// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//

#ifndef NIX_IGROUP_HPP
#define NIX_IGROUP_HPP

#include <nix/base/IEntityWithSources.hpp>
#include <nix/base/ISource.hpp>
#include <nix/base/IDataArray.hpp>
#include <nix/base/ITag.hpp>
#include <nix/base/IMultiTag.hpp>
#include <nix/NDSize.hpp>
#include <nix/Tag.hpp>
#include <nix/MultiTag.hpp>


namespace nix {
namespace base {

/**
 * @brief Interface for implementations of the Group entity.
 *
 * See {@link nix::Group} for a more detailed description.
 */
class NIXAPI IGroup : virtual public IEntityWithSources {

public:

    //--------------------------------------------------
    // Methods concerning data arrays.
    //--------------------------------------------------

    virtual bool hasDataArray(const std::string &id) const = 0;


    virtual ndsize_t dataArrayCount() const = 0;


    virtual std::shared_ptr<IDataArray> getDataArray(const std::string &id) const = 0;


    virtual std::shared_ptr<IDataArray> getDataArray(ndsize_t index) const = 0;


    virtual void addDataArray(const std::string &id) = 0;


    virtual bool removeDataArray(const std::string &id) = 0;


    virtual void dataArrays(const std::vector<DataArray> &data_arrays) = 0;

    //--------------------------------------------------
    // Methods concerning tags.
    //--------------------------------------------------

    virtual bool hasTag(const std::string &id) const = 0;


    virtual ndsize_t tagCount() const = 0;


    virtual std::shared_ptr<ITag> getTag(const std::string &id) const = 0;


    virtual std::shared_ptr<ITag> getTag(ndsize_t index) const = 0;


    virtual void addTag(const std::string &id) = 0;


    virtual bool removeTag(const std::string &id) = 0;


    virtual void tags(const std::vector<Tag> &tags) = 0;

    //--------------------------------------------------
    // Methods concerning multi tags.
    //--------------------------------------------------

    virtual bool hasMultiTag(const std::string &id) const = 0;


    virtual ndsize_t multiTagCount() const = 0;


    virtual std::shared_ptr<IMultiTag> getMultiTag(const std::string &id) const = 0;


    virtual std::shared_ptr<IMultiTag> getMultiTag(ndsize_t index) const = 0;


    virtual void addMultiTag(const std::string &id) = 0;


    virtual bool removeMultiTag(const std::string &id) = 0;


    virtual void multiTags(const std::vector<MultiTag> &tags) = 0;

    /**
    * @brief Destructor
    */
    virtual ~IGroup() {}
};

} // namespace base
} // namespace nix

#endif //NIX_IGROUP_HPP
