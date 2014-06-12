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

    /**
     * @brief Constructor that creates a null Section.
     */
    Section();

    /**
     * @brief Constructor that creates a null Section.
     */
    Section(std::nullptr_t ptr);

    /**
     * @brief Copy constructor.
     *
     * @param other     The Section to copy.
     */
    Section(const Section &other);

    /**
     * @brief Constructor that creates a new Section from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    Section(const std::shared_ptr<base::ISection> &p_impl);

    /**
     * @brief Constructor with move semantics that creates a new Section from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    Section(std::shared_ptr<base::ISection> &&ptr);

    //--------------------------------------------------
    // Attribute getter and setter
    //--------------------------------------------------

    void repository(const std::string &repository) {
        backend()->repository(repository);
    }

    boost::optional<std::string> repository() const {
        return backend()->repository();
    }

    void repository(const boost::none_t t) {
        backend()->repository(t);
    }

    void link(const std::string &id) {
        backend()->link(id);
    }

    void link(const Section &link) {
        if (link == none) {
            backend()->link(none);
        } else {
            backend()->link(link.id());
        }
    }

    Section link() const {
        return backend()->link();
    }

    void link(const boost::none_t t) {
        backend()->link(t);
    }

    void mapping(const std::string &mapping) {
        backend()->mapping(mapping);
    }

    boost::optional<std::string> mapping() const {
        return backend()->mapping();
    }

    void mapping(const boost::none_t t) {
        backend()->mapping(t);
    }

    //--------------------------------------------------
    // Methods for parent access
    //--------------------------------------------------

    Section parent() const {
        return backend()->parent();
    }

    //--------------------------------------------------
    // Methods for child section access
    //--------------------------------------------------

    size_t sectionCount() const {
        return backend()->sectionCount();
    }

    bool hasSection(const std::string &id) const {
        return backend()->hasSection(id);
    }

    /**
     * @brief Checks whether a section has a certain child section.
     *
     * @param section   The section to check.
     *
     * @return True if the section is a child, false otherwise.
     */
    bool hasSection(const Section &section) const {
        if(section == none) {
            throw std::runtime_error("Section::hasSection: Empty Section entity given!");
        }
        return backend()->hasSection(section.id());
    }

    Section getSection(const std::string &id) const {
        return backend()->getSection(id);
    }

    virtual Section getSection(size_t index) const {
        return backend()->getSection(index);
    }

    /**
     * @brief Get all direct child sections of the section.
     *
     * The parameter filter can be used to filter sections by various
     * criteria. By default a filter is used that accepts every section.
     *
     * @param filter    A filter function.
     *
     * @return A vector containing the filtered child sections.
     */
    std::vector<Section> sections(util::Filter<Section>::type filter = util::AcceptAll<Section>()) const
    {
        auto f = [this] (size_t i) { return getSection(i); };
        return getEntities<Section>(f,
                                    sectionCount(),
                                    filter);
    }

    /**
     * @brief Get all descendant sections of the section recursively.
     *
     * This method traverses the sub-tree of all child sections of the section. The traversal
     * is accomplished via breadth first and can be limited by depth. On each node or
     * section a filter is applied. If the filter returns true the respective section
     * will be added to the result list.
     * By default a filter is used that accepts every section.
     *
     * @param filter       A filter function.
     * @param max_depth    The maximum depth of traversal.
     *
     * @return A vector containing the filtered descendant sections.
     */
    std::vector<Section> findSections(util::Filter<Section>::type filter = util::AcceptAll<Section>(),
                                      size_t max_depth = std::numeric_limits<size_t>::max()) const;

    /**
     * @brief Find all related sections of the section.
     *
     * @param filter       A filter function.
     *
     * @return A vector containing all filtered related sections.
     */
    std::vector<Section> findRelated(util::Filter<Section>::type filter = util::AcceptAll<Section>()) const;

    Section createSection(const std::string &name, const std::string &type) {
        return backend()->createSection(name, type);
    }

    bool deleteSection(const std::string &id) {
        return backend()->deleteSection(id);
    }

    /**
     * @brief Deletes a subsection from this Section.
     *
     * @param section   The section to delete.
     *
     * @return bool successful or not
     */
    bool deleteSection(const Section &section) {
        if(section == none) {
            throw std::runtime_error("Section::deleteSection: Empty Section entity given!");
        }
        return backend()->deleteSection(section.id());
    }

    //--------------------------------------------------
    // Methods for property access
    //--------------------------------------------------

    size_t propertyCount() const {
        return backend()->propertyCount();
    }

    bool hasProperty(const std::string &id) const {
        return backend()->hasProperty(id);
    }

    /**
     * @brief Checks if a Property exists in this Section.
     *
     * @param property      The Property to check.
     *
     * @return True if the property exists, false otherwise.
     */
    bool hasProperty(const Property &property) const {
        if(property == none) {
            throw std::runtime_error("Section::hasProperty: Empty Property entity given!");
        }
        return backend()->hasProperty(property.id());
    }

    Property getProperty(const std::string &id) const {
        return backend()->getProperty(id);
    }

    Property getProperty(size_t index) const {
        return backend()->getProperty(index);
    }

    bool hasPropertyWithName(const std::string &name) const {
        return backend()->hasPropertyWithName(name);
    }

    Property getPropertyByName(const std::string &name) const {
        return backend()->getPropertyByName(name);
    }

    /**
     * @brief Get all properties of the section.
     *
     * The parameter filter can be used to filter properties by various
     * criteria. By default a filter is used that accepts every property.
     *
     * @param filter    A filter function.
     *
     * @return A vector containing the filtered properties.
     */
    std::vector<Property> properties(util::Filter<Property>::type filter = util::AcceptAll<Property>()) const
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
     * @return All inherited properties as a vector.
     */
    std::vector<Property> inheritedProperties() const;

    Property createProperty(const std::string &name, const std::string &type) {
        return backend()->createProperty(name, type);
    }

    bool deleteProperty(const std::string &id) {
        return backend()->deleteProperty(id);
    }

    /**
     * @brief Deletes the Property from this section.
     *
     * @param property      The Property to delete.
     *
     * @return True if the property was deleted, false otherwise.
     */
    bool deleteProperty(const Property &property) {
        if(property == none) {
            throw std::runtime_error("Section::deleteProperty: Empty Property entity given!");
        }
        return backend()->deleteProperty(property.id());
    }

    //------------------------------------------------------
    // Operators and other functions
    //------------------------------------------------------

    /**
     * @brief Assignment operator for none.
     */
    virtual Section &operator=(none_t) {
        nullify();
        return *this;
    }

    /**
     * @brief Output operator
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
