// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/MultiTag.hpp>

#include <nix/util/util.hpp>
#include <nix/util/dataAccess.hpp>


namespace nix {


void MultiTag::positions(const DataArray &positions) {
    if (!util::checkEntityInput(positions)) {
        throw UninitializedEntity();
    }
    backend()->positions(positions.id());
}


void MultiTag::positions(const std::string &name_or_id) {
    util::checkNameOrId(name_or_id);
    backend()->positions(name_or_id);
}


void MultiTag::extents(const DataArray &extents) {
    if (extents == none) {
        backend()->extents(none);
    } else if(!extents.isValidEntity()) {
        throw UninitializedEntity();
    } else {
        backend()->extents(extents.id());
    }
}


void MultiTag::extents(const std::string &name_or_id) {
    util::checkNameOrId(name_or_id);
    backend()->extents(name_or_id);
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
    if(!util::checkEntityInput(reference, false)) {
        return false;
    }
    DataArray da = backend()->getReference(reference.name());
    return da && da.id() == reference.id();
}


DataArray MultiTag::getReference(size_t index) const {
    if(index >= backend()->referenceCount()) {
        throw OutOfBounds("No reference at given index", index);
    }
    return backend()->getReference(index);
}


void MultiTag::addReference(const DataArray &reference) {
    if(!util::checkEntityInput(reference)) {
        throw UninitializedEntity();
    }
    backend()->addReference(reference.name());
}


bool MultiTag::removeReference(const DataArray &reference) {
    if (!util::checkEntityInput(reference)) {
        return false;
    }
    return backend()->removeReference(reference.name());
}


std::vector<DataArray> MultiTag::references(const util::Filter<DataArray>::type &filter) const {
    auto f = [this] (size_t i) { return getReference(i); };
    return getEntities<DataArray>(f, referenceCount(), filter);
}


DataView MultiTag::retrieveData(size_t position_index, size_t reference_index) const {
    return util::retrieveData(*this, position_index, reference_index);
}


DataView MultiTag::retrieveData(size_t position_index, const std::string &name_or_id) const {
    nix::DataArray array = backend()->getReference(name_or_id);
    if (array) {
        return util::retrieveData(*this, position_index, array);
    } else {
        throw std::invalid_argument("There is no DataArray with the specified name or id! Evoked at MultiTag::retrieveData");
    }
}


bool MultiTag::hasFeature(const Feature &feature) const {
    if (!util::checkEntityInput(feature, false)) {
        return false;
    }
    return backend()->hasFeature(feature.id());
}


std::vector<Feature> MultiTag::features(const util::Filter<Feature>::type &filter) const {
    auto f = [this] (size_t i) { return getFeature(i); };
    return getEntities<Feature>(f, featureCount(), filter);
}


bool MultiTag::deleteFeature(const Feature &feature) {
    if (!util::checkEntityInput(feature, false)) {
        return false;
    }
    return backend()->deleteFeature(feature.id());
}


DataView MultiTag::retrieveFeatureData(size_t position_index, size_t feature_index) const {
    return util::retrieveFeatureData(*this, position_index, feature_index);
}


DataView MultiTag::retrieveFeatureData(size_t position_index, const std::string &name_or_id) const {
    nix::Feature feature = backend()->getFeature(name_or_id);
    if (feature) {
        return util::retrieveFeatureData(*this, position_index, feature);
    } else {
        throw std::invalid_argument("There is no Feature with the specified name or id! Evoked at MultiTag::retrieveFeatureData");
    }
}

std::ostream& operator<<(std::ostream &out, const MultiTag &ent) {
    out << "MultiTag: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}

} // namespace nix
