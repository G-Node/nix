// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#include <nix/util/util.hpp>
#include <nix/hdf5/SimpleTagHDF5.hpp>
#include <nix/hdf5/RepresentationHDF5.hpp>
#include <nix/hdf5/DataSet.hpp>

using namespace std;

namespace nix {
namespace hdf5 {


const NDSize SimpleTagHDF5::MIN_CHUNK_SIZE = {1};
const NDSize SimpleTagHDF5::MAX_SIZE_1D = {H5S_UNLIMITED};


SimpleTagHDF5::SimpleTagHDF5(const SimpleTagHDF5 &tag)
    : EntityWithSourcesHDF5(tag.file(), tag.block(), tag.group(), tag.id()),
      references_list(tag.group(), "references")
{
    representation_group = tag.representation_group;
}


SimpleTagHDF5::SimpleTagHDF5(const File &file, const Block &block, const Group &group,
                             const string &id)
    : EntityWithSourcesHDF5(file, block, group, id), references_list(group, "references")
{
    representation_group = group.openGroup("representations");
}


SimpleTagHDF5::SimpleTagHDF5(const File &file, const Block &block, const Group &group,
                             const string &id, time_t time)
    : EntityWithSourcesHDF5(file, block, group, id, time), references_list(group, "references")
{
    representation_group = group.openGroup("representations");
}


vector<string> SimpleTagHDF5::units() const {
    vector<string> units;

    if (group().hasData("units")) {
        DataSet ds = group().openData("units");
        ds.read(units, true);
    }

    return units;
}


void SimpleTagHDF5::units(vector<string> &units) {
    if (group().hasData("units")) {
        DataSet ds = group().openData("units");
        ds.extend({units.size()});
        ds.write(units);
    } else {
        DataSet ds = DataSet::create(group().h5Group(), "units", units, &MAX_SIZE_1D, &MIN_CHUNK_SIZE);
        ds.write(units);
    }
}


vector<double> SimpleTagHDF5::position() const {
    vector<double> position;

    if (group().hasData("position")) {
        DataSet ds = group().openData("position");
        ds.read(position, true);
    }

    return position;
}


void SimpleTagHDF5::position(const vector<double> &position) {
    if (group().hasData("position")) {
        DataSet ds = group().openData("position");
        ds.extend({position.size()});
        ds.write(position);
    } else {
        DataSet ds = DataSet::create(group().h5Group(), "position", position, &MAX_SIZE_1D, &MIN_CHUNK_SIZE);
        ds.write(position);
    }
}


vector<double> SimpleTagHDF5::extent() const {
    vector<double> extent;

    if (group().hasData("extent")) {
        DataSet ds = group().openData("extent");
        ds.read(extent, true);
    }

    return extent;
}


void SimpleTagHDF5::extent(const vector<double> &extent) {
    if (group().hasData("extent")) {
        DataSet ds = group().openData("extent");
        ds.extend({extent.size()});
        ds.write(extent);
    } else {
        DataSet ds = DataSet::create(group().h5Group(), "extent", extent, &MAX_SIZE_1D, &MIN_CHUNK_SIZE);
        ds.write(extent);
    }
}

// Methods concerning references.

bool SimpleTagHDF5::hasReference(const DataArray &reference) const {
    return references_list.has(reference.id());
}


bool SimpleTagHDF5::hasReference(const std::string &id) const {
    return references_list.has(id);
}


size_t SimpleTagHDF5::referenceCount() const {
    return references_list.count();
}


DataArray SimpleTagHDF5::getReference(const std::string &id) const {
    if (hasReference(id)) {
        return block().getDataArray(id);
    } else {
        throw runtime_error("No reference with id: " + id);
    }
}


void SimpleTagHDF5::addReference(const DataArray &reference) {
    string reference_id = reference.id();

    if (block().hasDataArray(reference_id)) {
        throw runtime_error("Unable to find data array with reference_id " +
                            reference_id + " on block " + block().id());
    }

    references_list.add(reference_id);
}


bool SimpleTagHDF5::removeReference(const DataArray &reference) {
    return references_list.remove(reference.id());
}


std::vector<DataArray> SimpleTagHDF5::references() const {
    vector<string> ids = references_list.get();
    vector<DataArray> refs;

    for (size_t i = 0; i < ids.size(); i++) {
        if (block().hasDataArray(ids[i])) {
            refs.push_back(block().getDataArray(ids[i]));
        }
    }

    return refs;
}


void SimpleTagHDF5::references(const std::vector<DataArray> &references) {
    vector<string> ids(references.size());

    for (size_t i = 0; i < references.size(); i++) {
        ids[i] = references[i].id();
    }

    references_list.set(ids);
}

// Methods concerning representations.

bool SimpleTagHDF5::hasRepresentation(const string &id) const {
    return representation_group.hasGroup(id);
}


size_t SimpleTagHDF5::representationCount() const {
    return representation_group.objectCount();
}


Representation SimpleTagHDF5::getRepresentation(const std::string &id) const {
    Group rep_g = representation_group.openGroup(id, false);
    shared_ptr<RepresentationHDF5> tmp(new RepresentationHDF5(file(), block(), rep_g, id));

    return Representation(tmp);
}


Representation SimpleTagHDF5::getRepresentation(size_t index) const{
    string rep_id = representation_group.objectName(index);
    Group rep_g = representation_group.openGroup(rep_id, false);
    shared_ptr<RepresentationHDF5> tmp(new RepresentationHDF5(file(), block(), rep_g, rep_id));

    return Representation(tmp);
}


std::vector<Representation> SimpleTagHDF5::representations() const{
    vector<Representation>  representation_obj;
    size_t count = representation_group.objectCount();
    for (size_t i = 0; i < count; i++) {
        string rep_id = representation_group.objectName(i);
        Group rep_g = representation_group.openGroup(rep_id, false);
        shared_ptr<RepresentationHDF5> tmp(new RepresentationHDF5(file(), block(), rep_g, rep_id));

        representation_obj.push_back(Representation(tmp));
    }
    return representation_obj;
}


Representation SimpleTagHDF5::createRepresentation(DataArray data, LinkType link_type){
    string rep_id = util::createId("representation");
    while(representation_group.hasObject(rep_id))
        rep_id = util::createId("representation");

    Group rep_g = representation_group.openGroup(rep_id, false);
    shared_ptr<RepresentationHDF5> tmp(new RepresentationHDF5(file(), block(), rep_g, rep_id));
    tmp->linkType(link_type);
    tmp->data(data);

    return Representation(tmp);
}


bool SimpleTagHDF5::removeRepresentation(const string &id){
    if (representation_group.hasGroup(id)) {
        representation_group.removeGroup(id);
        return true;
    } else {
        return false;
    }
}

// Other methods and functions


void SimpleTagHDF5::swap(SimpleTagHDF5 &other) {
    using std::swap;

    EntityWithSourcesHDF5::swap(other);
    swap(representation_group, other.representation_group);
    swap(references_list, other.references_list);
}


SimpleTagHDF5& SimpleTagHDF5::operator=(const SimpleTagHDF5 &other) {
    if (*this != other) {
        SimpleTagHDF5 tmp(other);
        swap(tmp);
    }
    return *this;
}


ostream& operator<<(ostream &out, const SimpleTagHDF5 &ent) {
    out << "SimpleTag: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}


SimpleTagHDF5::~SimpleTagHDF5()
{
    // TODO Auto-generated destructor stub
}


} // namespace hdf5
} // namespace nix
