// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_GROUP_H
#define NIX_GROUP_H

#include <string>

#include <nix/hdf5/hdf5include.hpp>
#include <nix/hdf5/BaseHDF5.hpp>
#include <nix/hdf5/DataSetHDF5.hpp>
#include <nix/hdf5/DataSpace.hpp>
#include <nix/Hydra.hpp>
#include <nix/Platform.hpp>

#include <boost/optional.hpp>

namespace nix {
namespace hdf5 {

struct optGroup;

/**
 * TODO documentation
 */
class NIXAPI Group : public BaseHDF5 {

public:

    Group();

    Group(hid_t hid);

    Group(const Group &other);

    Group(const H5::Group &h5group);

    bool hasAttr(const std::string &name) const;
    void removeAttr(const std::string &name) const;

    template <typename T>
    void setAttr(const std::string &name, const T &value) const;

    template <typename T>
    bool getAttr(const std::string &name, T &value) const;

    bool hasObject(const std::string &path) const;
    size_t objectCount() const;
    std::string objectName(size_t index) const;

    bool hasData(const std::string &name) const;
    DataSet createData(const std::string &name,
            const H5::DataType &fileType,
            const H5::DataSpace &fileSpace, const H5::DSetCreatPropList &cpList) const;
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
    Group openGroup(const std::string &name, bool create = true) const;

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
    boost::optional<Group> findGroupByAttribute(const std::string &attribute, const std::string &value) const;

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
    * @param value     The name of the Group or the value of the attribute to look for.
    *
    * @return Optional containing the located Group or empty optional otherwise.
    */
    boost::optional<Group> findGroupByNameOrAttribute(std::string const &attribute, std::string const &value) const;

    /**
    * @brief Look for the first sub-data in the group with the given
    * name (value). If none cannot be found then search for an attribute that
    * is set to the given string value and return that if found.
    * Returns an empty optional otherwise.
    *
    * @param attribute The name of the attribute to search.
    * @param value     The name of the Group or the value of the attribute to look for.
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
    Group createLink(const Group &target, const std::string &link_name);

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

    H5::Group h5Group() const;
    virtual ~Group();


private:

    H5::Attribute openAttr(const std::string &name) const;
    H5::Attribute createAttr(const std::string &name, H5::DataType fileType, H5::DataSpace fileSpace) const;

    bool objectOfType(const std::string &name, H5O_type_t type) const;

    static void readAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, void *data);
    static void readAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, std::string *data);

    static void writeAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, const void *data);
    static void writeAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, const std::string *data);
}; // group Group

//template functions

template<typename T> void Group::setAttr(const std::string &name, const T &value) const
{
    typedef Hydra<const T> hydra_t;

    const hydra_t hydra(value);
    DataType dtype = hydra.element_data_type();
    NDSize shape = hydra.shape();

    H5::Attribute attr;

    if (hasAttr(name)) {
        attr = openAttr(name);
    } else {
        H5::DataType fileType = data_type_to_h5_filetype(dtype);
        H5::DataSpace fileSpace = DataSpace::create(shape, false);
        attr = createAttr(name, fileType, fileSpace);
    }

    writeAttr(attr, data_type_to_h5_memtype(dtype), shape, hydra.data());
}



template<typename T> bool Group::getAttr(const std::string &name, T &value) const
{
    if (!hasAttr(name)) {
        return false;
    }

    Hydra<T> hydra(value);

    //determine attr's size and resize value accordingly
    H5::Attribute attr = openAttr(name);
    H5::DataSpace space = attr.getSpace();
    int rank = space.getSimpleExtentNdims();
    NDSize dims(static_cast<size_t>(rank));
    space.getSimpleExtentDims (dims.data(), nullptr);
    hydra.resize(dims);

    DataType dtype = hydra.element_data_type();
    H5::DataType mem_type = data_type_to_h5_memtype(dtype);

    readAttr(attr, mem_type, dims, hydra.data());

    return true;
}

template<typename T>
void Group::setData(const std::string &name, const T &value)
{
    const Hydra<const T> hydra(value);
    DataType dtype = hydra.element_data_type();
    NDSize shape = hydra.shape();

    DataSet ds;
    if (!hasData(name)) {
        ds = DataSet::create(h5Group(), name, dtype, shape);
    } else {
        ds = openData(name);
        ds.setExtent(shape);
    }

    ds.write(dtype, shape, hydra.data());
}

template<typename T>
bool Group::getData(const std::string &name, T &value) const
{
    if (!hasData(name)) {
        return false;
    }

    Hydra<T> hydra(value);
    DataSet ds = openData(name);

    DataType dtype = hydra.element_data_type();
    NDSize shape = ds.size();

    hydra.resize(shape);
    ds.read(dtype, shape, hydra.data());

    return true;
}


/**
 * Helper struct that works as a functor like {@link Group::openGroup}:
 *
 * Open and eventually create a group with the given name inside
 * this group. If creation is not allowed (bool param is "false") and
 * the group does not exist an error is thrown. If creation is not
 * allowed (bool param is "false") and the group does not exist an
 * unset optional is returned.
 */
struct NIXAPI optGroup {
    mutable boost::optional<Group> g;
    Group parent;
    std::string g_name;

public:
    optGroup(const Group &parent, const std::string &g_name);

    optGroup(){};

    /**
     * @brief Open and eventually create a group with the given name
     *        inside this group. If creation is not allowed (bool
     *        param is "false") and the group does not exist an unset
     *        optional is returned.
     *
     * @param create  Whether to create the group if it does not yet exist
     *
     * @return An optional with the opened group or unset.
     */
    boost::optional<Group> operator() (bool create = false) const;
};


} // namespace hdf5
} // namespace nix


#endif /* NIX_GROUP_H */
