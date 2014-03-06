// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_SECTION_H
#define NIX_SECTION_H

#include <limits>
#include <functional>

#include <nix/util/util.hpp>
#include <nix/util/filter.hpp>
#include <nix/base/NamedEntity.hpp>
#include <nix/base/ISection.hpp>
#include <nix/Property.hpp>

#include <nix/Platform.hpp>

namespace nix {



class NIXAPI Section : virtual public base::ISection, public base::NamedEntity<base::ISection> {
    
public:


    Section();


    Section(std::nullptr_t ptr);


    Section(const Section &other);


    Section(const std::shared_ptr<base::ISection> &p_impl);


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
        backend()->repository(repository);
    }

    /**
     * Returns the repository url.
     *
     * @return boost::optional<string> the url.
     */
    boost::optional<std::string> repository() const {
        return backend()->repository();
    }
    
    /**
     * Deleter for the repository url.
     *
     * @param boost::none_t.
     */
    void repository(const boost::none_t t) {
        backend()->repository(t);
    }
    
    /**
     * Establish a link to another section. The linking section
     * inherits the properties defined in the linked section.
     * Properties of the same name are overridden.
     *
     * @param the id of the linked section.
     */
    void link(const Section &link) {
        backend()->link(link);
    }

    /**
     * Returns the id of the linked section.
     *
     * @return string the id.
     */
    Section link() const {
        return backend()->link();
    }
    
    /**
     * Deleter for the reference to the linked section.
     * NOTE: this function does not affect the linked section object.
     *
     * @param boost::none_t.
     */
    void link(const boost::none_t t) {
        backend()->link(t);
    }

    // TODO: how to support includes?!

    /**
     * Sets the mapping information for this section.
     *
     * @param string the mapping information.
     */
    void mapping(const std::string &mapping) {
        backend()->mapping(mapping);
    }

    /**
     * Return the mapping information.
     *
     * @return boost::optional<std::string>
     */
    boost::optional<std::string> mapping() const {
        return backend()->mapping();
    }
    
    /**
     * Deleter for the mapping information.
     *
     * @param boost::none_t.
     */
    void mapping(const boost::none_t t) {
        backend()->mapping(t);
    }
    
    //--------------------------------------------------
    // Methods for parent access
    //--------------------------------------------------

    /**
     * Returns the parent.
     *
     * @return string
     */
    Section parent() const {
        return backend()->parent();
    }

    //--------------------------------------------------
    // Methods for child section access
    //--------------------------------------------------

    /**
     * Return the number of children in this section.
     *
     * @return The number of child sections.
     */
    size_t sectionCount() const {
        return backend()->sectionCount();
    }

    /**
     * Check if a specific child exists.
     *
     * @param id    The id of the child section.
     *
     * @return True if the child exists false otherwise.
     */
    bool hasSection(const std::string &id) const {
        return backend()->hasSection(id);
    }

    /**
     * Get a specific child by its id.
     *
     * @param id    The id of the child.
     *
     * @return The child section.
     */
    Section getSection(const std::string &id) const {
        return backend()->getSection(id);
    }

    /**
     * Get a child section by its index.
     *
     * @param index The index of the child.
     *
     * @return The child section.
     */
    virtual Section getSection(size_t index) const {
        return backend()->getSection(index);
    }

    /**
     * Get sub sections associated with this section.
     *
     * The parameter "filter" is defaulted to giving back all sections. 
     * To use your own filter pass a lambda that accepts a "Section"
     * as parameter and returns a bool telling whether to get it or not.
     *
     * @param object filter function of type {@link nix::util::Filter::type}
     * @return object sub sections as a vector     
     */
    std::vector<Section> sections(util::AcceptAll<Section>::type filter
                                  = util::AcceptAll<Section>()) const
    {
        auto f = [this] (size_t i) { return getSection(i); };
        return getEntities<Section>(f,
                                    sectionCount(),
                                    filter);
    }

    /**
     * Go through the tree of sections originating from this section until
     * a max. level of "max_depth" and check for each section whether
     * to return it depending on predicate function "filter".
     * Return resulting vector of sections.
     * 
     * @param object filter function of type {@link nix::util::Filter::type}
     * @param int maximum depth to search tree
     * @return object vector of sections
     */
    std::vector<Section> findSections(std::function<bool(Section)> filter = util::AcceptAll<Section>(),
                                      size_t max_depth = std::numeric_limits<size_t>::max()) const;

    /**
     * Returns the sections of the given type found on the same level of relation.
     *
     * @param object filter function of the type {@link nix::util::Filter::type}
     *
     * @return vector<Section> the related sections
     */
    std::vector<Section> findRelated(std::function<bool(Section)> filter = util::AcceptAll<Section>()) const;

    /**
     *  Adds a new child section.
     *
     *  @param name: the name of the new section
     *  @param type: the type of the section
     *
     *  @return the new section.
     */
    Section createSection(const std::string &name, const std::string &type) {
        return backend()->createSection(name, type);
    }

    /**
     * Deletes a subsection from this Section.
     *
     * @param string the id of target section.
     *
     * @return bool successful or not
     */
    bool deleteSection(const std::string &id) {
        return backend()->deleteSection(id);
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
        return backend()->propertyCount();
    }

    /**
     * Checks if a Property with this id exists in this Section.
     *
     * @param string the id.
     */
    bool hasProperty(const std::string &id) const {
        return backend()->hasProperty(id);
    }

    /**
     * Returns the Property identified by id.
     *
     * @return Property
     */
    Property getProperty(const std::string &id) const {
        return backend()->getProperty(id);
    }

    /**
     * Returns the property defined by its index.
     *
     * @param index     The index of the property
     *
     * @return The property.
     */
    Property getProperty(size_t index) const {
        return backend()->getProperty(index);
    }

    /**
     * Checks if a property with a certian name exists.
     *
     * @param name      The name of the property.
     *
     * @return True if a property with the given name exists false otherwise.
     */
    bool hasPropertyWithName(const std::string &name) const {
        return backend()->hasPropertyWithName(name);
    }

    /**
     * Returns a property identified by its name.
     *
     * @param name      The name of the property.
     *
     * @return The found property.
     */
    Property getPropertyByName(const std::string &name) const {
        return backend()->getPropertyByName(name);
    }


    /**
     * Get properties associated with this section.
     *
     * The parameter "filter" is defaulted to giving back all properties. 
     * To use your own filter pass a lambda that accepts a "Property"
     * as parameter and returns a bool telling whether to get it or not.
     *
     * @param object filter function of type {@link nix::util::Filter::type}
     * @return object properties as a vector     
     */
    std::vector<Property> properties(util::AcceptAll<Property>::type filter
                                  = util::AcceptAll<Property>()) const
    {
        auto f = [this] (size_t i) { return getProperty(i); };
        return getEntities<Property>(f,
                                    propertyCount(),
                                    filter);
    }

    /**
     * Returns all Properties inherited from a linked section.
     * This list may include Properties that are locally overridden.
     *
     * @return vector<Property>
     */
    std::vector<Property> inheritedProperties() const;

    /**
     * Add a Property to this section.
     *
     * @param string the name of the Property.
     *
     * @return the Property
     */
    Property createProperty(const std::string &name) {
        return backend()->createProperty(name);
    }

    /**
     * Deletess the Property that is identified by the id.#
     *
     * @param string the id.
     */
    bool deleteProperty(const std::string &id) {
        return backend()->deleteProperty(id);
    }

    //------------------------------------------------------
    // Operators and other functions
    //------------------------------------------------------

    virtual Section &operator=(std::nullptr_t nullp) {
        nullify();
        return *this;
    }

    /**
     * Output operator
     */
    friend std::ostream& operator<<(std::ostream &out, const Section &ent);


private:

    std::vector<Section> findDownstream(std::function<bool(Section)> filter) const;

    std::vector<Section> findUpstream(std::function<bool(Section)> filter) const;

    std::vector<Section> findSideways(std::function<bool(Section)> filter, const std::string &caller_id) const;

    size_t tree_depth() const;
};


} // namespace nix

#endif // NIX_SECTION_H
