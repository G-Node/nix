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
 * Instances of the Dimension subclasses are used to define the axes of a DataArray.
 * There are  three subclasses:
 *     - RangeDimension
 *     - SampledDimension
 *     - SetDimension
 *
 * See detailed descriptions below.
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
 * Instances of the SampledDimension Class are used to describe a dimension of a DataArray that
 * has been sampled in regular intervals. For example this can be a time axis.
 *
 * SampledDimensions are characterized by the label of the dimension, the unit in which the sampling_interval
 * is given. If not otherwise stated the dimension starts with zero offset.
 */
class SampledDimension : public virtual base::ISampledDimension, public base::ImplContainer<base::ISampledDimension> {

public:

    SampledDimension();


    SampledDimension(const std::shared_ptr<base::ISampledDimension> &p_impl);


    SampledDimension(const SampledDimension &other);

    /**
     * Returns the id of the dimension entity which at the same time is
     * the order of the dimension. That is the dimension of the actual data that
     * is defined by this SampledDimension.
     *
     * @return size_t, the id, i.e. the order.
     */
    size_t id() const {
        return impl_ptr->id();
    }

    /**
     * Returns the type of the dimension entity.
     *
     * @return DimensionType either Sample, Range, or Set.
     */
    DimensionType dimensionType() const {
        return impl_ptr->dimensionType();
    }

    /**
     * Returns the label of the dimension.
     *
     * @return string the label.
     */
    std::string label() const {
        return impl_ptr->label();
    }

    /**
     * Set the label of the dimension.
     * -obligatory-
     *
     * @param label string
     */
    void label(const std::string &label) {
        impl_ptr->label(label);
    }

    /**
     * Returns the unit in which sampling_interval and offset are given.
     *
     * @return string the unit
     */
    std::string unit() const {
        return impl_ptr->unit();
    }

    /**
     * Sets the unit in which sampling_interval and offset are given.
     * -obligatory-
     *
     * @param unit, string
     */
    void unit(const std::string &unit) {
        impl_ptr->unit(unit);
    }

    /**
     * Returns the sampling interval in which the dimension has been sampled.
     *
     * @return double, the sampling interval
     */
    double samplingInterval() const {
       return impl_ptr->samplingInterval();
    }

    /**
     * Set the sampling interval in which the dimension has been sampled.
     * -obligatory-
     *
     * @param interval double
     */
    void samplingInterval(double interval) {
        impl_ptr->samplingInterval(interval);
    }

    /**
     * Returns the offset of this dimension. This property is optional. Thus,
     * this return value may be empty.
     *
     * @return boost::optional<double> the offset
     */
    boost::optional<double> offset() const {
        return impl_ptr->offset();
    }

    /**
     * Set the offset of this dimension. The offset is interpreted using the same
     * unit as the sampling interval.
     * -optional-
     *
     * @param offset double
     */
    void offset(double offset) {
        return impl_ptr->offset(offset);
    }

    /**
     * Deleter for the offset of this dimension.
     * -optional-
     *
     * @param boost::none_t
     */
    void offset(const boost::none_t t) {
        impl_ptr->offset(t);
    }

    SampledDimension& operator=(const SampledDimension &other);


    SampledDimension& operator=(const Dimension &other);

};


/**
 * Instances of the SetDimension Class are used to describe a dimension of a DataArray that
 * are sampled on a nominal scale or in categories. For example, the parallel channels of a
 * recording device can be defined with a SetDimension.
 *
 * SetDimensions are characterized by a set of labels.
 */
class SetDimension : virtual public base::ISetDimension, public base::ImplContainer<base::ISetDimension> {

public:

    SetDimension();


    SetDimension(const std::shared_ptr<base::ISetDimension> &p_impl);


    SetDimension(const SetDimension &other);


    /**
    * Returns the id of the dimension entity which at the same time is
    * the order of the dimension. That is the dimension of the actual data that
    * is defined by this SetDimension.
    *
    * @return size_t, the id, i.e. the order.
    */
    size_t id() const {
        return impl_ptr->id();
    }

    /**
     * Returns the type of the dimension entity.
     *
     * @return DimensionType either Sample, Range, or Set.
     */
    DimensionType dimensionType() const {
        return impl_ptr->dimensionType();
    }

    /**
     * Returns the set of labels stored in this SetDimension.
     *
     * @return vector<string> the labels
     */
    std::vector<std::string> labels() const {
        return impl_ptr->labels();
    }

    /**
     * Sets the labels stored in this SetDimension.
     * -obligatory-
     *
     * @param labels, vector<string>
     */
    void labels(const std::vector<std::string> &labels) {
        impl_ptr->labels(labels);
    }

    /**
     * Deleter for the labels of this dimension.
     * -optional-
     *
     * @param boost::none_t
     */
    void labels(const boost::none_t t) {
        impl_ptr->labels(t);
    }

    SetDimension& operator=(const SetDimension &other);


    SetDimension& operator=(const Dimension &other);

};


/**
 * Instances of the RangeDimension Class are used to describe a dimension of a DataArray that
 * are sampled in irregular intervals. For example, irregularly occurring events that have been
 * detected in a recorded signal.
 *
 * RangeDimensions are characterized by an array of tics that are given in a certain unit and
 * must be given in an ascending order. The dimension has also a label.
 */
class RangeDimension : virtual public base::IRangeDimension, public base::ImplContainer<base::IRangeDimension> {

public:

    RangeDimension();


    RangeDimension(const std::shared_ptr<base::IRangeDimension> &p_impl);


    RangeDimension(const RangeDimension &other);

    /**
     * Returns the id of the dimension entity which, at the same time, is
     * the order of the dimension. That is the dimension of the actual data that
     * is defined by this RangeDimension.
     *
     * @return size_t, the id, i.e. the order.
     */
    size_t id() const {
        return impl_ptr->id();
    }

    /**
     * Returns the type of the dimension entity.
     *
     * @return DimensionType either Sample, Range, or Set.
     */
    DimensionType dimensionType() const {
        return impl_ptr->dimensionType();
    }

    /**
     * Returns the label stored in this dimension.
     *
     * @return string the label
     */
    std::string label() const {
        return impl_ptr->label();
    }

    /**
     * Sets the label of the dimension.
     * -obligatory-
     *
     * @param label string
     */
    void label(const std::string &label) {
        impl_ptr->label(label);
    }

    /**
     * Returns the unit of this RangeDimension.
     *
     * @return string the unit.
     */
    std::string unit() const {
        return impl_ptr->unit();
    }

    /**
     * Sets the unit of this RangeDimension. So far, only SI units are
     * permitted.
     * -obligatory-
     *
     * @param unit string
     */
    void unit(const std::string &unit) {
        impl_ptr->unit(unit);
    }

    /**
     * Retruns the set of tics stored in this RangeDimension.
     *
     * @return vector<double> the tics.
     */
    std::vector<double> tics() const {
        return impl_ptr->tics();
    }

    /**
     * Sets the tics of the RangeDimension. Tics must be ordered
     * in ascending order.
     * -obligatory-
     *
     * @param tics vector<double>
     */
    void tics(const std::vector<double> &tics) {
        impl_ptr->tics(tics);
    }


    RangeDimension& operator=(const RangeDimension &other);


    RangeDimension& operator=(const Dimension &other);

};


} // namespace nix

#endif // NIX_DIMENSIONS_H
