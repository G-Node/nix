// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#ifndef NIX_TAG_HDF5_H
#define NIX_TAG_HDF5_H

#include <nix/hdf5/EntityWithSourcesHDF5.hpp>
#include <nix/base/ITag.hpp>

namespace nix {
namespace hdf5 {


/**
 * Class that represents a NIX tag.
 */
class TagHDF5 : virtual public base::ITag, public EntityWithSourcesHDF5 {

private:

    static const NDSize MIN_CHUNK_SIZE;
    static const NDSize MAX_SIZE_1D;

    optGroup feature_group;
    optGroup refs_group;

public:

    /**
     * Standard constructor for existing Tag
     */
    TagHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, Group group);

    /**
     * Standard constructor for new Tag
     */
    TagHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, Group group, const std::string &id,
                  const std::string &type, const std::string &name, const std::vector<double> &position);

    /**
     * Standard constructor for new Tag that preserves the creation time.
     */
    TagHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, Group group, const std::string &id,
                  const std::string &type, const std::string &name, const std::vector<double> &_position, const time_t time);


    std::vector<std::string> units() const;


    void units(const std::vector<std::string> &units);


    void units(const none_t t);


    std::vector<double> position() const;


    void position(const std::vector<double> &position);


    std::vector<double> extent() const;


    void extent(const std::vector<double> &extent);


    void extent(const none_t t);


    //--------------------------------------------------
    // Methods concerning references.
    //--------------------------------------------------

    bool hasReference(const std::string &id) const;


    size_t referenceCount() const;


    std::shared_ptr<base::IDataArray> getReference(const std::string &id) const;


    std::shared_ptr<base::IDataArray> getReference(size_t index) const;


    void addReference(const std::string &id);


    bool removeReference(const std::string &id);


    void references(const std::vector<DataArray> &references);

    //--------------------------------------------------
    // Methods concerning features.
    //--------------------------------------------------

    bool hasFeature(const std::string &id) const;


    size_t featureCount() const;


    std::shared_ptr<base::IFeature> getFeature(const std::string &id) const;


    std::shared_ptr<base::IFeature> getFeature(size_t index) const;


    std::shared_ptr<base::IFeature> createFeature(const std::string &data_array_id, LinkType link_type);


    bool deleteFeature(const std::string &id);


    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------

    /**
     * Destructor.
     */
    virtual ~TagHDF5();

};


} // namespace hdf5
} // namespace nix

#endif // NIX_TAG_HDF5_H
