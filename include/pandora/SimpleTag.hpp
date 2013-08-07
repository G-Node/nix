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
#include <vector>

#include <pandora/Group.hpp>
#include <pandora/File.hpp>
#include <pandora/Block.hpp>
#include <pandora/PandoraEntity.hpp>

#ifndef PAN_SIMPLETAG_H_INCLUDED
#define PAN_SIMPLETAG_H_INCLUDED

namespace pandora {

class Section;
class Source;
class DataArray;
class Representation;

/**
 * Class that represents a pandora tag.
 */
class SimpleTag : public PandoraEntity {

private:

  const Block block;

public:

  /**
   * Copy constructor
   */
  SimpleTag(const SimpleTag &tag);

  /**
   * Standard constructor
   */
  SimpleTag(File *file, const Block block, Group group, std::string id);

  std::string type() const;
  void type(const std::string &type);

  std::string name() const;
  void name(const std::string &name);

  std::string definition() const;
  void definition(const std::string &definition);

  Section metadata() const;
  void metadata(const Section &metadata);

  std::vector<Source> sources() const;
  void sources(const std::vector<Source> &sources);

  std::string unit() const;
  void unit(const std::string &unit);

  std::vector<double> position() const;
  void position(const std::vector<double> &position);

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
