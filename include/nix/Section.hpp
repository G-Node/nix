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
#include <nix/base/ISection.hpp>
#include <nix/Property.hpp>

namespace nix {

class Section : virtual public base::ISection, public base::NamedEntity<base::ISection> {

public:

    Section() {}

    Section(const Section &other)
        : NamedEntity(other.impl_ptr)
    {
    }

    Section(const std::shared_ptr<base::ISection> &p_impl)
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

    //--------------------------------------------------
    // Methods for parent access
    //--------------------------------------------------

    /**
     * Test if the section has a parent.
     *
     * @return True if the section has a parent, false otherwise.
     */
    bool hasParent() const {
        return impl_ptr->hasParent();
    }

    /**
     * Returns the parent..
     *
     * @return string
     */
    Section parent() const {
        return impl_ptr->parent();
    }

    //--------------------------------------------------
    // Methods for child section access
    //--------------------------------------------------

    /**
     * Return the number of children in this section.
     *
     * @return The number of child sections.
     */
    size_t childCount() const {
        return impl_ptr->childCount();
    }

    /**
     * Check if a specific child exists.
     *
     * @param id    The id of the child section.
     *
     * @return True if the child exists false otherwise.
     */
    bool hasChild(const std::string &id) const {
        return impl_ptr->hasChild(id);
    }

    /**
     * Get a specific child by its id.
     *
     * @param id    The id of the child.
     *
     * @return The child section.
     */
    Section getChild(const std::string &id) const {
        return impl_ptr->getChild(id);
    }


    /**
     * Returns the subsections
     *
     * @return vector of direct subsections.
     */
    std::vector<Section> children() const {
        return impl_ptr->children();
    }

    /**
     *  Adds a new child section.
     *
     *  @param name: the name of the new section
     *  @param type: the type of the section
     *
     *  @return the new section.
     */
    Section createChild(const std::string &name, const std::string &type) {
        return impl_ptr->createChild(name, type);
    }


    /**
     * Remove a subsection from this Section.
     *
     * @param string the id of target section.
     *
     * @return bool successful or not
     */
    bool removeChild(const std::string &id) {
        return impl_ptr->removeChild(id);
    }

    //--------------------------------------------------
    // Methods for property access
    //--------------------------------------------------

    /**
     * Returns the number of properties of this section.
     *
     * @return The number of Properties
     */
    size_t propertyCount() const {
        return impl_ptr->propertyCount();
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
     * Returns the Property identified by id.
     *
     * @return Property
     */
    Property getProperty(const std::string &id) const {
        return impl_ptr->getProperty(id);
    }

    /**
     * Returns all Properties.
     *
     * @return vector<Property>
     */
    std::vector<Property> properties() const {
        return impl_ptr->properties();
    }

    /**
     * Add a Property to this section.
     *
     * @param string the name of the Property.
     *
     * @return the Property
     */
    Property createProperty(const std::string &name) {
        return impl_ptr->createProperty(name);
    }

    /**
     * Removes the Property that is identified by the id.#
     *
     * @param string the id.
     */
    bool removeProperty(const std::string &id) {
        return impl_ptr->removeProperty(id);
    }

};


} // namespace nix

#endif // NIX_SECTION_H
