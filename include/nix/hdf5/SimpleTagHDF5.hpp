// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#ifndef NIX_SIMPLE_TAG_HDF5_H
#define NIX_SIMPLE_TAG_HDF5_H

#include <string>
#include <vector>

#include <nix.hpp>
#include <nix/hdf5/EntityWithSourcesHDF5.hpp>

namespace nix {
namespace hdf5 {


/**
 * Class that represents a pandora tag.
 */
class SimpleTagHDF5 : virtual public ISimpleTag, public EntityWithSourcesHDF5 {

private:

    static const PSize MIN_CHUNK_SIZE;
    static const PSize MAX_SIZE_1D;

    Group representation_group;
    util::ReferenceList references_list;

public:

    /**
     * Copy constructor
     */
    SimpleTagHDF5(const SimpleTag &tag);

    /**
     * Standard constructor
     */
    SimpleTagHDF5(File file, Block block, Group group, const std::string &id);

    /**
     * Standard constructor that preserves the creation time.
     */
    SimpleTagHDF5(File file, Block block, Group group, const std::string &id, time_t time);


    std::vector<std::string> units() const;


    void units(std::vector<std::string> &units);


    std::vector<double> position() const;


    void position(const std::vector<double> &position);


    std::vector<double> extent() const;


    void extent(const std::vector<double> &extent);

    //--------------------------------------------------
    // Methods concerning references.
    //--------------------------------------------------

    bool hasReference(const DataArray &reference) const;


    bool hasReference(const std::string &id) const;


    size_t referenceCount() const;


    DataArray getReference(const std::string &id) const;


    void addReference(const DataArray &reference);


    bool removeReference(const DataArray &reference);


    std::vector<DataArray> references() const;


    void references(const std::vector<DataArray> &references);

    //--------------------------------------------------
    // Methods concerning representations.
    //--------------------------------------------------

    bool hasRepresentation(const std::string &id) const;


    size_t representationCount() const;


    Representation getRepresentation(const std::string &id) const;


    Representation getRepresentation(size_t index) const;


    std::vector<Representation> representations() const;


    Representation createRepresentation(DataArray data, LinkType link_type);


    bool removeRepresentation(const std::string &id);


    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------

    /**
     * Assignment operator
     */
    SimpleTagHDF5& operator=(const SimpleTagHDF5 &other);

    /**
     * Output operator
     */
    friend std::ostream& operator<<(std::ostream &out, const SimpleTagHDF5 &ent);

    /**
     * Destructor.
     */
    virtual ~SimpleTag();
};


} // namespace hdf5
} // namespace nix

#endif // NIX_SIMPLE_TAG_HDF5_H
