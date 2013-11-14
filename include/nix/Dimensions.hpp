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
#include <nix/IDimensions.hpp>

namespace nix {


/**
 * TODO documentation
 */
class Dimension : public virtual IDimension, public base::ImplContainer<IDimension> {

public:

    size_t id() const {
        return impl_ptr->id();
    }

    DimensionType dimensionType() const {
        return impl_ptr->dimensionType();
    }

};


/**
 * TODO documentation
 */
class SampledDimension : public virtual ISampledDimension, public base::ImplContainer<ISampledDimension> {

public:

    SampledDimension()
        : ImplContainer()
    {
    }

    SampledDimension(const std::shared_ptr<ISampledDimension> &p_impl)
        : ImplContainer(p_impl)
    {
    }

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


    double offset() const {
        return impl_ptr->offset();
    }


    void offset(double offset) {
        return impl_ptr->offset(offset);
    }

};


/**
 * TODO documentation
 */
class SetDimension : virtual public ISetDimension, public base::ImplContainer<ISetDimension> {

public:

    SetDimension()
        : ImplContainer()
    {
    }

    SetDimension(const std::shared_ptr<SetDimension> &p_impl)
        : ImplContainer(p_impl)
    {
    }

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

};


/**
 * TODO documentation
 */
class RangeDimension : virtual public IRangeDimension, public base::ImplContainer<IRangeDimension> {

public:

    RangeDimension()
        : ImplContainer()
    {
    }

    RangeDimension(const std::shared_ptr<IRangeDimension> &p_impl)
        : ImplContainer(p_impl)
    {
    }


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

};


} // namespace nix

#endif // NIX_DIMENSIONS_H
