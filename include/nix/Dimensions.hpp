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
#include <nix/Exception.hpp>

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
class NIXAPI Dimension : public virtual base::IDimension, public base::ImplContainer<base::IDimension> {

public:

    Dimension();


    Dimension(const std::shared_ptr<base::IDimension> &p_impl);


    Dimension(const Dimension &other);


    Dimension(const SampledDimension &other);


    Dimension(const RangeDimension &other);


    Dimension(const SetDimension &other);


    size_t id() const {
        return backend()->id();
    }


    DimensionType dimensionType() const {
        return backend()->dimensionType();
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
class NIXAPI SampledDimension : public virtual base::ISampledDimension, public base::ImplContainer<base::ISampledDimension> {

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
        return backend()->id();
    }

    /**
     * Returns the type of the dimension entity.
     *
     * @return DimensionType either Sample, Range, or Set.
     */
    DimensionType dimensionType() const {
        return backend()->dimensionType();
    }

    /**
     * Returns the label of the dimension.
     *
     * @return string the label.
     */
    boost::optional<std::string> label() const {
        return backend()->label();
    }

    /**
     * Set the label of the dimension.
     * -obligatory-
     *
     * @param label string
     */
    void label(const std::string &label) {
        backend()->label(label);
    }    
    
    /**
     * Deleter for the label attribute.
     *
     * @param boost::none_t.
     */
    void label(const none_t t)
    {
        backend()->label(t);
    }

    /**
     * Returns the unit in which sampling_interval and offset are given.
     *
     * @return string the unit
     */
    boost::optional<std::string> unit() const {
        return backend()->unit();
    }

    /**
     * Sets the unit in which sampling_interval and offset are given.
     * -obligatory-
     *
     * @param unit, string
     */
    void unit(const std::string &unit) {
        //if (!(util::isSIUnit(unit) || util::isCompoundSIUnit(unit))) { TODO support compuond SI units.
        if (!(util::isSIUnit(unit))) {
            throw InvalidUnit("Unit is not a SI unit. Note: so far, only atomic SI units are supported.", "SampledDimension::unit(const string &unit)");
        }
        backend()->unit(unit);
    }
    
    /**
     * Deleter for the label attribute.
     *
     * @param boost::none_t.
     */
    void unit(const none_t t)
    {
        backend()->unit(t);
    }
    
    /**
     * Returns the sampling interval in which the dimension has been sampled.
     *
     * @return double, the sampling interval
     */
    double samplingInterval() const {
       return backend()->samplingInterval();
    }

    /**
     * Set the sampling interval in which the dimension has been sampled. Value must be
     * larger than 0.0!
     * -obligatory-
     *
     * @param interval double
     */
    void samplingInterval(double interval) {
        if(interval <= 0.0) {
            throw std::runtime_error("SampledDimenion::samplingInterval: Sampling intervals must be larger than 0.0!");
        }
        backend()->samplingInterval(interval);
    }

    /**
     * Returns the offset of this dimension. This property is optional. Thus,
     * this return value may be empty.
     *
     * @return boost::optional<double> the offset
     */
    boost::optional<double> offset() const {
        return backend()->offset();
    }

    /**
     * Set the offset of this dimension. The offset is interpreted using the same
     * unit as the sampling interval.
     * -optional-
     *
     * @param offset double
     */
    void offset(double offset) {
        return backend()->offset(offset);
    }

    /**
     * Deleter for the offset of this dimension.
     * -optional-
     *
     * @param boost::none_t
     */
    void offset(const boost::none_t t) {
        backend()->offset(t);
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
class NIXAPI SetDimension : virtual public base::ISetDimension, public base::ImplContainer<base::ISetDimension> {

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
        return backend()->id();
    }

    /**
     * Returns the type of the dimension entity.
     *
     * @return DimensionType either Sample, Range, or Set.
     */
    DimensionType dimensionType() const {
        return backend()->dimensionType();
    }

    /**
     * Returns the set of labels stored in this SetDimension.
     *
     * @return vector<string> the labels
     */
    std::vector<std::string> labels() const {
        return backend()->labels();
    }

    /**
     * Sets the labels stored in this SetDimension.
     * -obligatory-
     *
     * @param labels, vector<string>
     */
    void labels(const std::vector<std::string> &labels) {
        backend()->labels(labels);
    }

    /**
     * Deleter for the labels of this dimension.
     * -optional-
     *
     * @param boost::none_t
     */
    void labels(const boost::none_t t) {
        backend()->labels(t);
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
class NIXAPI RangeDimension : virtual public base::IRangeDimension, public base::ImplContainer<base::IRangeDimension> {

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
        return backend()->id();
    }

    /**
     * Returns the type of the dimension entity.
     *
     * @return DimensionType either Sample, Range, or Set.
     */
    DimensionType dimensionType() const {
        return backend()->dimensionType();
    }

    /**
     * Returns the label stored in this dimension.
     *
     * @return string the label
     */
     boost::optional<std::string> label() const {
        return backend()->label();
    }

    /**
     * Sets the label of the dimension.
     * -obligatory-
     *
     * @param label string
     */
    void label(const std::string &label) {
        backend()->label(label);
    }
    
    /**
     * Deleter for the label attribute.
     *
     * @param boost::none_t.
     */
    void label(const none_t t)
    {
        backend()->label(t);
    }
    
    /**
     * Returns the unit of this RangeDimension.
     *
     * @return string the unit.
     */
    boost::optional<std::string> unit() const {
        return backend()->unit();
    }

    /**
     * Sets the unit of this RangeDimension. So far, only SI units are
     * permitted.
     * -obligatory-
     *
     * @param unit string
     */
    void unit(const std::string &unit) {
        //if (!(util::isSIUnit(unit) || util::isCompoundSIUnit(unit))) {
        if (!(util::isSIUnit(unit))) {
            throw InvalidUnit("Unit is not an atomic SI. Note: So far composite units are not supported", "RangeDimension::unit(const string &unit)");
        }
        backend()->unit(unit);
    }
    
    /**
     * Deleter for the unit attribute.
     *
     * @param boost::none_t.
     */
    void unit(const none_t t)
    {
        backend()->unit(t);
    }
    
    /**
     * Retruns the set of ticks stored in this RangeDimension.
     *
     * @return vector<double> the ticks.
     */
    std::vector<double> ticks() const {
        return backend()->ticks();
    }

    /**
     * Sets the ticks of the RangeDimension. Ticks must be ordered
     * in ascending order.
     * -obligatory-
     *
     * @param ticks vector<double>
     */
    void ticks(const std::vector<double> &ticks) {
        if (!std::is_sorted(ticks.begin(), ticks.end())) {
            std::string caller = "Range::ticks()";
            throw UnsortedTicks(caller);
        }
        backend()->ticks(ticks);
    }


    RangeDimension& operator=(const RangeDimension &other);


    RangeDimension& operator=(const Dimension &other);

};


} // namespace nix

#endif // NIX_DIMENSIONS_H
