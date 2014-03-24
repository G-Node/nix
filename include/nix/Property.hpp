// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_PROPERTY_H
#define NIX_PROPERTY_H

#include <stdexcept>

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

    /**
     * Set the mapping information for this Property. The mapping defines how
     * this Property should be treated in a mapping procedure. The mapping is provided
     * in form of an url pointing to the definition of a section into which this
     * property should be mapped
     * (e.g. http:// ... /preparation/preparation.xml#preparation:BathSolution
     *
     * @param mapping string the mapping information.
     *
     */
    void mapping(const std::string &mapping) {
        backend()->mapping(mapping);
    }

    /**
     * Getter for the mapping information stored in this Property.
     * The result may be not initialized, check on true before
     * dereferencing.
     *
     * @returns boost::optional<String> the mapping
     */
    boost::optional<std::string> mapping() const {
        return backend()->mapping();
    }
    
    /**
     * Deletes the mapping information.
     *
     * @param boost::none_t
     */
    void mapping(const boost::none_t t) {
        backend()->mapping(t);
    }

    /**
     * Returns the dataType of the stored Values. Returned value may be
     * not initialized if no value present, check on true before dereferencing.
     *
     * @param boost::optional<nix::DataType> the value DataType
     *
     */
    boost::optional<DataType> dataType() const {
        return backend()->dataType();
    }

    /**
     * Set the unit of the stored values. Note: all values
     * have the same unit.
     *
     * @param std::string the unit
     */
    void unit(const std::string &unit) {
        if (backend()->valueCount() > 0 && backend()->unit()) {
            throw std::runtime_error("Cannot change unit of a not-empty property!");
        }
        if (!(util::isSIUnit(unit) || util::isCompoundSIUnit(unit))){
            throw InvalidUnit("Unit is not SI or composite of SI units.", "Property::unit(const string &unit)");
        }
        backend()->unit(unit);
    }

    /**
     * Returns the unit. Return value may be uninitialized,
     * check for true before dereferencing.
     *
     * @return boost::optional<String> the unit
     *
     */
    boost::optional<std::string> unit() const {
        return backend()->unit();
    }
    
    /**
     * Deletes the unit information.
     *
     * @param boost::none_t
     */
    void unit(const boost::none_t t) {
        return backend()->unit(t);
    }

    //--------------------------------------------------
    // Methods for Value access
    //--------------------------------------------------

    /**
     * Deletes all values of this Property.
     *
     */
    void deleteValues() {
        backend()->deleteValues();
    }

    /**
     * Returns the number of values stored in this Property.
     *
     * @return size_t the count
     */
    size_t valueCount() const {
        return backend()->valueCount();
    }

    /**
     * Set the values of this Property. Replaces all existing Values.
     *
     * @param std::vector<Value> the new values.
     */
    void values(const std::vector<Value> &values) {
        backend()->values(values);
    }

    /**
     * Returns the values stored in this Property. Returned vector may
     * be empty.
     *
     * @return std::vector<Value> the values.
     */
    std::vector<Value> values(void) const {
        return backend()->values();
    }
    
    /**
     * Deletes the values stored in this Property.
     *
     * @param boost::none_t
     */
    void values(const boost::none_t t) {
        backend()->values(t);
    }

    //------------------------------------------------------
    // Operators and other functions
    //------------------------------------------------------

    virtual Property &operator=(none_t) {
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
