//
// Created by grewe on 11/20/15.
//

#ifndef NIX_GROUPHDF5_HPP
#define NIX_GROUPHDF5_HPP

#include <nix/util/util.hpp>
#include <nix/Group.hpp>
#include "DataArrayHDF5.hpp"


namespace nix {
namespace hdf5 {

/**
 * Class that represents a NIX Group entity.
 */
class GroupHDF5 : virtual public base::IGroup, public EntityWithSourcesHDF5,
                  public std::enable_shared_from_this<GroupHDF5> {

private:

    optGroup data_array_group, tag_group, multi_tag_group;


    // Helper methods for generic entity related methods below
    boost::optional<H5Group> groupForObjectType(ObjectType ot) const;

    boost::optional<H5Group> findEntityGroup(const nix::Identity &ident) const;

public:

    //--------------------------------------------------
    // Generic entity methods
    //--------------------------------------------------
    bool hasEntity(const nix::Identity &ident) const ;

    std::shared_ptr<base::IEntity> getEntity(const nix::Identity &ident) const;

    std::shared_ptr<base::IEntity> getEntity(ObjectType type, ndsize_t index) const;

    ndsize_t entityCount(ObjectType type) const;

    bool removeEntity(const nix::Identity &ident);

    void addEntity(const nix::Identity &ident);
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
};
}
}

#endif //NIX_GROUPHDF5_HPP
