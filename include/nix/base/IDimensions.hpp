// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_DIMENSIONS_H
#define NIX_I_DIMENSIONS_H

#include <string>
#include <vector>
#include <boost/optional.hpp>

namespace nix {

enum class DimensionType : int {
    Sample, Set, Range
};


namespace base {

/**
 * TODO documentation
 */
class IDimension {

public:

    virtual size_t id() const = 0;

    virtual DimensionType dimensionType() const = 0;

    virtual ~IDimension() {}

};


/**
 * TODO documentation
 */
class ISampledDimension : virtual public IDimension {

public:

    virtual std::string label() const = 0;

    virtual void label(const std::string &label) = 0;

    virtual std::string unit() const = 0;

    virtual void unit(const std::string &unit) = 0;

    virtual double samplingInterval() const = 0;

    virtual void samplingInterval(double interval) = 0;

    virtual boost::optional<double> offset() const = 0;

    virtual void offset(double offset) = 0;

    virtual ~ISampledDimension() {}

};


/**
 * TODO documentation
 */
class ISetDimension : virtual public IDimension {

public:

    virtual std::vector<std::string> labels() const = 0;

    virtual void labels(const std::vector<std::string> &labels) = 0;

    virtual ~ISetDimension() {}

};


/**
 * TODO documentation
 */
class IRangeDimension : virtual public IDimension {

public:

    virtual std::string label() const = 0;

    virtual void label(const std::string &label) = 0;

    virtual std::string unit() const = 0;

    virtual void unit(const std::string &unit) = 0;

    virtual std::vector<double> tics() const = 0;

    virtual void tics(const std::vector<double> &tics) = 0;

    virtual ~IRangeDimension() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_DIMENSIONS_H
