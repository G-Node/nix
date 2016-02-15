// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_GROUP_H
#define NIX_GROUP_H

#include "LocID.hpp"
#include "H5DataSet.hpp"
#include "DataSpace.hpp"
#include <nix/Hydra.hpp>
#include <nix/Platform.hpp>

#include <boost/optional.hpp>

#include <string>
#include <vector>

namespace nix {
namespace hdf5 {

struct optGroup;

/**
 * TODO documentation
 */
class NIXAPI H5Group : public LocID {

public:

    H5Group();

    H5Group(hid_t hid);

    H5Group(hid_t hid, bool is_copy) : LocID(hid, is_copy) { }

    H5Group(const H5Group &other);

    bool hasObject(const std::string &path) const;
    ndsize_t objectCount() const;
    std::string objectName(ndsize_t index) const;

    bool hasData(const std::string &name) const;

    DataSet createData(const std::string &name, const h5x::DataType &fileType,
            const NDSize &size, const NDSize &maxsize = {}, NDSize chunks = {},
            bool maxSizeUnlimited = true, bool guessChunks = true) const;

    DataSet openData(const std::string &name) const;
    void removeData(const std::string &name);

    template<typename T>
    void setData(const std::string &name, const T &value);
    template<typename T>
    bool getData(const std::string &name, T &value) const;

    bool hasGroup(const std::string &name) const;

    /**
     * @brief Open and eventually create a group with the given name
     *        inside this group. If creation is not allowed (bool
     *        param is "false") and the group does not exist an error
     *        is thrown.
     *
     * @param name    The name of the group to create.
     * @param create  Whether to create the group if it does not yet exist
     *
     * @return The opened group.
     */
    H5Group openGroup(const std::string &name, bool create = true) const;

    /**
     * @brief Create an {@link optGroup} functor that can be used to
     *        open and eventually create an optional group inside this
     *        group.
     *
     * @param name    The name of the group to create.
     *
     * @return The opened group.
     */
    optGroup openOptGroup(const std::string &name);

    void removeGroup(const std::string &name);
    void renameGroup(const std::string &old_name, const std::string &new_name);

    /**
     * @brief Look for the first sub-group in the group with the given
     * attribute that is set to the given string value and return it
     * if found. Return empty optional if not found.
     *
     * @param attribute The name of the attribute to search.
     * @param value     The value of the attribute to search.
     *
     * @return The name of the first group object found.
     */
    boost::optional<H5Group> findGroupByAttribute(const std::string &attribute, const std::string &value) const;

    /**
     * @brief Look for the first sub-data in the group with the given
     * attribute that is set to the given string value and return it
     * if found. Return empty optional if not found.
     *
     * @param attribute The name of the attribute to search.
     * @param value     The value of the attribute to search.
     *
     * @return The name of the first dataset object found.
     */
    boost::optional<DataSet> findDataByAttribute(const std::string &attribute, const std::string &value) const;

    /**
    * @brief Look for the first sub-data in the group with the given
    * name (value). If none cannot be found then search for an attribute that
    * is set to the given string value and return that if found.
    * Returns an empty optional otherwise.
    *
    * @param attribute The name of the attribute to search.
    * @param value     The name of the H5Group or the value of the attribute to look for.
    *
    * @return Optional containing the located H5Group or empty optional otherwise.
    */
    boost::optional<H5Group> findGroupByNameOrAttribute(std::string const &attribute, std::string const &value) const;

    /**
    * @brief Look for the first sub-data in the group with the given
    * name (value). If none cannot be found then search for an attribute that
    * is set to the given string value and return that if found.
    * Returns an empty optional otherwise.
    *
    * @param attribute The name of the attribute to search.
    * @param value     The name of the H5Group or the value of the attribute to look for.
    *
    * @return Optional containing the located Dataset or empty optional otherwise.
    */
    boost::optional<DataSet> findDataByNameOrAttribute(std::string const &attribute, std::string const &value) const;

    /**
     * @brief Create a new hard link with the given name inside this group,
     *        that points to the target group.
     *
     * @param target    The target of the link to create.
     * @param linkname  The name of the link to create.
     *
     * @return The linked group.
     */
    H5Group createLink(const H5Group &target, const std::string &link_name);

    /**
     * @brief Renames all links of the object defined by the old name.
     *
     * This method will only change the links where the last part of the path
     * is the old name.
     *
     * @param old_name  The old name of the object
     * @param new_name  The new name of the object
     *
     * @return True if all links where changed successfully.
     */
    bool renameAllLinks(const std::string &old_name, const std::string &new_name);

    /**
     * @brief Removes all links to the object defined by the given name.
     *
     * @param name      The name of the object to remove.
     *
     * @return  True if all links were deleted.
     */
    bool removeAllLinks(const std::string &name);


    virtual ~H5Group();


private:

    bool objectOfType(const std::string &name, H5O_type_t type) const;

}; // group H5Group


//template functions

template<typename T>
void H5Group::setData(const std::string &name, const T &value)
{
    const Hydra<const T> hydra(value);
    DataType dtype = hydra.element_data_type();
    NDSize shape = hydra.shape();

    DataSet ds;
    if (!hasData(name)) {
        h5x::DataType fileType = data_type_to_h5_filetype(dtype);
        ds = createData(name, fileType, shape);
    } else {
        ds = openData(name);
        ds.setExtent(shape);
    }

    ds.write(dtype, shape, hydra.data());
}


template<typename T>
bool H5Group::getData(const std::string &name, T &value) const
{
    if (!hasData(name)) {
        return false;
    }

    Hydra<T> hydra(value);
    DataSet ds = openData(name);

    DataType dtype = hydra.element_data_type();
    h5x::DataType memType = data_type_to_h5_memtype(dtype);
    NDSize shape = ds.size();
    hydra.resize(shape);

    ds.read(memType, shape, hydra.data());

    return true;
}


/**
 * Helper struct that works as a functor like {@link H5Group::openGroup}:
 *
 * Open and optionally create a group with the given name inside
 * this group. If creation is not allowed (bool param is "false") and
 * the group does not exist an error is thrown. If creation is not
 * allowed (bool param is "false") and the group does not exist an
 * unset optional is returned.
 */
struct NIXAPI optGroup {
    mutable boost::optional<H5Group> g;
    H5Group parent;
    std::string g_name;

public:
    optGroup(const H5Group &parent, const std::string &g_name);

    optGroup(){};

    /**
     * @brief Open and optionally create a group with the given name
     *        inside this group. If creation is not allowed (bool
     *        param is "false") and the group does not exist an unset
     *        optional is returned.
     *
     * @param create  Whether to create the group if it does not yet exist
     *
     * @return An optional with the opened group or unset.
     */
    boost::optional<H5Group> operator() (bool create = false) const;
};


} // namespace hdf5
} // namespace nix


#endif /* NIX_GROUP_H */
