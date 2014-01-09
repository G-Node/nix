// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/DataArray.hpp>

namespace nix {

double DataArray::applyPolynomial(std::vector<double> &coefficients, double origin, double input) const{
    double value = 0.0;
    double term = 1.0;
    for(size_t i = 0; i < coefficients.size(); i++){
        value += coefficients[i] * term;
        term *= input - origin;
    }
    return value;
}

}
