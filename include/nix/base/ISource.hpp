// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_SOURCE_H
#define NIX_I_SOURCE_H


#include <nix/base/IEntityWithMetadata.hpp>

#include <string>
#include <memory>

namespace nix {
namespace base {


/**
 * @brief Interface for implementations of the Source entity.
 *
 * See {@link nix::Source} for a more detailed description.
 */
class NIXAPI ISource : virtual public IEntityWithMetadata {

public:

    virtual bool hasSource(const std::string &name_or_id) const = 0;


    virtual std::shared_ptr<ISource> getSource(const std::string &name_or_id) const = 0;


    virtual std::shared_ptr<ISource> getSource(size_t index) const = 0;


    virtual size_t sourceCount() const = 0;


    virtual std::shared_ptr<ISource> createSource(const std::string &name, const std::string &type) = 0;


    virtual bool deleteSource(const std::string &name_or_id) = 0;


    virtual ~ISource() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_SOURCE_H
