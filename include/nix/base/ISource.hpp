// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_SOURCE_H
#define NIX_I_SOURCE_H

#include <string>
#include <vector>

#include <nix/base/IEntityWithMetadata.hpp>

namespace nix {


class Source;


namespace base {


/**
 * @brief Interface for implementations of the Source entity.
 *
 * See {@link nix::Source} for a more detailed description.
 */
class NIXAPI ISource : virtual public IEntityWithMetadata {

public:

    virtual bool hasSource(const std::string &id) const = 0;


    virtual Source getSource(const std::string &id) const = 0;


    virtual Source getSource(size_t index) const = 0;


    virtual size_t sourceCount() const = 0;


    virtual Source createSource(const std::string &name, const std::string &type) = 0;


    virtual bool deleteSource(const std::string &id) = 0;


    virtual ~ISource() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_SOURCE_H
