// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_SIMPLE_TAG_H
#define NIX_I_SIMPLE_TAG_H

#include <string>
#include <vector>

#include <nix/base/IEntityWithSources.hpp>
#include <nix/base/IDataArray.hpp>
#include <nix/base/IFeature.hpp>

namespace nix {

class DataArray;

namespace base {

/**
 * @brief Interface for implementations of the Tag entity.
 *
 * See {@link nix::Tag} for a more detailed description.
 */
class NIXAPI ITag : virtual public base::IEntityWithSources {

public:

    virtual std::vector<std::string> units() const = 0;


    virtual void units(const std::vector<std::string> &units) = 0;


    virtual void units(const none_t t) = 0;


    virtual std::vector<double> position() const = 0;


    virtual void position(const std::vector<double> &position) = 0;


    virtual std::vector<double> extent() const = 0;


    virtual void extent(const std::vector<double> &extent) = 0;


    virtual void extent(const none_t t) = 0;

    //--------------------------------------------------
    // Methods concerning references.
    //--------------------------------------------------

    virtual bool hasReference(const std::string &id) const = 0;


    virtual size_t referenceCount() const = 0;


    virtual std::shared_ptr<IDataArray> getReference(const std::string &id) const = 0;


    virtual std::shared_ptr<IDataArray> getReference(size_t index) const = 0;


    virtual void addReference(const std::string &id) = 0;


    virtual bool removeReference(const std::string &id) = 0;

    // TODO evaluate if DataArray can be replaced by shared_ptr<IDataArray>
    virtual void references(const std::vector<DataArray> &references) = 0;

    //--------------------------------------------------
    // Methods concerning features.
    //--------------------------------------------------

    virtual bool hasFeature(const std::string &id) const = 0;


    virtual size_t featureCount() const = 0;


    virtual std::shared_ptr<IFeature> getFeature(const std::string &id) const = 0;


    virtual std::shared_ptr<IFeature> getFeature(size_t index) const = 0;


    virtual std::shared_ptr<IFeature> createFeature(const std::string &data_array_id, LinkType link_type) = 0;


    virtual bool deleteFeature(const std::string &id) = 0;


    virtual ~ITag() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_SIMPLE_TAG_H
