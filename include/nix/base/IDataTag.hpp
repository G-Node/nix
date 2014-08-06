// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_DATA_TAG_H
#define NIX_I_DATA_TAG_H

#include <vector>
#include <string>

#include <nix/base/IEntityWithSources.hpp>
#include <nix/base/IDataArray.hpp>
#include <nix/base/IFeature.hpp>

namespace nix {

class DataArray;

namespace base {

/**
 * @brief Interface for implementations of the DataTag entity.
 *
 * See {@link nix::DataTag} for a more detailed description.
 */
class NIXAPI IDataTag : virtual public base::IEntityWithSources {

public:

    //--------------------------------------------------
    // Element getters and setters
    //--------------------------------------------------

    virtual std::shared_ptr<base::IDataArray> positions() const = 0;


    virtual void positions(const std::string &id) = 0;


    virtual bool hasPositions() const = 0;


    virtual std::shared_ptr<base::IDataArray> extents() const = 0;


    virtual void extents(const std::string &id) = 0;


    virtual void extents(const none_t t) = 0;


    virtual std::vector<std::string> units() const = 0;


    virtual void units(const std::vector<std::string> &units) = 0;


    virtual void units(const none_t t) = 0;

    //--------------------------------------------------
    // Methods concerning references.
    // TODO implement when done with the DataArray class.
    //--------------------------------------------------


    virtual bool hasReference(const std::string &id) const = 0;


    virtual size_t referenceCount() const = 0;


    virtual std::shared_ptr<base::IDataArray> getReference(const std::string &id) const = 0;


    virtual std::shared_ptr<base::IDataArray> getReference(size_t index) const = 0;


    virtual void addReference(const std::string &id) = 0;


    virtual bool removeReference(const std::string &id) = 0;


    virtual void references(const std::vector<DataArray> &references) = 0;

    //--------------------------------------------------
    // Methods concerning features.
    //--------------------------------------------------


    virtual bool hasFeature(const std::string &id) const = 0;


    virtual size_t featureCount() const = 0;


    virtual std::shared_ptr<base::IFeature> getFeature(const std::string &id) const = 0;


    virtual std::shared_ptr<base::IFeature> getFeature(size_t index) const = 0;


    virtual std::shared_ptr<base::IFeature> createFeature(const std::string &id, LinkType link_type) = 0;


    virtual bool deleteFeature(const std::string &id) = 0;

    /**
     * @brief Destructor
     */
    virtual ~IDataTag() {}

};

} // namespace base
} // namespace nix

#endif // NIX_I_DATA_TAG_H
