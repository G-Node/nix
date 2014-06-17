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
#include <nix/Exception.hpp>

namespace nix {
namespace base {

/**
 * @brief Interface for implementations of the Property entity.
 *
 * See {@link nix::Property} for a more detailed description.
 */
class NIXAPI IProperty : virtual public base::INamedEntity {

public:

    // TODO implement include
    /*
    virtual void include(const std::string &include) = 0;


    virtual std::string include() const = 0;
    */

    /**
     * @brief Set the mapping information for this Property.
     *
     * The mapping defines how this Property should be treated in a mapping procedure. The mapping
     * is provided in form of an url pointing to the definition of a section into which this
     * property should be mapped.
     *
     * @param mapping   The mapping information.
     */
    virtual void mapping(const std::string &mapping) = 0;

    /**
     * @brief Getter for the mapping information stored in this Property.
     *
     * @return The mapping for the Property.
     */
    virtual boost::optional<std::string> mapping() const = 0;

    /**
     * @brief Deletes the mapping information.
     *
     * @param t         None
     */
    virtual void mapping(const none_t t) = 0;

    /**
     * @brief Returns the data type of the stored Values.
     *
     * @return The data type.
     */
    virtual boost::optional<DataType> dataType() const = 0;

    /**
     * @brief Set the unit for all stored values.
     *
     * @param unit      The unit for all values.
     */
    virtual void unit(const std::string &unit) = 0;

    /**
     * @brief Returns the unit for all stored values.
     *
     * @return The unit for all values.
     */
    virtual boost::optional<std::string> unit() const = 0;

    /**
     * @brief Remove the unit.
     *
     * @param t         None
     */
    virtual void unit(const none_t t) = 0;

    /**
     * @brief Deletes all values from the property.
     */
    virtual void deleteValues() = 0;

    /**
     * @brief Get the number of values of the property.
     *
     * @return The number of values.
     */
    virtual size_t valueCount() const = 0;

    /**
     * @brief Set the values of the property.
     *
     * @param values    The values to set.
     */
    virtual void values(const std::vector<Value> &values) = 0;

    /**
     * @brief Get all values of the property.
     *
     * @return The values of the property.
     */
    virtual std::vector<Value> values(void) const = 0;

    /**
     * @brief Deletes all values from the property.
     */
    virtual void values(const boost::none_t t) = 0;

    /**
     * @brief Destructor
     */
    virtual ~IProperty() {}
};


} // namespace base
} // namespace nix

#endif // NIX_I_PROPERTY_H
