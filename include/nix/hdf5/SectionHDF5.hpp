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

class SectionHDF5 : public NamedEntityHDF5, virtual public ISection {

private:

    // TODO add parent section and parent block
    // Section parent_section;
    // Block parent_block
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
     * Standard constructor that preserves the creation time.
     */
    SectionHDF5(const File &file, const Group &group, const std::string &id, time_t time);



    //--------------------------------------------------
    // Attribute getter and setter
    //--------------------------------------------------

    void repository(const std::string &repository);


    std::string repository() const;


    // TODO should this thake a section as parameter?
    void link(const std::string &link);


    // TODO maybe return a section here (what if there is none)?
    std::string link() const;


    void mapping(const std::string &mapping);


    std::string mapping() const;

    //--------------------------------------------------
    // Methods for parent access
    //--------------------------------------------------


    bool hasParent() const;


    Section parent() const;

    //--------------------------------------------------
    // Methods for child section access
    //--------------------------------------------------


    size_t childCount() const;


    bool hasChild(const std::string &id) const;


    Section getChild(const std::string &id) const;



    std::vector<Section> children() const;


    Section createChild(const std::string &name, const std::string &type);


    bool removeChild(const std::string &id);

    //--------------------------------------------------
    // Methods for property access
    //--------------------------------------------------


    size_t propertyCount() const;


    bool hasProperty(const std::string &id) const;


    Property getProperty(const std::string &id) const;


    std::vector<Property> properties() const;


    Property createProperty(const std::string &name);


    bool removeProperty(const std::string &id);

    //--------------------------------------------------
    // Ohter methods and operators
    //--------------------------------------------------

    virtual ~SectionHDF5();

private:

    //bool hasParent() const;

    //Section findParent() const;

    //std::vector<Section> findUpstream(const std::string &type) const;

    //std::vector<Section> findSideways(const std::string &type) const;

    //std::vector<Section> findDownstream(const std::string &type) const;

};


} // namespace hdf5
} // namespace nix

#endif // NIX_SECTION_HDF5_H
