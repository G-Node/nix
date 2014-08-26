// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include <nix/MultiTag.hpp>

namespace nix {


void MultiTag::positions(const DataArray &positions) {
    if (positions == none) {
        throw std::runtime_error("Empty positions entity (DataArray) given");
    }
    else {
        backend()->positions(positions.id());
    }
}


void MultiTag::extents(const DataArray &extents) {
    if (extents == none) {
        backend()->extents(none);
    }
    else {
        backend()->extents(extents.id());
    }
}


void MultiTag::units(const std::vector<std::string> &units) {
    std::vector<std::string> sanitized;
    sanitized.reserve(units.size());
    std::transform(begin(units), end(units), std::back_inserter(sanitized), [](const std::string &x) {
        std::string unit = util::unitSanitizer(x);
        if (unit.length() > 0 && (unit != "none" && !(util::isSIUnit(unit)))) {
            std::string msg = "Unit " + unit +" is not a SI unit. Note: so far only atomic SI units are supported.";
            throw InvalidUnit(msg, "MultiTag::units(vector<string> &units)");
        }
        return unit;
    });
    backend()->units(sanitized);
}


bool MultiTag::hasReference(const DataArray &reference) const {
    if (reference == none) {
        throw std::runtime_error("MultiTag::hasReference: Empty DataArray entity given!");
    }
    return backend()->hasReference(reference.id());
}


void MultiTag::addReference(const DataArray &reference) {
    if (reference == none) {
        throw std::runtime_error("MultiTag::addReference: Empty DataArray entity given!");
    }
    backend()->addReference(reference.id());
}


bool MultiTag::removeReference(const DataArray &reference) {
    if (reference == none) {
        throw std::runtime_error("MultiTag::removeReference: Empty DataArray reference given!");
    }
    return backend()->removeReference(reference.id());
}


std::vector<DataArray> MultiTag::references(const util::Filter<DataArray>::type &filter) const
{
    auto f = [this] (size_t i) { return getReference(i); };
    return getEntities<DataArray>(f,
                                  referenceCount(),
                                  filter);
}


bool MultiTag::hasFeature(const Feature &feature) const {
    if (feature == none) {
        throw std::runtime_error("MultiTag::hasFeature: Empty feature given!");
    }
    return backend()->hasFeature(feature.id());
}


std::vector<Feature> MultiTag::features(const util::Filter<Feature>::type &filter) const
{
    auto f = [this] (size_t i) { return getFeature(i); };
    return getEntities<Feature>(f,
                                featureCount(),
                                filter);
}


bool MultiTag::deleteFeature(const Feature &feature) {
    if (feature == none) {
        throw std::runtime_error("MultiTag::deleteFeature: Empty Feature entity given!");
    }
    return backend()->deleteFeature(feature.id());
}


std::ostream& operator<<(std::ostream &out, const MultiTag &ent) {
    out << "MultiTag: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}

} // namespace nix
