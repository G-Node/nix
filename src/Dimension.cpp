// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "pandora/Dimension.hpp"

namespace pandora {

Dimension::Dimension(Group group, size_t id)
  : group(group), dim_id(id)
{}

bool Dimension::operator==(const Dimension &other) const {
  return group == other.group && dimensionType() == other.dimensionType() && dim_id == other.dim_id;
}

bool Dimension::operator!=(const Dimension &other) const {
  return group != other.group || dimensionType() != other.dimensionType() || dim_id != other.dim_id;
}

Dimension::~Dimension() {
  // TODO Auto-generated destructor stub
}

} /* namespace pandora */
