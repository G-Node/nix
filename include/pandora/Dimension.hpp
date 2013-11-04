// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file pandora/Dimension.hpp
 * @brief Definition of the class PandoraEntity.
 */

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

class Dimension {

protected:

  Group group;
  size_t dim_id;

public:

  Dimension(Group group, size_t id);

  size_t id() const { return dim_id; }

  virtual DimensionType dimensionType() const = 0;

  virtual void swap(const Dimension &other) = 0;

  bool operator==(const Dimension &other) const;

  bool operator!=(const Dimension &other) const;

  virtual ~Dimension();
};

} /* namespace pandora */

#endif /* PAN_DIMENSION_H_INCLUDED */
