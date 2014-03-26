// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#ifndef NIX_REFERENCE_LIST_H
#define NIX_REFERENCE_LIST_H

#include <nix/hdf5/Group.hpp>
#include <nix/hdf5/EntityHDF5.hpp>

#include <nix/Platform.hpp>

namespace nix {
namespace hdf5 {

/**
 * ReferenceList is a sub-class of {@link Group} classes: it always
 * lives in a group and requires one upon construction to nest itself
 * in. Thus when copying an entity it is enough to copy its group(s),
 * any ReferenceList will come along.
 * Its' intended use is to store references to other entities, e.g.
 * referenced {@link DataArray} entities in {@link SimpleTag} or 
 * {@link DataTag} entities. 
 */
class NIXAPI ReferenceList {

    static const NDSize MIN_CHUNK_SIZE;
    static const NDSize MAX_SIZE_1D;

    Group group;
    std::string ds_name;

public:

    ReferenceList(const ReferenceList &other);

    ReferenceList(const Group &group, const std::string &ds_name);

    bool has(const std::string &id) const;

    inline bool has(const EntityHDF5 &entity) const;

    std::vector<std::string> get() const;

    void set(const std::vector<std::string> &ids);

    void add(const std::string &id);

    inline void add(const EntityHDF5 &entity);

    bool remove(const std::string &id);

    inline bool remove(const EntityHDF5 &entity);

    inline size_t count() const;

    bool operator==(const ReferenceList &other) const;

    bool operator!=(const ReferenceList &other) const;

    ReferenceList& operator=(const ReferenceList &other);

    virtual ~ReferenceList();

};

// implementations of inline methods

bool ReferenceList::has(const EntityHDF5 &entity) const {
    return has(entity.id());
}


void ReferenceList::add(const EntityHDF5 &entity) {
    add(entity.id());
}


bool ReferenceList::remove(const EntityHDF5 &entity) {
    return remove(entity.id());
}


size_t ReferenceList::count() const {
    return get().size();
}

} // namespace hdf5
} // namespace nix

#endif // NIX_REFERENCE_LIST_H
