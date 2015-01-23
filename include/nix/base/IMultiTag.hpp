// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_MULTI_TAG_H
#define NIX_I_MULTI_TAG_H

#include <vector>
#include <string>

#include <nix/base/IEntityWithSources.hpp>
#include <nix/base/IDataArray.hpp>
#include <nix/base/IFeature.hpp>
#include <nix/base/IBaseTag.hpp>

namespace nix {

class DataArray;

namespace base {

/**
 * @brief Interface for implementations of the MultiTag entity.
 *
 * See {@link nix::MultiTag} for a more detailed description.
 */
class NIXAPI IMultiTag : virtual public base::IBaseTag {

public:

    //--------------------------------------------------
    // Element getters and setters
    //--------------------------------------------------

    virtual std::shared_ptr<base::IDataArray> positions() const = 0;


    virtual void positions(const std::string &name_or_id) = 0;


    virtual bool hasPositions() const = 0;


    virtual std::shared_ptr<base::IDataArray> extents() const = 0;


    virtual void extents(const std::string &name_or_id) = 0;


    virtual void extents(const none_t t) = 0;


    virtual std::vector<std::string> units() const = 0;


    virtual void units(const std::vector<std::string> &units) = 0;


    virtual void units(const none_t t) = 0;

    /**
     * @brief Destructor
     */
    virtual ~IMultiTag() {}

};

} // namespace base
} // namespace nix

#endif // NIX_I_MULTI_TAG_H
