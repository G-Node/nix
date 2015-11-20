//
// Created by grewe on 11/20/15.
//

#ifndef NIX_GROUPHDF5_HPP
#define NIX_GROUPHDF5_HPP

#include <nix/hdf5/GroupHDF5.hpp>

#include <nix/util/util.hpp>
#include <nix/Group.hpp>
#include <nix/hdf5/DataArrayHDF5.hpp>


namespace nix {
namespace hdf5 {

/**
 * Class that represents a NIX Group entity.
 */
class GroupHDF5 : virtual public base::IGroup, public EntityWithSourcesHDF5,
                  public std::enable_shared_from_this<GroupHDF5> {

private:

    optGroup data_array_group, tag_group, multi_tag_group;

public:
    /**
     * Standard constructor for existing Group
     */
    GroupHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const H5Group &h5group);

    /**
     * Standard constructor for new Group
     */
    GroupHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const H5Group &h5group,
              const std::string &id, const std::string &type, const std::string &name);

    /**
     * Standard constructor for new Group that preserves the creation time.
     */
    GroupHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const H5Group &h5group,
              const std::string &id, const std::string &type, const std::string &name, time_t time);


    bool hasDataArray(const std::string &name_or_id) const;


    std::shared_ptr<base::IDataArray> getDataArray(const std::string &name_or_id) const;

};
}
}

#endif //NIX_GROUPHDF5_HPP