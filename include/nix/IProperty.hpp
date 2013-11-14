// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_PROPERTY_H
#define NIX_I_PROPERTY_H

#include <string>
#include <vector>

#include <nix/base/INamedEntity.hpp>

namespace nix {

class IProperty : virtual public base::INamedEntity {

public:

    virtual void include(const std::string &include) = 0;

    virtual std::string include() const = 0;

    virtual void mapping(const std::string &mapping) = 0;

    virtual std::string mapping() const = 0;

    virtual void dataType(const std::string &dataType) = 0;

    virtual std::string dataType() const = 0;

    virtual void unit(const std::string &unit) = 0;

    virtual std::string unit() const = 0;

    // TODO what about the templates

    virtual void removeValue(size_t index) = 0;

    virtual void removeValues() = 0;

    virtual size_t valueCount() const = 0;

};


} // namespace nix

#endif // NIX_I_PROPERTY_H
