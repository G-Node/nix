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
#include <nix/ObjectType.hpp>

#include <string>
#include <memory>

namespace nix {
namespace base {

class IBlock;

/**
 * @brief Interface for implementations of the Source entity.
 *
 * See {@link nix::Source} for a more detailed description.
 */
class NIXAPI ISource : virtual public IEntityWithMetadata {

public:

    virtual bool hasSource(const std::string &name_or_id) const = 0;


    virtual std::shared_ptr<ISource> getSource(const std::string &name_or_id) const = 0;


    virtual std::shared_ptr<ISource> getSource(ndsize_t index) const = 0;


    virtual ndsize_t sourceCount() const = 0;


    virtual std::shared_ptr<ISource> createSource(const std::string &name, const std::string &type) = 0;


    virtual bool deleteSource(const std::string &name_or_id) = 0;


    virtual std::shared_ptr<IFile> parentFile() const = 0;


    virtual std::shared_ptr<IBlock> parentBlock() const = 0;


    virtual ~ISource() {}

};


} // namespace base

template<>
struct objectToType<nix::base::ISource> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::Source;
};

} // namespace nix

#endif // NIX_I_SOURCE_H
