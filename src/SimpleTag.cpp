// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include <nix/SimpleTag.hpp>

using namespace std;
using namespace nix;


void SimpleTag::units(const std::vector<std::string> &units) {
    std::vector<std::string> sanitized;
    sanitized.reserve(units.size());
    std::transform(begin(units), end(units), std::back_inserter(sanitized), [](const std::string &x) {
        std::string unit = util::unitSanitizer(x);
        if (unit.length() > 0 && (unit != "none" && !(util::isSIUnit(unit)))) {
            std::string msg = "Unit " + unit +" is not a SI unit. Note: so far only atomic SI units are supported.";
            throw InvalidUnit(msg, "SimpleTag::units(vector<string> &units)");
        }
        return unit;
    });
    backend()->units(sanitized);
}


bool SimpleTag::hasReference(const DataArray &reference) const {
    if (reference == none) {
        throw std::runtime_error("SimpleTag::hasReference: Emty DataArray entity given!");
    }
    return backend()->hasReference(reference.id());
}


void SimpleTag::addReference(const DataArray &reference) {
    if (reference == none) {
        throw std::runtime_error("SimpleTag::addReference: Empty DataArray entity given!");
    }
    backend()->addReference(reference.id());
}


bool SimpleTag::removeReference(const DataArray &reference) {
    if (reference == none) {
        throw std::runtime_error("SimpleTag::removeReference: Empty DataArray entity given!");
    }
    return backend()->removeReference(reference.id());
}


std::vector<DataArray> SimpleTag::references(util::Filter<DataArray>::type filter) const {
    auto f = [this] (size_t i) { return getReference(i); };
    return getEntities<DataArray>(f,
                                  referenceCount(),
                                  filter);
}

bool SimpleTag::hasFeature(const Feature &feature) const {
    if (feature == none) {
        throw std::runtime_error("SimpleTag::hasFeature: Empty DataArray entity given!");
    }
    return backend()->hasFeature(feature.id());
}


std::vector<Feature> SimpleTag::features(util::Filter<Feature>::type filter) const {
    auto f = [this] (size_t i) { return getFeature(i); };
    return getEntities<Feature>(f,
                                featureCount(),
                                filter);
}


Feature SimpleTag::createFeature(const DataArray &data, LinkType link_type) {
    if (data == none) {
        throw std::runtime_error("SimpleTag::createFeature: Empty DataArray entity given!");
    }
    return backend()->createFeature(data.id(), link_type);
}


bool SimpleTag::deleteFeature(const Feature &feature) {
    if (feature == none) {
        throw std::runtime_error("SimpleTag::deleteFeature: Empty Feature entity given!");
    }
    return backend()->deleteFeature(feature.id());
}

std::ostream &nix::operator<<(std::ostream &out, const SimpleTag &ent) {
    out << "SimpleTag: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}

