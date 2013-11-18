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

    Group property_group, section_group;

public:

    /**
     * Copy constructor
     */
    SectionHDF5(const SectionHDF5 &section);

    /**
     * Standard constructor
     */
    SectionHDF5(File file, Group group, const std::string &id);

    /**
     * Standard constructor that preserves the creation time.
     */
    SectionHDF5(File file, Group group, const std::string &id, time_t time);



    //TNode interface
    //virtual size_type childCount() const;
    //virtual Section   getChild(size_type index) const;


    void repository(const std::string &repository);


    std::string repository() const;


    void link(const std::string &link);


    std::string link() const;

    // TODO: how to support includes?!
    // void include(std::string include);
    // std::string include() const;


    void mapping(const std::string &mapping);


    std::string mapping() const;


    void parent(const std::string &parent);


    std::string parent() const;


    std::vector<Section> sections() const;


    bool hasChildren() const;


    Section addSection(const std::string &name, const std::string &type);


    bool hasSection(const std::string &id) const;


    bool hasRelatedSection(const std::string &type) const;


    std::vector<Section> getRelatedSections(const std::string &type) const;


    bool removeSection(const std::string &id);


    std::vector<Property> properties() const;


    std::vector<Property> inheritedProperties() const;


    Property getProperty(const std::string &id) const;


    Property getPropertyByName(const std::string &name) const;


    bool hasProperty(const std::string &id) const;


    bool hasPropertyByName(const std::string &name) const;


    Property addProperty(const std::string &name);


    void removeProperty(const std::string &id);


    size_t propertyCount() const;

    bool operator==(const Section &other) const;

    bool operator!=(const Section &other) const;

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
