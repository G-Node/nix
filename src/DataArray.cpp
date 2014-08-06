// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include <nix/DataArray.hpp>

using namespace nix;


double DataArray::applyPolynomial(std::vector<double> &coefficients, double origin, double input) const{
    double value = 0.0;
    double term = 1.0;
    for (size_t i = 0; i < coefficients.size(); i++) {
        value += coefficients[i] * term;
        term *= input - origin;
    }
    return value;
}


void DataArray::unit(const std::string &unit) {
    if (!(util::isSIUnit(unit) || util::isCompoundSIUnit(unit))) {
        throw InvalidUnit("Unit is not SI or composite of SI units.", "DataArray::unit(const string &unit)");
    }
    backend()->unit(unit);
}


std::vector<Dimension> DataArray::dimensions(util::Filter<Dimension>::type filter) const {
    auto f = [this] (size_t i) { return getDimension(i+1); }; // +1 since index starts at 1
    return getEntities<Dimension>(f,
                                  dimensionCount(),
                                  filter);
}

std::ostream &operator<<(std::ostream &out, const DataArray &ent) {
    out << "DataArray: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}


std::ostream& nix::operator<<(std::ostream &out, const DataArray &ent) {
    out << "DataArray: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}

