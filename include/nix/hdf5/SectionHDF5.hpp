// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_SECTION_HDF5_H
#define NIX_SECTION_HDF5_H

#include <string>
#include <functional>

#include <nix.hpp>
#include <nix/hdf5/NamedEntityHDF5.hpp>

namespace nix {
namespace hdf5 {

class SectionHDF5 : public NamedEntityHDF5, virtual public base::ISection,
                    public std::enable_shared_from_this<SectionHDF5> {

private:

    Section parent_section;
    Group property_group, section_group;

public:

    /**
     * Copy constructor
     */
    SectionHDF5(const SectionHDF5 &section);

    /**
     * Standard constructor
     */
    SectionHDF5(const File &file, const Group &group, const std::string &id);

    /**
     * Standard constructor with parent.
     */
    SectionHDF5(const File &file, const Section &parent, const Group &group,
                const std::string &id);

    /**
     * Constructor that preserves the creation time.
     */
    SectionHDF5(const File &file, const Group &group, const std::string &id, time_t time);

    /**
     * Constructor with parent that preserves the creation time.
     */
    SectionHDF5(const File &file, const Section &parent, const Group &group,
                const std::string &id, time_t time);


    //--------------------------------------------------
    // Attribute getter and setter
    //--------------------------------------------------

    void repository(const std::string &repository);


    std::string repository() const;


    void link(const Section &link);


    Section link() const;


    void mapping(const std::string &mapping);


    std::string mapping() const;

    //--------------------------------------------------
    // Methods for parent access
    //--------------------------------------------------


    Section parent() const;


    //--------------------------------------------------
    // Methods for child section access
    //--------------------------------------------------


    size_t sectionCount() const;


    bool hasSection(const std::string &id) const;


    Section getSection(const std::string &id) const;


    Section getSection(size_t index) const;


    Section createSection(const std::string &name, const std::string &type);


    bool deleteSection(const std::string &id);

    //--------------------------------------------------
    // Methods for property access
    //--------------------------------------------------


    size_t propertyCount() const;


    bool hasProperty(const std::string &id) const;


    Property getProperty(const std::string &id) const;


    Property getProperty(size_t index) const;


    bool hasPropertyWithName(const std::string &name) const;


    Property getPropertyByName(const std::string &name) const;


    Property createProperty(const std::string &name);


    bool deleteProperty(const std::string &id);

    //--------------------------------------------------
    // Ohter methods and operators
    //--------------------------------------------------

    virtual ~SectionHDF5();

};


} // namespace hdf5
} // namespace nix

#endif // NIX_SECTION_HDF5_H
