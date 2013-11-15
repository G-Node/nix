// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_SECTION_H
#define NIX_SECTION_H

#include <nix/base/NamedEntity.hpp>
#include <nix/ISection.hpp>
#include <nix/Property.hpp>

namespace nix {

class Section : virtual public ISection, public base::NamedEntity<ISection> {

public:

    Section() {}

    Section(const Section &other)
        : NamedEntity(other.impl_ptr)
    {
    }

    Section(const std::shared_ptr<ISection> &p_impl)
        : NamedEntity(p_impl)
    {
    }

    //--------------------------------------------------
    // Attribute getter and setter
    //--------------------------------------------------

    /**
     * Set the repository in which a section of this type is defined. Usually
     * this information is provided in the form of an URL
     *
     * @param string the url of the repository.
     */
    void repository(const std::string &repository) {
        impl_ptr->repository(repository);
    }

    /**
     * Returns the repository url.
     *
     * @return string the url.
     */
    std::string repository() const {
        return impl_ptr->repository();
    }

    /**
     * Establish a link to another section. The linking section
     * inherits the properties defined in the linked section.
     * Properties of the same name are overridden.
     *
     * @param the id of the linked section.
     */
    void link(const std::string &link) {
        impl_ptr->link(link);
    }

    /**
     * Returns the id of the linked section.
     *
     * @return string the id.
     */
    std::string link() const {
        return impl_ptr->link();
    }

    // TODO: how to support includes?!
    // void include(std::string include);
    // std::string include() const;

    /**
     * Sets the mapping information for this section.
     *
     * @param string the mapping information.
     */
    void mapping(const std::string &mapping) {
        impl_ptr->mapping(mapping);
    }

    /**
     * Return the mapping information.
     *
     * @return string
     */
    std::string mapping() const {
        return impl_ptr->mapping();
    }

    // TODO work with sections here

    /**
     * Sets the id of the parent section.
     *
     * @param string the id.
     */
    void parent(const std::string &id) {
        impl_ptr->parent(id);
    }

    /**
     * Returns the parent information.
     *
     * @return string
     */
    std::string parent() const {
        return impl_ptr->parent();
    }

    //--------------------------------------------------
    // Methods for child section access
    //--------------------------------------------------

    /**
     * Returns the subsections
     *
     * @return vector of direct subsections.
     */
    std::vector<Section> sections() const {
        return impl_ptr->sections();
    }

    /**
     * Returns whether or not this section has child sections.
     */
    bool hasChildren() const {
        return impl_ptr->hasChildren();
    }

    /**
     *  Adds a new child section.
     *
     *  @param name: the name of the new section
     *  @param type: the type of the section
     *
     *  @return the new section.
     */
    // TODO should this be named createSection?
    Section addSection(const std::string &name, const std::string &type) {
        return impl_ptr->addSection(name, type);
    }

    /**
     * Performs a search on the tree starting at this section and returns whether a section with
     * the specified id exists.
     *
     * @param id: string the id of requested section
     * @param depth: uint (default 0). The depth of the search. 0 indicates unlimited depth.
     *
     * @return bool
     */
    bool hasSection(const std::string &id) const {
        return impl_ptr->hasSection(id);
    }

    /**
     * Determines whether this section has a related section of the specified type.
     *
     * @param string the type
     *
     * @return bool
     */
    bool hasRelatedSection(const std::string &type) const {
        return impl_ptr->hasRelatedSection(type);
    }

    /**
     * Returns the sections of the given type found on the same level of relation.
     *
     * @param string the type
     *
     * @return vector<Section> the related sections
     */
    // TODO should this be called relatedSections?
    std::vector<Section> getRelatedSections(const std::string &type) const {
        return impl_ptr->getRelatedSections(type);
    }

    /**
     * Remove a subsection from this Section.
     *
     * @param string the id of target section.
     *
     * @return bool successful or not
     */
    bool removeSection(const std::string &id) {
        return impl_ptr->removeSection(id);
    }

    //--------------------------------------------------
    // Methods for property access
    //--------------------------------------------------

    /**
     * Returns all Properties.
     *
     * @return vector<Property>
     */
    std::vector<Property> properties() const {
        return impl_ptr->properties();
    }

    /**
     * Returns all Properties inherited from a linked section.
     * This list may include Properties that are locally overridden.
     *
     * @return vector<Property>
     */
    std::vector<Property> inheritedProperties() const {
        return impl_ptr->inheritedProperties();
    }

    /**
     * Returns the Property identified by id.
     *
     * @return Property
     */
    Property getProperty(const std::string &id) const {
        return impl_ptr->getProperty(id);
    }

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
    Property getPropertyByName(const std::string &name) const {
        return impl_ptr->getPropertyByName(name);
    }

    /**
     * Checks if a Property with this id exists in this Section.
     *
     * @param string the id.
     */
    bool hasProperty(const std::string &id) const {
        return impl_ptr->hasProperty(id);
    }

    /**
     * Checks if a Property with the given name exists.
     *
     * @param string the name
     */
    // TODO rename to hasPropertyWithName()?
    bool hasPropertyByName(const std::string &name) const {
        return impl_ptr->hasPropertyByName(name);
    }

    /**
     * Add a Property to this section.
     *
     * @param string the name of the Property.
     *
     * @return the Property
     */
    Property addProperty(const std::string &name) {
        return impl_ptr->addProperty(name);
    }

    /**
     * Removes the Property that is identified by the id.#
     *
     * @param string the id.
     */
    // TODO return type bool?
    void removeProperty(const std::string &id) {
        impl_ptr->removeProperty(id);
    }

};


} // namespace nix

#endif // NIX_SECTION_H
