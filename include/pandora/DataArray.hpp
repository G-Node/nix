// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Authors: Christian Kellner <kellner@bio.lmu.de>, Jan Grewe <jan.grewe@g-node.org>


#include <boost/multi_array.hpp>

#include <pandora/PSize.hpp>
#include <pandora/EntityWithSources.hpp>
#include <pandora/DataSet.hpp>


#ifndef PAN_DATA_ARRAY_H_INCLUDED
#define PAN_DATA_ARRAY_H_INCLUDED

namespace pandora {

class Dimension;


class DataArray : public EntityWithSources {

  static const PSize MIN_CHUNK_SIZE;
  static const PSize MAX_SIZE_1D;

  Group dimension_group;

public:

  /**
   * Copy constructor
   */
  DataArray(const DataArray &tag);

  /**
   * Standard constructor
   */
  DataArray(File file, Block block, Group group, std::string id);

  /**
   * Standard constructor that preserves the creation time.
   */
  DataArray(File file, Block block, Group group, std::string id, time_t time);

  //--------------------------------------------------
  // Element getters and setters
  //--------------------------------------------------

  std::string label() const;
  void label(const std::string &value);

  std::string unit() const;
  void unit(const std::string &value);

  double expansionOrigin()const;
  void expansionOrigin(double expansion_origin);

  void polynomCoefficients(std::vector<double> &polynom_coefficients);
  std::vector<double> polynomCoefficients() const;

  //--------------------------------------------------
  // Methods concerning data access.
  //--------------------------------------------------


  template<typename T, size_t dims>
  void getData(boost::multi_array<T, dims> &data) const;

  template<typename T, size_t dims>
  void setData(const boost::multi_array<T, dims> &data);

  template<typename T, size_t dims>
  void getRawData(boost::multi_array<T, dims> &data) const{
	  DataSet ds = group.openData("data");
	  ds.read(data, true);
  }

  template<typename T, size_t dims>
  void setRawData(const boost::multi_array<T, dims> &data){
	  if (!group.hasData("data")){
		  PSize size = {dims};
		  PSize maxsize = {H5S_UNLIMITED};
		  PSize chunks = {1};
		  DataSet ds(DataSet::create(group.h5Group(), "data", data, &maxsize, &chunks));
		  ds.write(data);
	  }
	  else{
		  DataSet ds = group.openData("data");
		  ds.write(data);
	  }
  }


  //--------------------------------------------------
  // Methods concerning dimensions
  // TODO figure out how dimension access should work regarding different dimension types.
  //--------------------------------------------------


  //--------------------------------------------------
  // Other methods and functions
  //--------------------------------------------------

  /**
   * Assignment operator
   */
  DataArray& operator=(const DataArray &other);


  /**
   * Output operator
   */
  friend std::ostream& operator<<(std::ostream &out, const DataArray &ent);


  ~DataArray();
};


} //namespace pandora

#endif // PAN_DATA_ARRAY_H_INCLUDED
