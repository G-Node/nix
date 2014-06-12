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

/**
 * @brief Class representing an odML property entity.
 *
 * In the odML model information is stored in the form of extended
 * key-value pairs. A Property contains information that is valid for
 * all Values stored in it. Its {@link dataType} provides information about
 * the type of the stored Value entities (e.g. double or integer).
 *
 * The {@link unit} is the unit of the stored values. Similar
 * to the {@link nix::Section} entity, mapping information can be provided
 * using the {@link mapping} field.
 */
class NIXAPI Property : virtual public base::IProperty, public base::NamedEntity<base::IProperty> {

public:

    /**
     * @brief Constructor that creates a null property.
     */
    Property()
        : NamedEntity()
    {
    }

    /**
     * @brief Copy constructor.
     *
     * @param other     The property to copy.
     */
    Property(const Property &other)
        : NamedEntity(other.impl())
    {
    }

    /**
     * @brief Constructor that creates a new Property from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    Property(const std::shared_ptr<base::IProperty> &p_impl)
        : NamedEntity(p_impl)
    {
    }

    /**
     * @brief Constructor with move semantics that creates a new Property from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    Property(std::shared_ptr<base::IProperty> &&ptr)
        : NamedEntity(std::move(ptr))
    {
    }


    //--------------------------------------------------
    // Attribute getter and setter
    //--------------------------------------------------


    void mapping(const std::string &mapping) {
        backend()->mapping(mapping);
    }

    boost::optional<std::string> mapping() const {
        return backend()->mapping();
    }

    void mapping(const boost::none_t t) {
        backend()->mapping(t);
    }

    boost::optional<DataType> dataType() const {
        return backend()->dataType();
    }

    void unit(const std::string &unit) {
        if (backend()->valueCount() > 0 && backend()->unit()) {
            throw std::runtime_error("Cannot change unit of a not-empty property!");
        }

        std::string clean_unit = util::deblankString(unit);
        if (!(util::isSIUnit(clean_unit) || util::isCompoundSIUnit(clean_unit))) {
            throw InvalidUnit("Unit is not SI or composite of SI units.", "Property::unit(const string &unit)");
        }
        backend()->unit(clean_unit);
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

    /**
     * @brief Assignment operator for none.
     */
    virtual Property &operator=(none_t) {
        nullify();
        return *this;
    }

    /**
     * @brief Output operator
     */
    friend std::ostream& operator<<(std::ostream &out, const Property &ent) {
        out << "Property: {name = " << ent.name() << "}";
        return out;
    }

    virtual ~Property() {}

};


} // namespace nix

#endif // NIX_PROPERTY_H
