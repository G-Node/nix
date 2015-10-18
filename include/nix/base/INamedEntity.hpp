// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_NAMEDENTITY_H
#define NIX_I_NAMEDENTITY_H


#include <nix/base/IEntity.hpp>

#include <boost/optional.hpp>

#include <string>
#include <memory>

namespace nix {
namespace base {

/**
 * @brief Interface for entities with a {@link name}, {@link type}
 * and {@link definition}.
 *
 * See {@link nix::base::NamedEntity} for more detailed information.
 */
class NIXAPI INamedEntity : virtual public IEntity {

public:

    virtual void type(const std::string &type) = 0;


    virtual std::string type() const = 0;


    virtual std::string name() const = 0;


    virtual void definition(const std::string &definition) = 0;


    virtual boost::optional<std::string> definition() const = 0;


    virtual void definition(const none_t t) = 0;


    virtual int compare(const std::shared_ptr<INamedEntity> &other) const = 0;


    virtual ~INamedEntity() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_NAMEDENTITY_H
