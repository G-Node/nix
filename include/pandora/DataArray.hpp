// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Authors: Christian Kellner <kellner@bio.lmu.de>, Jan Grewe <jan.grewe@g-node.org>

#include <pandora/Group.hpp>
#include <pandora/File.hpp>
#include <pandora/NamedEntityWithSources.hpp>
#include <pandora/DataSet.hpp>
#include <pandora/Charon.hpp>
#include <H5Cpp.h>
#include <string>

#ifndef PANDORA_DATA_ARRAY_H
#define PANDORA_DATA_ARRAY_H

namespace pandora {

class DataArray : public EntityWithSources{

public:
  DataArray(File parentFile, Group thisGroup, std::string identifier);
  DataArray(const DataArray &other);

  std::string label() const;
  void label(const std::string &value);

  std::string unit() const;
  void unit(const std::string &value);

  void expansionOrigin(double expansion_origin = 0.0);
  double expansionOrigin()const;

  int polynomOrder() const;

  void polynomCoefficients(std::vector<double> &polynom_coefficients);
  std::vector<double> polynomCoefficients() const;

  void setCalibration(std::vector<double> &coefficients, double origin = 0.0);

  template<typename T, size_t numDims>
  void setData(boost::multi_array<T,numDims>);
  //template<typename T, size_t numDims>
  //boost::multi_array getData();

  DataSet data();

  ~DataArray();
};


} //namespace pandora

#endif // PANDORA_DATA_ARRAY_H
