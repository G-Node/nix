// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/Group.hpp>
#include <boost/multi_array.hpp>

namespace nix {
namespace hdf5 {


Group::Group()
    : h5group()
{}


Group::Group(H5::Group h5group)
    : h5group(h5group)
{}


Group::Group(const Group &group)
    : h5group(group.h5group)
{}


Group& Group::operator=(const Group &group)
{
    h5group = group.h5group;
    return *this;
}


bool Group::hasAttr(const std::string &name) const {
    return H5Aexists(h5group.getId(), name.c_str());
}


void Group::removeAttr(const std::string &name) const {
    h5group.removeAttr(name);
}


bool Group::hasObject(const std::string &name) const {
    // empty string should return false, not exception (which H5Lexists would)
    if (name.empty()) {
        return false;
    }
    htri_t res = H5Lexists(h5group.getLocId(), name.c_str(), H5P_DEFAULT);
    return res;
}


size_t Group::objectCount() const {
    return h5group.getNumObjs();
}


std::string Group::objectName(size_t index) const {
    // check if index valid
    if(index > objectCount()) {
        throw OutOfBounds("No object at given index", index);
    }
    
    std::string str_name;
    // check whether name is found by index
    ssize_t name_len = H5Lget_name_by_idx(h5group.getLocId(),
                                                  ".",
                                                  H5_INDEX_NAME,
                                                  H5_ITER_NATIVE,
                                                  (hsize_t) index,
                                                  NULL,
                                                  0,
                                                  H5P_DEFAULT);
    if (name_len > 0) {
        char* name = new char[name_len+1];
        name_len = H5Lget_name_by_idx(h5group.getLocId(),
                                      ".",
                                      H5_INDEX_NAME,
                                      H5_ITER_NATIVE,
                                      (hsize_t) index,
                                      name,
                                      name_len+1,
                                      H5P_DEFAULT);
        str_name = name;
        delete [] name;
    } else {
        throw std::runtime_error("objectName: No object found, H5Lget_name_by_idx returned no name");    
    }

    return str_name;
}


bool Group::hasData(const std::string &name) const {
    if (hasObject(name)) {
        H5G_stat_t info;
        h5group.getObjinfo(name, info);
        if (info.type == H5G_DATASET) {
            return true;
        }
    }
    return false;
}


void Group::removeData(const std::string &name) {
    if (hasData(name))
        h5group.unlink(name);
}


DataSet Group::openData(const std::string &name) const {
    H5::DataSet ds5 = h5group.openDataSet(name);
    return DataSet(ds5);
}


bool Group::hasGroup(const std::string &name) const {
    if (hasObject(name)) {
        H5G_stat_t info;
        h5group.getObjinfo(name, info);
        if (info.type == H5G_GROUP) {
            return true;
        }
    }
    return false;
}


Group Group::openGroup(const std::string &name, bool create) const {
    Group g;
    if (hasGroup(name)) {
        g = Group(h5group.openGroup(name));
    } else if (create) {
        g = Group(h5group.createGroup(name));
    } else {
        throw std::runtime_error("Unable to open group with name '" + name + "'!");
    }
    return g;
}


void Group::removeGroup(const std::string &name) {
    if (hasGroup(name))
        h5group.unlink(name);
}


void Group::renameGroup(const std::string &old_name, const std::string &new_name) {
    if (hasGroup(old_name)) {
        h5group.move(old_name, new_name);
    }
}


bool Group::operator==(const Group &group) const {
    return h5group.getLocId() == group.h5group.getLocId();
}


bool Group::operator!=(const Group &group) const {
    return h5group.getLocId() != group.h5group.getLocId();
}


H5::Group Group::h5Group() const {
    return h5group;
}


Group Group::createLink(const Group &target, const std::string &link_name) {
    herr_t error = H5Lcreate_hard(target.h5group.getLocId(), ".", h5group.getLocId(), link_name.c_str(),
                                  H5L_SAME_LOC, H5L_SAME_LOC);
    if (error)
        throw std::runtime_error("Unable to create link " + link_name);

    return openGroup(link_name, false);
}

// TODO implement some kind of roll-back in order to avoid half renamed links.
bool Group::renameAllLinks(const std::string &old_name, const std::string &new_name) {
    bool renamed = false;

    if (hasGroup(old_name)) {
        std::vector<std::string> links;

        Group  group     = openGroup(old_name, false);
        size_t size      = 128;
        char *name_read  = new char[size];

        size_t size_read = H5Iget_name(group.h5group.getId(), name_read, size);
        while (size_read > 0) {

            if (size_read < size) {
                H5Ldelete(h5group.getId(), name_read, H5L_SAME_LOC);
                links.push_back(name_read);
            } else {
                delete[] name_read;
                size = size * 2;
                name_read = new char[size];
            }

            size_read = H5Iget_name(group.h5group.getId(), name_read, size);
        }

        renamed = links.size() > 0;
        for (std::string curr_name: links) {
            size_t pos = curr_name.find_last_of('/') + 1;

            if (curr_name.substr(pos) == old_name) {
                curr_name.replace(curr_name.begin() + pos, curr_name.end(), new_name.begin(), new_name.end());
            }

            herr_t error = H5Lcreate_hard(group.h5group.getLocId(), ".", h5group.getLocId(), curr_name.c_str(),
                                          H5L_SAME_LOC, H5L_SAME_LOC);

            renamed = renamed && (error >= 0);
        }
    }

    return renamed;
}

// TODO implement some kind of roll-back in order to avoid half removed links.
bool Group::removeAllLinks(const std::string &name) {
    bool removed = false;

    if (hasGroup(name)) {
        Group  group      = openGroup(name, false);
        size_t size       = 128;
        char *name_read   = new char[size];

        size_t size_read  = H5Iget_name(group.h5group.getId(), name_read, size);
        while (size_read > 0) {
            if (size_read < size) {
                H5Ldelete(h5group.getId(), name_read, H5L_SAME_LOC);
            } else {
                delete[] name_read;
                size = size * 2;
                name_read = new char[size];
            }
            size_read = H5Iget_name(group.h5group.getId(), name_read, size);
        }

        delete[] name_read;
        removed = true;
    }

    return removed;
}


void Group::readAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, void *data) {
    attr.read(mem_type, data);
}


void Group::readAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, std::string *data) {
    StringWriter writer(size, data);
    attr.read(mem_type, *writer);
    writer.finish();
    H5::DataSet::vlenReclaim(*writer, mem_type, attr.getSpace()); //recycle space?
}


void Group::writeAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, const void *data) {
    attr.write(mem_type, data);
}


void Group::writeAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, const std::string *data) {
    StringReader reader(size, data);
    attr.write(mem_type, *reader);
}


Group::~Group() {
    h5group.close();
}

} // namespace hdf5
} // namespace nix
