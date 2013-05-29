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

#include <string>

#include <pandora/Group.hpp>
#include <pandora/File.hpp>
#include <pandora/PandoraEntity.hpp>

#ifndef PAN_SIMPLETAG_H_INCLUDED
#define PAN_SIMPLETAG_H_INCLUDED

namespace pandora {


class DataArray;
class SimpleTagIterator;

/**
 * Class that represents a pandora tag.
 */
class SimpleTag : public PandoraEntity {


public:

  /**
   * Copy constructor
   */
  SimpleTag(const SimpleTag &tag);

  /**
   * Standard constructor
   */
  SimpleTag(File *file, Group group, std::string id);

  /**
   * Destructor.
   */
  virtual ~SimpleTag();
};


} /* namespace pandora */

#endif /* PAN_SIMPLETAG_H_INCLUDED */
