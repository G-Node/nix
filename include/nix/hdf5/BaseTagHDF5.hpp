// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#ifndef NIX_BASETAG_HDF5_H
#define NIX_BASETAG_HDF5_H

#include <nix/hdf5/EntityWithSourcesHDF5.hpp>
#include <nix/base/IBaseTag.hpp>

namespace nix {
namespace hdf5 {


/**
* Class that represents a NIX tag.
*/
class BaseTagHDF5 : public EntityWithSourcesHDF5, virtual public base::IBaseTag {

private:

    optGroup feature_group;
    optGroup refs_group;

public:

    /**
    * Standard constructor for existing Tag
    */
    BaseTagHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const Group &group);

    /**
    * Standard constructor for new Tag
    */
    BaseTagHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const Group &group, const std::string &id,
            const std::string &type, const std::string &name);

    /**
    * Standard constructor for new Tag that preserves the creation time.
    */
    BaseTagHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const Group &group, const std::string &id,
            const std::string &type, const std::string &name, const time_t time);

    //--------------------------------------------------
    // Methods concerning references.
    //--------------------------------------------------

    virtual bool hasReference(const std::string &name_or_id) const;


    virtual size_t referenceCount() const;


    virtual std::shared_ptr<base::IDataArray> getReference(const std::string &name_or_id) const;


    virtual std::shared_ptr<base::IDataArray> getReference(size_t index) const;


    virtual void addReference(const std::string &name_or_id);


    virtual bool removeReference(const std::string &name_or_id);

    // TODO evaluate if DataArray can be replaced by shared_ptr<IDataArray>
    virtual void references(const std::vector<DataArray> &references);

    //--------------------------------------------------
    // Methods concerning features.
    //--------------------------------------------------

    virtual bool hasFeature(const std::string &name_or_id) const;


    virtual size_t featureCount() const;


    virtual std::shared_ptr<base::IFeature> getFeature(const std::string &name_or_id) const;


    virtual std::shared_ptr<base::IFeature> getFeature(size_t index) const;


    virtual std::shared_ptr<base::IFeature> createFeature(const std::string &name_or_id, LinkType link_type);


    virtual bool deleteFeature(const std::string &name_or_id);


    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------

    /**
    * Destructor.
    */
    virtual ~BaseTagHDF5();

};


} // namespace hdf5
} // namespace nix

#endif // NIX_BASETAG_HDF5_H
