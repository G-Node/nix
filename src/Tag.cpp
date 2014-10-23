// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include <nix/util/dataAccess.hpp>
#include <nix/Tag.hpp>

using namespace std;
using namespace nix;


void Tag::units(const std::vector<std::string> &units) {
    std::vector<std::string> sanitized;
    sanitized.reserve(units.size());
    std::transform(begin(units), end(units), std::back_inserter(sanitized), [](const std::string &x) {
        std::string unit = util::unitSanitizer(x);
        if (unit.length() > 0 && (unit != "none" && !(util::isSIUnit(unit)))) {
            std::string msg = "Unit " + unit +" is not a SI unit. Note: so far only atomic SI units are supported.";
            throw InvalidUnit(msg, "Tag::units(vector<string> &units)");
        }
        return unit;
    });
    backend()->units(sanitized);
}


bool Tag::hasReference(const DataArray &reference) const {
    if (reference == none) {
        throw std::runtime_error("Tag::hasReference: Emty DataArray entity given!");
    }
    return backend()->hasReference(reference.id());
}


void Tag::addReference(const DataArray &reference) {
    if (reference == none) {
        throw std::runtime_error("Tag::addReference: Empty DataArray entity given!");
    }
    backend()->addReference(reference.id());
}


bool Tag::removeReference(const DataArray &reference) {
    if (reference == none) {
        throw std::runtime_error("Tag::removeReference: Empty DataArray entity given!");
    }
    return backend()->removeReference(reference.id());
}


std::vector<DataArray> Tag::references(const util::Filter<DataArray>::type &filter) const {
    auto f = [this] (size_t i) { return getReference(i); };
    return getEntities<DataArray>(f,
                                  referenceCount(),
                                  filter);
}

bool Tag::hasFeature(const Feature &feature) const {
    if (feature == none) {
        throw std::runtime_error("Tag::hasFeature: Empty DataArray entity given!");
    }
    return backend()->hasFeature(feature.id());
}


std::vector<Feature> Tag::features(const util::Filter<Feature>::type &filter) const {
    auto f = [this] (size_t i) { return getFeature(i); };
    return getEntities<Feature>(f,
                                featureCount(),
                                filter);
}


Feature Tag::createFeature(const DataArray &data, LinkType link_type) {
    if (data == none) {
        throw std::runtime_error("Tag::createFeature: Empty DataArray entity given!");
    }
    return backend()->createFeature(data.id(), link_type);
}


bool Tag::deleteFeature(const Feature &feature) {
    if (feature == none) {
        throw std::runtime_error("Tag::deleteFeature: Empty Feature entity given!");
    }
    return backend()->deleteFeature(feature.id());
}

ZonedIO Tag::retrieveData(size_t reference_index) const {
    return util::retrieveData(*this, reference_index);
}

ZonedIO Tag::retrieveFeatureData(size_t feature_index) const {
    return util::retrieveFeatureData(*this, feature_index);
}

std::ostream &nix::operator<<(std::ostream &out, const Tag &ent) {
    out << "Tag: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}

