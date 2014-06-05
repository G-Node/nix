// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_SOURCE_HDF5_H
#define NIX_SOURCE_HDF5_H


#include <string>
#include <functional>

#include <nix.hpp>
#include <nix/hdf5/EntityWithMetadataHDF5.hpp>

namespace nix {
namespace hdf5 {

/**
 * An entity that implements ISource for the HDF5 back-end.
 */
class SourceHDF5 : virtual public base::ISource, public EntityWithMetadataHDF5  {

private:

    Group source_group;

public:


    /**
     * Copyconstructor.
     */
    SourceHDF5(const SourceHDF5 &source);

    /**
     * Default constructor.
     */
    SourceHDF5(File file, Group group, const std::string &id, const std::string &type, const std::string &name);

    /**
     * Default constructor that preserves the creation time.
     */
    SourceHDF5(File file, Group group, const std::string &id, const std::string &type, const std::string &name, time_t time);

    //--------------------------------------------------
    // Methods concerning child sources
    //--------------------------------------------------


    bool hasSource(const std::string &id) const;


    Source getSource(const std::string &id) const;


    Source getSource(size_t index) const;


    size_t sourceCount() const;


    Source createSource(const std::string &name, const std::string &type);


    bool deleteSource(const std::string &id);

    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------


    void swap(SourceHDF5 &other);


    SourceHDF5& operator=(const SourceHDF5 &other);


    friend std::ostream& operator<<(std::ostream &out, const SourceHDF5 &ent);


    virtual ~SourceHDF5();
};


} // namestace hdf5
} // namespace nix

#endif // NIX_SOURCE_HDF5_H
