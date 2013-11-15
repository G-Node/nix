// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_SECTION_H
#define NIX_I_SECTION_H

#include <string>
#include <vector>

#include <nix/base/IEntity.hpp>
#include <nix/base/INamedEntity.hpp>

namespace nix {

class Section;
class Property;

// TODO find solution for tnode
class ISection : virtual public base::INamedEntity  {

public:

    //--------------------------------------------------
    // Attribute getter and setter
    //--------------------------------------------------

    /**
     * Set the repository in which a section of this type is defined. Usually
     * this information is provided in the form of an URL
     *
     * @param string the url of the repository.
     */
    virtual void repository(const std::string &repository) = 0;

    /**
     * Returns the repository url.
     *
     * @return string the url.
     */
    virtual std::string repository() const = 0;

    /**
     * Establish a link to another section. The linking section
     * inherits the properties defined in the linked section.
     * Properties of the same name are overridden.
     *
     * @param the id of the linked section.
     */
    virtual void link(const std::string &link) = 0;

    /**
     * Returns the id of the linked section.
     *
     * @return string the id.
     */
    virtual std::string link() const = 0;

    // TODO: how to support includes?!
    // void include(std::string include) = 0;
    // std::string include() const = 0;

    /**
     * Sets the mapping information for this section.
     *
     * @param string the mapping information.
     */
    virtual void mapping(const std::string &mapping) = 0;

    /**
     * Return the mapping information.
     *
     * @return string
     */
    virtual std::string mapping() const = 0;

    // TODO work with sections here

    /**
     * Sets the id of the parent section.
     *
     * @param string the id.
     */
    virtual void parent(const std::string &parent) = 0;

    /**
     * Returns the parent information.
     *
     * @return string
     */
    virtual std::string parent() const = 0;

    //--------------------------------------------------
    // Methods for child section access
    //--------------------------------------------------

    /**
     * Returns the subsections
     *
     * @return vector of direct subsections.
     */
    virtual std::vector<Section> sections() const = 0;

    /**
     * Returns whether or not this section has child sections.
     */
    virtual bool hasChildren() const = 0;

    /**
     *  Adds a new child section.
     *
     *  @param name: the name of the new section
     *  @param type: the type of the section
     *
     *  @return the new section.
     */
    virtual Section addSection(const std::string &name, const std::string &type) = 0;

    /**
     * Performs a search on the tree starting at this section and returns whether a section with
     * the specified id exists.
     *
     * @param id: string the id of requested section
     * @param depth: uint (default 0). The depth of the search. 0 indicates unlimited depth.
     *
     * @return bool
     */
    virtual bool hasSection(const std::string &id) const = 0;

    /**
     * Determines whether this section has a related section of the specified type.
     *
     * @param string the type
     *
     * @return bool
     */
    virtual bool hasRelatedSection(const std::string &type) const = 0;

    /**
     * Returns the sections of the given type found on the same level of relation.
     *
     * @param string the type
     *
     * @return vector<Section> the related sections
     */
    virtual std::vector<Section> getRelatedSections(const std::string &type) const = 0;

    /**
     * Remove a subsection from this Section.
     *
     * @param string the id of target section.
     *
     * @return bool successful or not
     */
    virtual bool removeSection(const std::string &id) = 0;

    //--------------------------------------------------
    // Methods for property access
    //--------------------------------------------------

    /**
     * Returns all Properties.
     *
     * @return vector<Property>
     */
    virtual std::vector<Property> properties() const = 0;

    /**
     * Returns all Properties inherited from a linked section.
     * This list may include Properties that are locally overridden.
     *
     * @return vector<Property>
     */
    virtual std::vector<Property> inheritedProperties() const = 0;

    /**
     * Returns the Property identified by id.
     *
     * @return Property
     */
    virtual Property getProperty(const std::string &id) const = 0;

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
    virtual Property getPropertyByName(const std::string &name) const = 0;

    /**
     * Checks if a Property with this id exists in this Section.
     *
     * @param string the id.
     */
    virtual bool hasProperty(const std::string &id) const = 0;

    /**
     * Checks if a Property with the given name exists.
     *
     * @param string the name
     */
    virtual bool hasPropertyByName(const std::string &name) const = 0;

    /**
     * Add a Property to this section.
     *
     * @param string the name of the Property.
     *
     * @return the Property
     */
    virtual Property addProperty(const std::string &name) = 0;

    /**
     * Removes the Property that is identified by the id.#
     *
     * @param string the id.
     */
    virtual void removeProperty(const std::string &id) = 0;

};

} // namespace nix

#endif // NIX_I_SECTION_H
