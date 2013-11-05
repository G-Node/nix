// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef PAN_DIMENSION_H_INCLUDED
#define PAN_DIMENSION_H_INCLUDED

#include <string>
#include <iostream>
#include <ctime>

#include <pandora/Util.hpp>
#include <pandora/Group.hpp>


namespace pandora {

enum class DimensionType : int {
  SAMPLED_DIMENSION, SET_DIMENSION, RANGE_DIMENSION
};

/**
 * @brief The Dimension class
 */
class Dimension {

protected:

  Group group;
  size_t dim_id;

public:

  Dimension(Group group, size_t id);

  Dimension(const Dimension &other);

  size_t id() const { return dim_id; }

  virtual DimensionType dimensionType() const = 0;

  virtual void swap(Dimension &other);

  bool operator==(const Dimension &other) const;

  bool operator!=(const Dimension &other) const;

  virtual ~Dimension();
};


/**
 * @brief The SampledDimension class
 */
class SampledDimension : public Dimension {

  SampledDimension(Group group, size_t id);

  SampledDimension(const SampledDimension &other);

  DimensionType dimensionType() const;

  std::string label() const;

  void label(const std::string &label);

  std::string unit() const;

  void unit(const std::string &unit);

  double samplingInterval() const;

  void samplingInterval(double sampling_interval);

  double offset() const;

  void offset(double offset);

  SampledDimension& operator=(const SampledDimension &other);

  virtual ~SampledDimension();
};


/**
 * @brief The SetDimension class
 */
class SetDimension : public Dimension {

  SetDimension(Group group, size_t id);

  SetDimension(const SetDimension &other);

  DimensionType dimensionType() const;

  std::vector<std::string> labels() const;

  void labels(const std::vector<std::string> &labels);

  SetDimension& operator=(const SetDimension &other);

  virtual ~SetDimension();

};


/**
 * @brief The RangeDimension class
 */
class RangeDimension : public Dimension {

  RangeDimension(Group group, size_t id);

  RangeDimension(const RangeDimension &other);

  DimensionType dimensionType() const;

  std::string label() const;

  void label(const std::string &label);

  std::string unit() const;

  void unit(const std::string &unit);

  std::vector<double> tics() const;

  void tics(const std::vector<double> &tics);

  RangeDimension& operator=(const RangeDimension &other);

  virtual ~RangeDimension();
};


} /* namespace pandora */

#endif /* PAN_DIMENSION_H_INCLUDED */
