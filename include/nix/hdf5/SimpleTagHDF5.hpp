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
#include <nix/hdf5/ReferenceList.hpp>

namespace nix {
namespace hdf5 {


/**
 * Class that represents a pandora tag.
 */
class SimpleTagHDF5 : virtual public base::ISimpleTag, public EntityWithSourcesHDF5 {

private:

    static const NDSize MIN_CHUNK_SIZE;
    static const NDSize MAX_SIZE_1D;

    Group representation_group;
    ReferenceList references_list;

public:

    /**
     * Copy constructor
     */
    SimpleTagHDF5(const SimpleTagHDF5 &tag);

    /**
     * Standard constructor
     */
    SimpleTagHDF5(const File &file, const Block &block, const Group &group, 
                  const std::string &id, const vector<double> _position);

    /**
     * Standard constructor that preserves the creation time.
     */
    SimpleTagHDF5(const File &file, const Block &block, const Group &group, 
                  const std::string &id, const vector<double> _position, time_t time);


    std::vector<std::string> units() const;


    void units(std::vector<std::string> &units);


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


    DataArray getReference(const std::string &id) const;


    DataArray getReference(size_t index) const;


    void addReference(const std::string &id);


    bool removeReference(const std::string &id);


    void references(const std::vector<DataArray> &references);

    //--------------------------------------------------
    // Methods concerning representations.
    //--------------------------------------------------

    bool hasRepresentation(const std::string &id) const;


    size_t representationCount() const;


    Representation getRepresentation(const std::string &id) const;


    Representation getRepresentation(size_t index) const;


    Representation createRepresentation(const std::string &data_array_id, LinkType link_type);


    bool deleteRepresentation(const std::string &id);


    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------

    void swap(SimpleTagHDF5 &other);

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
    virtual ~SimpleTagHDF5();

};


} // namespace hdf5
} // namespace nix

#endif // NIX_SIMPLE_TAG_HDF5_H
