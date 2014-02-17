// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_DIMENSIONS_H
#define NIX_DIMENSIONS_H

#include <nix/base/ImplContainer.hpp>
#include <nix/base/IDimensions.hpp>

namespace nix {


class SampledDimension;
class RangeDimension;
class SetDimension;

/**
 * TODO documentation
 */
class Dimension : public virtual base::IDimension, public base::ImplContainer<base::IDimension> {

public:

    Dimension();


    Dimension(const std::shared_ptr<base::IDimension> &p_impl);


    Dimension(const Dimension &other);


    Dimension(const SampledDimension &other);


    Dimension(const RangeDimension &other);


    Dimension(const SetDimension &other);


    size_t id() const {
        return impl_ptr->id();
    }


    DimensionType dimensionType() const {
        return impl_ptr->dimensionType();
    }


    Dimension& operator=(const SampledDimension &other);


    Dimension& operator=(const RangeDimension &other);


    Dimension& operator=(const SetDimension &other);

};


/**
 * TODO documentation
 */
class SampledDimension : public virtual base::ISampledDimension, public base::ImplContainer<base::ISampledDimension> {

public:

    SampledDimension();


    SampledDimension(const std::shared_ptr<base::ISampledDimension> &p_impl);


    SampledDimension(const SampledDimension &other);


    size_t id() const {
        return impl_ptr->id();
    }


    DimensionType dimensionType() const {
        return impl_ptr->dimensionType();
    }


    std::string label() const {
        return impl_ptr->label();
    }


    void label(const std::string &label) {
        impl_ptr->label(label);
    }


    std::string unit() const {
        return impl_ptr->unit();
    }


    void unit(const std::string &unit) {
        impl_ptr->unit(unit);
    }


    double samplingInterval() const {
       return impl_ptr->samplingInterval();
    }


    void samplingInterval(double interval) {
        impl_ptr->samplingInterval(interval);
    }


    boost::optional<double> offset() const {
        return impl_ptr->offset();
    }


    void offset(double offset) {
        return impl_ptr->offset(offset);
    }


    SampledDimension& operator=(const SampledDimension &other);


    SampledDimension& operator=(const Dimension &other);

};


/**
 * TODO documentation
 */
class SetDimension : virtual public base::ISetDimension, public base::ImplContainer<base::ISetDimension> {

public:

    SetDimension();


    SetDimension(const std::shared_ptr<base::ISetDimension> &p_impl);


    SetDimension(const SetDimension &other);


    size_t id() const {
        return impl_ptr->id();
    }


    DimensionType dimensionType() const {
        return impl_ptr->dimensionType();
    }


    std::vector<std::string> labels() const {
        return impl_ptr->labels();
    }


    void labels(const std::vector<std::string> &labels) {
        impl_ptr->labels(labels);
    }


    SetDimension& operator=(const SetDimension &other);


    SetDimension& operator=(const Dimension &other);

};


/**
 * TODO documentation
 */
class RangeDimension : virtual public base::IRangeDimension, public base::ImplContainer<base::IRangeDimension> {

public:

    RangeDimension();


    RangeDimension(const std::shared_ptr<base::IRangeDimension> &p_impl);


    RangeDimension(const RangeDimension &other);


    size_t id() const {
        return impl_ptr->id();
    }


    DimensionType dimensionType() const {
        return impl_ptr->dimensionType();
    }


    std::string label() const {
        return impl_ptr->label();
    }


    void label(const std::string &label) {
        impl_ptr->label(label);
    }


    std::string unit() const {
        return impl_ptr->unit();
    }


    void unit(const std::string &unit) {
        impl_ptr->unit(unit);
    }


    std::vector<double> tics() const {
        return impl_ptr->tics();
    }


    void tics(const std::vector<double> &tics) {
        impl_ptr->tics(tics);
    }


    RangeDimension& operator=(const RangeDimension &other);


    RangeDimension& operator=(const Dimension &other);

};


} // namespace nix

#endif // NIX_DIMENSIONS_H
