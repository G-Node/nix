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
class Value;

namespace base {

/**
 * @brief Interface for implementations of the Section entity.
 *
 * See {@link nix::Section} for a more detailed description.
 */
class NIXAPI ISection : virtual public base::INamedEntity  {

public:

    //--------------------------------------------------
    // Attribute getter and setter
    //--------------------------------------------------

    /**
     * @brief Set the repository in which a section of this type is defined.
     *
     * Usually this information is provided in the form of an URL
     *
     * @param repository        URL to the repository.
     */
    virtual void repository(const std::string &repository) = 0;

    /**
     * @brief Gets the repository URL.
     *
     * @return The URL to the repository.
     */
    virtual boost::optional<std::string> repository() const = 0;

    /**
     * @brief Deleter for the repository.
     *
     * @param t         None
     */
    virtual void repository(const boost::none_t t) = 0;

    /**
     * @brief Establish a link to another section.
     *
     * The linking section inherits the properties defined in the linked section.
     * Properties of the same name are overridden.
     *
     * @param id        The id of the section that should be linked.
     */
    virtual void link(const std::string &id) = 0;

    /**
     * @brief Get the linked section.
     *
     * @return The linked section. If no section was linked a null
     *         Section will be returned.
     */
    virtual Section link() const = 0;

    /**
     * @brief Deleter for the linked section.
     *
     * This just removes the link between both sections, but does not remove
     * the linked section from the file.
     *
     * @param t         None
     */
    virtual void link(const none_t t) = 0;

    /**
     * @brief Sets the mapping information for this section.
     *
     * The mapping is provided as a path or URL to another section.
     *
     * @param mapping   The mapping information to this section.
     */
    virtual void mapping(const std::string &mapping) = 0;

    /**
     * @brief Gets the mapping information.
     *
     * @return The mapping information.
     */
    virtual boost::optional<std::string> mapping() const = 0;

    /**
     * @brief Deleter for the mapping information.
     *
     * @param t         None
     */
    virtual void mapping(const none_t t) = 0;

    //--------------------------------------------------
    // Methods for parent access
    //--------------------------------------------------

    /**
     * @brief Returns the parent section.
     *
     * Each section which is not a root section has a parent.
     *
     * @return The parent section. If the section has no parent, a null
     *         section will be returned.
     */
    virtual Section parent() const = 0;

    //--------------------------------------------------
    // Methods for child section access
    //--------------------------------------------------

    /**
     * @brief Get the number of child section of the section.
     *
     * @return The number of child sections.
     */
    virtual size_t sectionCount() const = 0;

    /**
     * @brief Checks whether a section has a certain child section.
     *
     * @param id        The id of requested section.
     *
     * @return True if the section is a child, false otherwise.
     */
    virtual bool hasSection(const std::string &id) const = 0;

    /**
     * @brief Get a specific child section by its id.
     *
     * @param id        The id of the child section.
     *
     * @return The child section.
     */
    virtual Section getSection(const std::string &id) const = 0;

    /**
     * @brief Get a child section by its index.
     *
     * @param index     The index of the child.
     *
     * @return The specified child section.
     */
    virtual Section getSection(size_t index) const = 0;

    /**
     *  @brief Adds a new child section.
     *
     *  @param name     The name of the new section
     *  @param type     The type of the section
     *
     *  @return The new child section.
     */
    virtual Section createSection(const std::string &name, const std::string &type) = 0;

    /**
     * @brief Deletes a section from the section.
     *
     * @param id        The id of the child section to delete.
     *
     * @return True if the section was deleted, false otherwise.
     */
    virtual bool deleteSection(const std::string &id) = 0;

    //--------------------------------------------------
    // Methods for property access
    //--------------------------------------------------

    /**
     * @brief Gets the number of properties of this section.
     *
     * @return The number of Properties
     */
    virtual size_t propertyCount() const = 0;

    /**
     * @brief Checks if a Property with this id exists in this Section.
     *
     * @param id        The id of the property.
     *
     * @return True if the property exists, false otherwise.
     */
    virtual bool hasProperty(const std::string &id) const = 0;

    /**
     * @brief Gets the Property identified by its id.
     *
     * @param id        The id of the property.
     *
     * @return The specified property.
     */
    virtual Property getProperty(const std::string &id) const = 0;

    /**
     * @brief Gets the property defined by its index.
     *
     * @param index     The index of the property
     *
     * @return The property.
     */
    virtual Property getProperty(size_t index) const = 0;

    /**
     * @brief Checks if a property with a certain name exists.
     *
     * @param name      The name of the property.
     *
     * @return True if a property with the given name exists false otherwise.
     */
    virtual bool hasPropertyWithName(const std::string &name) const = 0;

    /**
     * @brief Returns a property identified by its name.
     *
     * @param name      The name of the property.
     *
     * @return The found property.
     */
    virtual Property getPropertyByName(const std::string &name) const = 0;

    /**
     * @brief Add a new Property to the Section.
     *
     * @param name      The name of the property.
     * @param value     The Value to be stored. 
     *
     * @return The newly created property.
     */
    virtual Property createProperty(const std::string &name, const Value &value) = 0;

    /**
     * @brief Add a new Property that contains multiple values to the Section.
     *
     * @param name      The name of the property.
     * @param values    A vector of Values.
     *
     * @return The newly created property.
     */
    virtual Property createProperty(const std::string &name, const std::vector<Value> &values) = 0;

    /**
     * @brief Delete the Property identified by its id.
     *
     * @param id        The id of the property.
     *
     * @return True if the property was deleted, false otherwise.
     */
    virtual bool deleteProperty(const std::string &id) = 0;

    /**
     * @brief Destructor
     */
    virtual ~ISection() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_SECTION_H
