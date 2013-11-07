// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Authors: Christian Kellner <kellner@bio.lmu.de>, Jan Grewe <jan.grewe@g-node.org>

//TODO convenience methods for accessing dimensionality and shape of data

#include <memory>
#include <boost/multi_array.hpp>

#include <pandora/DataSet.hpp>
#include <pandora/Dimension.hpp>
#include <pandora/EntityWithSources.hpp>

#ifndef PAN_DATA_ARRAY_H_INCLUDED
#define PAN_DATA_ARRAY_H_INCLUDED

namespace pandora {


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
  DataArray(File file, Block block, Group group, const std::string &id);

  /**
   * Standard constructor that preserves the creation time.
   */
  DataArray(File file, Block block, Group group, const std::string &id, time_t time);

  //--------------------------------------------------
  // Element getters and setters
  //--------------------------------------------------

  /**
   * Get the label for the values stored in the DataArray.
   *
   * @return string the label
   */
  std::string label() const;

  /**
   * Set the label for the data stored.
   *
   * @param string label
   */
  void label(const std::string &label);


  /**
   * Get the unit of the data stored in this dataArray.
   *
   * @return string the unit.
   */
  std::string unit() const;

  /**
   * Set the unit for the values stored in this DataArray.
   *
   * @param string the unit
   */
  void unit(const std::string &unit);


  /**
   * Returns the expansion origin of the calibration polynom.
   * This is set to 0.0 by default.
   *
   * @return double the expansion origin.
   */
  double expansionOrigin()const;

  /**
   * Set the expansion origin for the calibration.
   *
   * @param double the expansion origin.
   */
  void expansionOrigin(double expansion_origin);

  /**
   * Set the polynom coefficients for the calibration. By default this is set
   * to a two element vector of [0.0, 1.0] for a linear calibration with zero offset.
   *
   * @param vector<double> the coefficients
   */
  void polynomCoefficients(std::vector<double> &polynom_coefficients);

  /**
   * Returns the polynom coefficients.
   *
   * @return vector<double> the coefficients.
   */
  std::vector<double> polynomCoefficients() const;

  //--------------------------------------------------
  // Methods concerning data access.
  //--------------------------------------------------

  /**
   * Returns the data stored in the DataArray and applies the calibration.
   * Output is returned in a boost multi_array.
   *
   * @param output argument boost::multi_array<T, dims>
   */
  template<typename T, size_t dims>
  void getData(boost::multi_array<T, dims> &data) const{
    getRawData(data);
    double origin = expansionOrigin();
    std::vector<double> polynoms = polynomCoefficients();
    for(auto i = data.data(); i < (data.data() + data.num_elements()); ++i) {
        *i = applyPolynomial(polynoms, origin, (double)(*i));
    }
  }

  /**
   * Returns the data as it is stored. Does not apply the calibration.
   *
   * @param output argument boost::multi_array<T, dims>
   */
  template<typename T, size_t dims>
  void getRawData(boost::multi_array<T, dims> &data) const{
    if(group.hasData("data")){
      DataSet ds = group.openData("data");
      ds.read(data, true);
    }
  }

  /**
   * Set the rawData that is to be stored in the DataArray.
   *
   * @param boost::multi_array<T, dims>
   */
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
  //--------------------------------------------------

  std::vector<std::shared_ptr<Dimension>> dimensions() const;

  size_t dimensionCount() const;

  std::shared_ptr<Dimension> getDimension(size_t id) const;

  std::shared_ptr<Dimension> createDimension(size_t id, DimensionType type);

  bool removeDimension(size_t id);

  //--------------------------------------------------
  // Other methods and functions
  //--------------------------------------------------

  /**
   * Conversion of the data by applying the calibration.
   */
  double applyPolynomial(std::vector<double> &coefficients, double origin, double input) const;

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
