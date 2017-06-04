// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_DIMENSIONS_H
#define NIX_I_DIMENSIONS_H

#include <nix/Platform.hpp>

#include <string>
#include <vector>
#include <ostream>

#include <boost/optional.hpp>
#include <nix/NDSize.hpp>
#include <nix/ObjectType.hpp>

namespace nix {

/**
 * @brief Enumeration that provides constants for different dimension types.
 */
enum class DimensionType : unsigned int {
    Sample, Set, Range
};

/**
 * @brief Output operator for DimensionType
 */
NIXAPI std::ostream& operator<<(std::ostream &out, const DimensionType &dtype);

namespace base {

/**
 * @brief Interface for implementations of the Dimension entity.
 *
 * See {@link nix::Dimension} for more detailed information.
 */
class NIXAPI IDimension {

protected:
    typedef boost::none_t none_t;

public:

    virtual ndsize_t index() const = 0;


    virtual DimensionType dimensionType() const = 0;


    virtual ~IDimension() {}

};


/**
 * @brief Interface for implementations of the SampledDimension entity.
 *
 * See {@link nix::SampledDimension} for more detailed information.
 */
class NIXAPI ISampledDimension : virtual public IDimension {

public:

    virtual boost::optional<std::string> label() const = 0;


    virtual void label(const std::string &label) = 0;


    virtual void label(const none_t t) = 0;


    virtual boost::optional<std::string> unit() const = 0;


    virtual void unit(const std::string &unit) = 0;


    virtual void unit(const none_t t) = 0;


    virtual double samplingInterval() const = 0;


    virtual void samplingInterval(double interval) = 0;


    virtual boost::optional<double> offset() const = 0;


    virtual void offset(double offset) = 0;


    virtual void offset(const none_t t) = 0;


    virtual ~ISampledDimension() {}

};


/**
 * @brief Interface for implementations of the SetDimension entity.
 *
 * See {@link nix::SetDimension} for more detailed information.
 */
class NIXAPI ISetDimension : virtual public IDimension {

public:

    virtual std::vector<std::string> labels() const = 0;


    virtual void labels(const std::vector<std::string> &labels) = 0;


    virtual void labels(const none_t t) = 0;


    virtual ~ISetDimension() {}

};


/**
 * @brief Interface for implementations of the RangeDimension entity.
 *
 * See {@link nix::RangeDimension} for more detailed information.
 */
class NIXAPI IRangeDimension : virtual public IDimension {

public:

    virtual boost::optional<std::string> label() const = 0;

    
    virtual bool alias() const = 0;


    virtual void label(const std::string &label) = 0;


    virtual void label(const none_t t) = 0;


    virtual boost::optional<std::string> unit() const = 0;


    virtual void unit(const std::string &unit) = 0;


    virtual void unit(const none_t t) = 0;


    virtual std::vector<double> ticks() const = 0;


    virtual void ticks(const std::vector<double> &ticks) = 0;


    virtual ~IRangeDimension() {}

};


} // namespace base

template<>
struct objectToType<nix::base::ISetDimension> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::SetDimension;
};

template<>
struct objectToType<nix::base::ISampledDimension> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::SampledDimension;
};


template<>
struct objectToType<nix::base::IRangeDimension> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::RangeDimension;
};

} // namespace nix

#endif // NIX_I_DIMENSIONS_H
