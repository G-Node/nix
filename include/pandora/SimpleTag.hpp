// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file pandora/SimpleTag.hpp
 * @brief Definition of the class SimpleTag.
 */


#include <pandora/EntityWithSources.hpp>

#ifndef PAN_SIMPLETAG_H_INCLUDED
#define PAN_SIMPLETAG_H_INCLUDED

namespace pandora {


class Section;
class DataArray;
class Representation;

/**
 * Class that represents a pandora tag.
 */
class SimpleTag : public EntityWithSources {


public:

  /**
   * Copy constructor
   */
  SimpleTag(const SimpleTag &tag);

  /**
   * Standard constructor
   */
  SimpleTag(File file, Block block, Group group, std::string id);

  /**
   * Standard constructor that preserves the creation time.
   */
  SimpleTag(File file, Block block, Group group, std::string id, time_t time);

  /**
   * Getter for the units of the tag. The units are applied to all values for position
   * and extent in order to calculate the right position vectors in referenced data arrays.
   *
   * @return All units of the tag as a vector.
   */
  std::vector<std::string> units() const;

  /**
   * Setter for the units of a tag.
   *
   * @param units     All units as a vector.
   */
  void units(const std::vector<std::string> &units);

  std::vector<double> position() const;
  void position(const std::vector<double> &pposition);

  std::vector<double> extent() const;
  void extent(const std::vector<double> &extent);

  std::vector<DataArray> references() const;
  void references(const std::vector<DataArray> &references);

  std::vector<Representation> representations() const;
  void representations(const std::vector<Representation> &representatios);

  /**
   * Destructor.
   */
  virtual ~SimpleTag();
};


} /* namespace pandora */

#endif /* PAN_SIMPLETAG_H_INCLUDED */
