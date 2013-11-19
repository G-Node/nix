// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/SourceHDF5.hpp>

using namespace std;

namespace nix {
namespace hdf5 {


SourceHDF5::SourceHDF5(const Source &source)
    : EntityWithMetadataHDF5(source.file, source.group, source.entity_id)
{
    source_group = source.source_group;
}


SourceHDF5::SourceHDF5(File file, Group group, const std::string &id)
    : EntityWithMetadataHDF5(file, group, id)
{
    source_group = group.openGroup("sources");
}


SourceHDF5::SourceHDF5(File file, Group group, const std::string &id, time_t time)
    : EntityWithMetadataHDF5(file, group, id, time)
{
    source_group = group.openGroup("sources");
}


SourceHDF5::size_type SourceHDF5::childCount() const {
    return source_group.objectCount();
}


Source SourceHDF5::getChild(size_type index) const {
    string id = source_group.objectName(index);
    return Source(file, source_group.openGroup(id, false), id);
}


bool SourceHDF5::hasSource(const string &id) const {
    return source_group.hasGroup(id);
}


Source SourceHDF5::getSource(const string &id) const {
    return Source(file, source_group.openGroup(id, false), id);
}


Source SourceHDF5::getSource(size_t index) const {
    string id = source_group.objectName(index);
    return Source(file, source_group.openGroup(id, false), id);
}


size_t SourceHDF5::sourceCount() const {
    return source_group.objectCount();
}


std::vector<Source> SourceHDF5::sources() const {
    return collectIf(predCollectAll, true, 1);
}


Source SourceHDF5::createSource(const string &name, const string &type) {
    string id = util::createId("source");

    while(source_group.hasObject(id)) {
        id = util::createId("source");
    }

    Source s(file, source_group.openGroup(id, true), id);
    s.name(name);
    s.type(type);

    return s;
}


ostream& operator<<(ostream &out, const SourceHDF5 &ent) {
    out << "Source: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}


Source& SourceHDF5::operator=(const SourceHDF5 &other) {
    if (*this != other) {
        this->file = other.file;
        this->group = other.group;
        this->entity_id = other.entity_id;
        this->source_group = other.source_group;
    }
    return *this;
}


SourceHDF5::~SourceHDF5() {}


} // namespace hdf5
} // namespace nix
