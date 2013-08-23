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

class DataArray : public NamedEntityWithSources{

public:
  DataArray(File parentFile, Group thisGroup, std::string identifier);
  DataArray(const DataArray &other);

  std::string label() const;
  void label(const std::string &value);

  std::string unit() const;
  void unit(const std::string &value);

  void expansionOrigin(double expansion_origin = 0.0);
  double getExpansionOrigin()const;

  void polynomCoefficients(std::vector<double> &polynom_coefficients);
  std::vector<double> polynomCoefficients() const;

  void setCalibration(std::vector<double> &coefficients, double origin = 0.0);

  double applyPolynom(std::vector<double> &coefficients, double origin, double input) const;

  template<typename T, size_t numDims>
  void setData(const boost::multi_array<T, numDims> &values){
	  if (!group.hasData("data")){
		  PSize size = {numDims};
		  PSize maxsize = {H5S_UNLIMITED};
		  PSize chunks = {1};
		  DataSet ds(DataSet::create(group.h5Group(), "data", values, &maxsize, &chunks));
	  }
	  else{
		  DataSet ds = group.openData("data");
		  ds.write(values);
	  }
  }

  template<typename T, size_t numDims>
  void getData(boost::multi_array<T, numDims> &values, bool convert=false) const{
	  DataSet ds = group.openData("data");
	  ds.read(values, true);
	  if(convert){
		 // boost::multi_array<T, numDims> temp(values);
		 // values.
	  }


  }

  DataSet data();

  ~DataArray();
};


} //namespace pandora

#endif // PANDORA_DATA_ARRAY_H
