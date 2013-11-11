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


    /**
     * Set the repository in which a section of this type is defined. Usually
     * this information is provided in the form of an URL
     *
     * @param string the url of the repository.
     */
    //void repository(const std::string &repository);

    /**
     * Returns the repository url.
     *
     * @return string the url.
     */
    //std::string repository() const;

    /**
     * Establish a link to another section. The linking section
     * inherits the properties defined in the linked section.
     * Properties of the same name are overridden.
     *
     * @param the id of the linked section.
     */
    //void link(const std::string &link);

    /**
     * Returns the id of the linked section.
     *
     * @return string the id.
     */
    //std::string link() const;

    // TODO: how to support includes?!
    // void include(std::string include);
    // std::string include() const;

    /**
     * Sets the mapping information for this section.
     *
     * @param string the mapping information.
     */
    //void mapping(const std::string &mapping);

    /**
     * Return the mapping information.
     *
     * @return string
     */
    //std::string mapping() const;

    /**
     * Sets the id of the parent section.
     *
     * @param string the id.
     */
    //void parent(const std::string &parent);

    /**
     * Returns the parent information.
     *
     * @return string
     */
    //std::string parent() const;

    /**
     * Returns the subsections
     *
     * @return vector of direct subsections.
     */
    //std::vector<Section> sections() const;

    /**
     * Returns whether or not this section has child sections.
     */
    //bool hasChildren() const;

    /**
     *  Adds a new child section.
     *
     *  @param name: the name of the new section
     *  @param type: the type of the section
     *
     *  @return the new section.
     */
    //Section addSection(const std::string &name, const std::string &type);

    /**
     * Performs a search on the tree starting at this section and returns whether a section with
     * the specified id exists.
     *
     * @param id: string the id of requested section
     * @param depth: uint (default 0). The depth of the search. 0 indicates unlimited depth.
     *
     * @return bool
     */
    //bool hasSection(const std::string &id) const;

    /**
     * Determines whether this section has a related section of the specified type.
     *
     * @param string the type
     *
     * @return bool
     */
    //bool hasRelatedSection(const std::string &type) const;

    /**
     * Returns the sections of the given type found on the same level of relation.
     *
     * @param string the type
     *
     * @return vector<Section> the related sections
     */
    //std::vector<Section> getRelatedSections(const std::string &type) const;

    /**
     * Remove a subsection from this Section.
     *
     * @param string the id of target section.
     *
     * @return bool successful or not
     */
    //bool removeSection(const std::string &id);

    /**
     * Returns all Properties.
     *
     * @return vector<Property>
     */
    //std::vector<Property> properties() const;

    /**
     * Returns all Properties inherited from a linked section.
     * This list may include Properties that are locally overridden.
     *
     * @return vector<Property>
     */
    //std::vector<Property> inheritedProperties() const;

    /**
     * Returns the Property identified by id.
     *
     * @return Property
     */
    //Property getProperty(const std::string &id) const;

    /**
     * Returns the Property that is defined by name.
     * Method tries to locate the Property also in linked Sections (if any).
     *
     * Raises runtime exception if not found. Check with hasPropertyByName.
     *
     * @param string name
     *
     * @return Property
     */
    //Property getPropertyByName(const std::string &name) const;

    /**
     * Checks if a Property with this id exists in this Section.
     *
     * @param string the id.
     */
    //bool hasProperty(const std::string &id) const;

    /**
     * Checks if a Property with the given name exists.
     *
     * @param string the name
     */
    //bool hasPropertyByName(const std::string &name) const;

    /**
     * Add a Property to this section.
     *
     * @param string the name of the Property.
     *
     * @return the Property
     */
    //Property addProperty(const std::string &name);

    /**
     * Removes the Property that is identified by the id.#
     *
     * @param string the id.
     */
    //void removeProperty(const std::string &id);

    /**
     * The Number of properties.
     *
     * @param size_t
     */
    //size_t propertyCount() const;

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
