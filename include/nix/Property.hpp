// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_PROPERTY_H
#define NIX_PROPERTY_H

#include <nix/base/Entity.hpp>
#include <nix/base/IProperty.hpp>
#include <nix/Value.hpp>
#include <nix/ObjectType.hpp>

#include <nix/Platform.hpp>

#include <ostream>
#include <memory>

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
class NIXAPI Property : public base::Entity<base::IProperty> {

public:

    /**
     * @brief Constructor that creates an uninitialized Property.
     *
     * Calling any method on an uninitialized property will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a property is initialized:
     *
     * ~~~
     * Property e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    Property()
        : Entity()
    {
    }

    /**
     * @brief Copy constructor.
     *
     * Copying of all NIX front facing objects like Property is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The property to copy.
     */
    Property(const Property &other)
        : Entity(other.impl())
    {
    }

    /**
     * @brief Constructor that creates a new Property from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    Property(const std::shared_ptr<base::IProperty> &p_impl)
        : Entity(p_impl)
    {
    }

    /**
     * @brief Constructor with move semantics that creates a new Property from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    Property(std::shared_ptr<base::IProperty> &&ptr)
        : Entity(std::move(ptr))
    {
    }


    //--------------------------------------------------
    // Attribute getter and setter
    //--------------------------------------------------

    /**
     * @brief Getter for the name of the property.
     *
     * The {@link name} of an property serves as a human readable identifier. It is not obliged
     * to be unique. However it is strongly recommended to use unique name inside one specific
     * {@link nix::Section}.
     *
     * @return string The name of the property.
     */
    std::string name() const {
        return backend()->name();
    }

    /**
     * @brief Setter for the definition of the property.
     *
     * @param definition The definition of the property.
     */
    void definition(const std::string &definition);

    /**
     * @brief Getter for the definition of the property.
     *
     * The {@link definition} is an optional property that allows the user to add
     * a freely assignable textual definition to the property.
     *
     * @return The definition of the property.
     */
    boost::optional<std::string> definition() const {
        return backend()->definition();
    }

    /**
     * @brief Deleter for the definition of the property.
     */
    void definition(const none_t t) {
        backend()->definition(t);
    }

    /**
     * @brief Set the mapping information for this Property.
     *
     * The mapping defines how this Property should be treated in a mapping procedure. The mapping
     * is provided in form of an url pointing to the definition of a section into which this
     * property should be mapped.
     *
     * @param mapping   The mapping information.
     */
    void mapping(const std::string &mapping);

    /**
     * @brief Getter for the mapping information stored in this Property.
     *
     * @return The mapping for the Property.
     */
    boost::optional<std::string> mapping() const {
        return backend()->mapping();
    }

    /**
     * @brief Deletes the mapping information.
     *
     * @param t         None
     */
    void mapping(const boost::none_t t) {
        backend()->mapping(t);
    }

    /**
     * @brief Returns the data type of the stored Values.
     *
     * @return The data type.
     */
    DataType dataType() const {
        return backend()->dataType();
    }

    /**
     * @brief Set the unit for all stored values.
     *
     * @param unit      The unit for all values.
     */
    void unit(const std::string &unit);

    /**
     * @brief Returns the unit for all stored values.
     *
     * @return The unit for all values.
     */
    boost::optional<std::string> unit() const {
        return backend()->unit();
    }

    /**
     * @brief Remove the unit.
     *
     * @param t         None
     */
    void unit(const boost::none_t t) {
        return backend()->unit(t);
    }

    //--------------------------------------------------
    // Methods for Value access
    //--------------------------------------------------

    /**
     * @brief Deletes all values from the property.
     */
    void deleteValues() {
        backend()->deleteValues();
    }

    /**
     * @brief Get the number of values of the property.
     *
     * @return The number of values.
     */
    ndsize_t valueCount() const {
        return backend()->valueCount();
    }

    /**
     * @brief Set the values of the property.
     *
     * @param values    The values to set.
     */
    void values(const std::vector<Value> &values) {
        backend()->values(values);
    }

    /**
     * @brief Get all values of the property.
     *
     * @return The values of the property.
     */
    std::vector<Value> values(void) const {
        return backend()->values();
    }

    /**
     * @brief Deletes all values from the property.
     */
    void values(const boost::none_t t) {
        backend()->values(t);
    }

    //------------------------------------------------------
    // Operators and other functions
    //------------------------------------------------------

    /**
     * @brief Compare two properties.
     *
     * @param other The property to compare with.
     *
     * @return > 0 if the property is larger that other, 0 if both are
     * equal, and < 0 otherwise.
     */
    int compare(const Property &other) const;

    /**
     * @brief Assignment operator for none.
     */
    Property &operator=(const none_t &t) {
        ImplContainer::operator=(t);
        return *this;
    }

    /**
     * @brief Output operator
     */
    NIXAPI friend std::ostream& operator<<(std::ostream &out, const Property &ent);

    /**
     * @brief Destructor
     */
    virtual ~Property() {}

};

template<>
struct objectToType<nix::Property> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::Property;
    typedef nix::base::IProperty backendType;
};


} // namespace nix

#endif // NIX_PROPERTY_H
