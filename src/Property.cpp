// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/Property.hpp>

#include <nix/util/util.hpp>

namespace nix {

void Property::unit(const std::string &unit) {
    util::deblankString(unit);
    if (!(util::isSIUnit(unit) || util::isCompoundSIUnit(unit))) {
        throw InvalidUnit("Unit is not SI or composite of SI units.", "Property::unit(const string &unit)");
    }
    backend()->unit(unit);
}

std::ostream& operator<<(std::ostream &out, const Property &ent) {
    out << "Property: {name = " << ent.name() << "}";
    return out;
}

} // namespace nix

