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

#include <nix/Value.hpp>
#include <nix/base/INamedEntity.hpp>

namespace nix {
namespace base {

class NIXAPI IProperty : virtual public base::INamedEntity {

public:

    // TODO implement include
    /*
    virtual void include(const std::string &include) = 0;


    virtual std::string include() const = 0;
    */

    virtual void mapping(const std::string &mapping) = 0;


    virtual boost::optional<std::string> mapping() const = 0;


    virtual void mapping(const none_t t) = 0;
    

    virtual boost::optional<DataType> dataType() const = 0;


    virtual void unit(const std::string &unit) = 0;


    virtual boost::optional<std::string> unit() const = 0;


    virtual void unit(const none_t t) = 0;
    

    virtual void deleteValues() = 0;


    virtual size_t valueCount() const = 0;


    virtual void values(const std::vector<Value> &values) = 0;


    virtual std::vector<Value> values(void) const = 0;
    
    
    virtual void values(const boost::none_t t) = 0;


    virtual ~IProperty() {}
};


} // namespace base
} // namespace nix

#endif // NIX_I_PROPERTY_H
