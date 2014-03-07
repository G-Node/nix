// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_PROPERTY_H
#define NIX_PROPERTY_H

#include <nix/base/NamedEntity.hpp>
#include <nix/base/IProperty.hpp>
#include <nix/Value.hpp>

#include <nix/Platform.hpp>

namespace nix {


class NIXAPI Property : virtual public base::IProperty, public base::NamedEntity<base::IProperty> {

public:

    Property()
        : NamedEntity()
    {
    }


    Property(const Property &other)
        : NamedEntity(other.impl())
    {
    }

    Property(const std::shared_ptr<base::IProperty> &p_impl)
        : NamedEntity(p_impl)
    {
    }

    //--------------------------------------------------
    // Attribute getter and setter
    //--------------------------------------------------

    // TODO implement include
    /*
    void include(const std::string &include) {
        backend()->include(include);
    }


    std::string include() const {
        return backend()->include();
    }
    */


    void mapping(const std::string &mapping) {
        backend()->mapping(mapping);
    }


    boost::optional<std::string> mapping() const {
        return backend()->mapping();
    }
    
    
    void mapping(const boost::none_t t) {
        backend()->mapping();
    }


    boost::optional<DataType> dataType() const {
        return backend()->dataType();
    }


    void unit(const std::string &unit) {
        backend()->unit(unit);
    }


    boost::optional<std::string> unit() const {
        return backend()->unit();
    }
    
    
    void unit(const boost::none_t t) {
        return backend()->unit(t);
    }

    //--------------------------------------------------
    // Methods for Value access
    //--------------------------------------------------

    void deleteValues() {
        backend()->deleteValues();
    }


    size_t valueCount() const {
        return backend()->valueCount();
    }

    void values(const std::vector<Value> &values) {
        backend()->values(values);
    }

    std::vector<Value> values(void) const {
        return backend()->values();
    }
    
    void values(const boost::none_t t) {
        backend()->values(t);
    }

    //------------------------------------------------------
    // Operators and other functions
    //------------------------------------------------------

    virtual Property &operator=(std::nullptr_t) {
        nullify();
        return *this;
    }

    /**
     * Output operator
     */
    friend std::ostream& operator<<(std::ostream &out, const Property &ent) {
        out << "Property: {name = " << ent.name() << "}";
        return out;
    }

    virtual ~Property() {}

};


} // namespace nix

#endif // NIX_PROPERTY_H
