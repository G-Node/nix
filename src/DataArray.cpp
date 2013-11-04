// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file DataArray.cpp
 * @brief Implementations of the class DataArray.
 */

#include <pandora/DataSet.hpp>
#include <pandora/DataArray.hpp>

using namespace std;

namespace pandora {


const PSize DataArray::MIN_CHUNK_SIZE = {1};
const PSize DataArray::MAX_SIZE_1D = {H5S_UNLIMITED};


DataArray::DataArray(const DataArray &data_array)
: EntityWithSources(data_array.file, data_array.block, data_array.group, data_array.entity_id)
{
  dimension_group = data_array.dimension_group;
  vector<double> pcs = data_array.polynomCoefficients();
  polynomCoefficients(pcs);
  expansionOrigin(data_array.expansionOrigin());
}


DataArray::DataArray(File file, const Block block, Group group, const string &id)
: EntityWithSources(file, block, group, id)
{
  dimension_group = group.openGroup("dimensions");

  if (!group.hasAttr("polynom_coefficients")) {
    vector<double> pc = {0,1};
    polynomCoefficients(pc);
  }

  if (!group.hasAttr("expansion_origin")) {
    expansionOrigin(0);
  }
}


DataArray::DataArray(File file, const Block block, Group group, const string &id, time_t time)
: EntityWithSources(file, block, group, id, time)
{
  dimension_group = group.openGroup("dimensions");

  if (!group.hasAttr("polynom_coefficients")) {
    vector<double> pc = {0,1};
    polynomCoefficients(pc);
  }

  if (!group.hasAttr("expansion_origin")) {
    expansionOrigin(0);
  }
}


string DataArray::label() const {
  string value;
  group.getAttr("label", value);
  return value;
}


void DataArray::label(const string &label) {
  group.setAttr("label", label);
  forceUpdatedAt();
}

string DataArray::unit() const {
  string value;
  group.getAttr("unit", value);
  return value;
}

void DataArray::unit(const string &unit) {
  group.setAttr("unit", unit);
  forceUpdatedAt();
}


double DataArray::expansionOrigin() const {
  double expansion_origin;
  group.getAttr("expansion_origin", expansion_origin);
  return expansion_origin;
}


void DataArray::expansionOrigin(double expansion_origin) {
  group.setAttr("expansion_origin", expansion_origin);
  forceUpdatedAt();
}


vector<double> DataArray::polynomCoefficients()const{
  vector<double> polynom_coefficients;

  if (group.hasData("polynom_coefficients")) {
    DataSet ds = group.openData("polynom_coefficients");
    ds.read(polynom_coefficients, true);
  }

  return polynom_coefficients;
}


void DataArray::polynomCoefficients(vector<double> &coefficients){
  if (group.hasData("polynom_coefficients")) {
    DataSet ds = group.openData("polynom_coefficients");
    ds.extend({coefficients.size()});
    ds.write(coefficients);
  } else {
    DataSet ds = DataSet::create(group.h5Group(), "polynom_coefficients", coefficients, &MAX_SIZE_1D, &MIN_CHUNK_SIZE);
    ds.write(coefficients);
  }
  forceUpdatedAt();
}


double DataArray::applyPolynomial(std::vector<double> &coefficients, double origin, double input) const{
  double value = 0.0;
  double term = 1.0;
  for(size_t i = 0; i < coefficients.size(); i++){
    value += coefficients[i] * term;
    term *= input - origin;
  }
  return value;
}


DataArray& DataArray::operator=(const DataArray &other) {
  if (*this != other) {
    this->file = other.file;
    this->block = other.block;
    this->group = other.group;
    this->entity_id = other.entity_id;
    this->dimension_group = other.dimension_group;
    this->sources_refs = other.sources_refs;
  }
  return *this;
}


ostream& operator<<(ostream &out, const DataArray &ent) {
  out << "DataArray: {name = " << ent.name();
  out << ", type = " << ent.type();
  out << ", id = " << ent.id() << "}";
  return out;
}


DataArray::~DataArray(){
  //dtor
}

} //namespace pandora
