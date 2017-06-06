// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/Tag.hpp>

#include <nix/util/util.hpp>
#include <nix/util/dataAccess.hpp>

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
    if (!util::checkEntityInput(reference, false)) {
        return false;
    }
    DataArray da = backend()->getReference(reference.name());
    return da && da.id() == reference.id();
}


DataArray Tag::getReference(size_t index) const {
    if(index >= backend()->referenceCount()) {
        throw OutOfBounds("No reference at given index", index);
    }
    return backend()->getReference(index);
}


void Tag::addReference(const DataArray &reference) {
    if (!util::checkEntityInput(reference, false)) {
        throw UninitializedEntity();
    }
    backend()->addReference(reference.name());
}


void Tag::addReference(const std::string &id) {
    util::checkNameOrId(id);
    backend()->addReference(id);
}


bool Tag::removeReference(const DataArray &reference) {
    if (!util::checkEntityInput(reference, false)) {
        return false;
    }
    return backend()->removeReference(reference.name());
}


std::vector<DataArray> Tag::references(const util::Filter<DataArray>::type &filter) const {
    auto f = [this] (size_t i) { return getReference(i); };
    return getEntities<DataArray>(f, referenceCount(), filter);
}


bool Tag::hasFeature(const Feature &feature) const {
    if (!util::checkEntityInput(feature, false)) {
        return false;
    }
    return backend()->hasFeature(feature.id());
}


std::vector<Feature> Tag::features(const util::Filter<Feature>::type &filter) const {
    auto f = [this] (size_t i) { return getFeature(i); };
    return getEntities<Feature>(f, featureCount(), filter);
}


Feature Tag::getFeature(const std::string &id) const {
    util::checkNameOrId(id);
    return backend()->getFeature(id);
}


Feature Tag::getFeature(ndsize_t index) const {
    if (index >= backend()->featureCount()) {
        throw OutOfBounds("No feature at given index", index);
    }
    return backend()->getFeature(index);
}


Feature Tag::createFeature(const DataArray &data, LinkType link_type) {
    if (!util::checkEntityInput(data)) {
        throw UninitializedEntity();
    }
    return backend()->createFeature(data.id(), link_type);
}


bool Tag::deleteFeature(const Feature &feature) {
    if (!util::checkEntityInput(feature, false)) {
        return false;
    }
    return backend()->deleteFeature(feature.id());
}


DataView Tag::retrieveData(size_t reference_index) const {
    return util::retrieveData(*this, reference_index);
}


DataView Tag::retrieveData(const std::string &name_or_id) const {
    nix::DataArray array = backend()->getReference(name_or_id);
    if (array) {
        return util::retrieveData(*this, array);
    } else {
        throw std::invalid_argument("There is no DataArray with the specified name or id! Evoked at Tag::retrieveData");
    }
}


DataView Tag::retrieveFeatureData(size_t feature_index) const {
    return util::retrieveFeatureData(*this, feature_index);
}


DataView Tag::retrieveFeatureData(const std::string &name_or_id) const {
    nix::Feature feature = backend()->getFeature(name_or_id);
    if (feature) {
        return util::retrieveFeatureData(*this, feature);
    } else {
        throw std::invalid_argument("There is no Feature with the specified name or id! Evoked at Tag::retrieveFeatureData");
    }
}


std::ostream &nix::operator<<(std::ostream &out, const Tag &ent) {
    out << "Tag: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}
