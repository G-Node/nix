// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_SECTION_H
#define NIX_SECTION_H

#include <nix/util/filter.hpp>
#include <nix/base/NamedEntity.hpp>
#include <nix/base/ISection.hpp>
#include <nix/Property.hpp>
#include <nix/DataType.hpp>
#include <nix/Platform.hpp>
#include <nix/types.hpp>
#include <nix/ObjectType.hpp>

#include <memory>
#include <functional>
#include <string>
#include <cstdlib>

namespace nix {


class NIXAPI Section : public base::NamedEntity<base::ISection> {

public:

    /**
     * @brief Constructor that creates an uninitialized Section.
     *
     * Calling any method on an uninitialized section will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a section is initialized:
     *
     * ~~~
     * Section e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    Section();

    /**
     * @brief Constructor that creates a null Section.
     */
    Section(std::nullptr_t ptr);

    /**
     * @brief Copy constructor.
     *
     * Copying of all NIX front facing objects like Section is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
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

    /**
     * @brief Set the repository in which a section of this type is defined.
     *
     * Usually this information is provided in the form of an URL
     *
     * @param repository        URL to the repository.
     */
    void repository(const std::string &repository);

    /**
     * @brief Gets the repository URL.
     *
     * @return The URL to the repository.
     */
    boost::optional<std::string> repository() const {
        return backend()->repository();
    }

    /**
     * @brief Deleter for the repository.
     *
     * @param t         None
     */
    void repository(const boost::none_t t) {
        backend()->repository(t);
    }

    /**
     * @brief Establish a link to another section.
     *
     * The linking section inherits the properties defined in the linked section.
     * Properties of the same name are overridden.
     *
     * @param id        The id of the section that should be linked.
     */
    void link(const std::string &id);

    /**
     * @brief Establish a link to another section.
     *
     * The linking section inherits the properties defined in the linked section.
     * Properties of the same name are overridden.
     *
     * @param link      The section to link with.
     */
    void link(const Section &link);

    /**
     * @brief Get the linked section.
     *
     * @return The linked section. If no section was linked a null
     *         Section will be returned.
     */
    Section link() const {
        return backend()->link();
    }

    /**
     * @brief Deleter for the linked section.
     *
     * This just removes the link between both sections, but does not remove
     * the linked section from the file.
     *
     * @param t         None
     */
    void link(const boost::none_t t) {
        backend()->link(t);
    }

    /**
     * @brief Sets the mapping information for this section.
     *
     * The mapping is provided as a path or URL to another section.
     *
     * @param mapping   The mapping information to this section.
     */
    void mapping(const std::string &mapping);

    /**
     * @brief Gets the mapping information.
     *
     * @return The mapping information.
     */
    boost::optional<std::string> mapping() const {
        return backend()->mapping();
    }

    /**
     * @brief Deleter for the mapping information.
     *
     * @param t         None
     */
    void mapping(const boost::none_t t) {
        backend()->mapping(t);
    }

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
    Section parent() const {
        return backend()->parent();
    }

    //--------------------------------------------------
    // Methods for child section access
    //--------------------------------------------------

    /**
     * @brief Get the number of child section of the section.
     *
     * @return The number of child sections.
     */
    ndsize_t sectionCount() const {
        return backend()->sectionCount();
    }

    /**
     * @brief Checks whether a section has a certain child section.
     *
     * @param name_or_id        Name or id of requested section.
     *
     * @return True if the section is a child, false otherwise.
     */
    bool hasSection(const std::string &name_or_id) const {
        return backend()->hasSection(name_or_id);
    }

    /**
     * @brief Checks whether a section has a certain child section.
     *
     * @param section   The section to check.
     *
     * @return True if the section is a child, false otherwise.
     */
    bool hasSection(const Section &section) const;

    /**
     * @brief Get a specific child section by its name or id.
     *
     * @param name_or_id  The name or the ID of the child section.
     *
     * @return The child section.
     */
    Section getSection(const std::string &name_or_id) const {
        return backend()->getSection(name_or_id);
    }

    /**
     * @brief Get a child section by its index.
     *
     * @param index     The index of the child.
     *
     * @return The specified child section.
     */
    virtual Section getSection(ndsize_t index) const;

    /**
     * @brief Get all direct child sections of the section.
     *
     * The parameter filter can be used to filter sections by various
     * criteria. By default a filter is used that accepts all sections.
     *
     * @param filter    A filter function.
     *
     * @return A vector containing the matching child sections.
     */
    std::vector<Section> sections(const util::Filter<Section>::type &filter = util::AcceptAll<Section>()) const;

    /**
     * @brief Get all descendant sections of the section recursively.
     *
     * This method traverses the sub-tree of all child sections of the section. The traversal
     * is accomplished via breadth first and can be limited in depth. On each node or
     * section a filter is applied. If the filter returns true the respective section
     * will be added to the result list.
     * By default a filter is used that accepts all sections.
     *
     * @param filter       A filter function.
     * @param max_depth    The maximum depth of traversal.
     *
     * @return A vector containing the matching descendant sections.
     */
    std::vector<Section> findSections(const util::Filter<Section>::type &filter = util::AcceptAll<Section>(),
                                      size_t max_depth = std::numeric_limits<size_t>::max()) const;

    /**
     * @brief Find all related sections of the section.
     *
     * @param filter       A filter function.
     *
     * @return A vector containing all filtered related sections.
     */
    std::vector<Section> findRelated(const util::Filter<Section>::type &filter = util::AcceptAll<Section>()) const;

    /**
     *  @brief Adds a new child section.
     *
     *  @param name     The name of the new section
     *  @param type     The type of the section
     *
     *  @return The new child section.
     */
    Section createSection(const std::string &name, const std::string &type);

    /**
     * @brief Deletes a section from the section.
     *
     * @param name_or_id        Name or id of the child section to delete.
     *
     * @return True if the section was deleted, false otherwise.
     */
    bool deleteSection(const std::string &name_or_id) {
        return backend()->deleteSection(name_or_id);
    }

    /**
     * @brief Deletes a subsection from this Section.
     *
     * @param section   The section to delete.
     *
     * @return bool successful or not
     */
    bool deleteSection(const Section &section);

    //--------------------------------------------------
    // Methods for property access
    //--------------------------------------------------

    /**
     * @brief Gets the number of properties of this section.
     *
     * @return The number of Properties
     */
    ndsize_t propertyCount() const {
        return backend()->propertyCount();
    }

    /**
     * @brief Checks if a Property with this name/id exists in this Section.
     *
     * @param name_or_id    Name or id of the property.
     *
     * @return True if the property exists, false otherwise.
     */
    bool hasProperty(const std::string &name_or_id) const {
        return backend()->hasProperty(name_or_id);
    }

    /**
     * @brief Checks if a Property exists in this Section.
     *
     * @param property      The Property to check.
     *
     * @return True if the property exists, false otherwise.
     */
    bool hasProperty(const Property &property) const;

    /**
     * @brief Gets the Property identified by its name or id.
     *
     * @param name_or_id    Name or id of the property.
     *
     * @return The specified property.
     */
    Property getProperty(const std::string &name_or_id) const {
        return backend()->getProperty(name_or_id);
    }

    /**
     * @brief Gets the property defined by its index.
     *
     * @param index     The index of the property
     *
     * @return The property.
     */
    Property getProperty(ndsize_t index) const {
        return backend()->getProperty(index);
    }

    /**
     * @brief Get all properties of the section.
     *
     * The parameter filter can be used to filter properties by various
     * criteria. By default a filter is used that accepts all properties.
     *
     * @param filter    A filter function.
     *
     * @return A vector containing the matching properties.
     */
    std::vector<Property> properties(const util::Filter<Property>::type &filter=util::AcceptAll<Property>()) const;

    /**
     * Returns all Properties inherited from a linked section.
     * This list may include Properties that are locally overridden.
     *
     * @return All inherited properties as a vector.
     */
    std::vector<Property> inheritedProperties() const;

    /**
     * @brief Add a new Property that does not have any Values to this Section.
     *
     * @param name     The name of the property.
     * @param dtype    The DataType of the property.
     *
     * @return The newly created property
     */
    Property createProperty(const std::string &name, const DataType &dtype);

    /**
     * @brief Add a new Property to the Section.
     *
     * @param name      The name of the property.
     * @param value     The Value to be stored.
     *
     * @return The newly created property.
     */
    Property createProperty(const std::string &name, const Value &value);

    /**
     * @brief Add a new Property with values to the Section.
     *
     * @param name      The name of the property.
     * @param values    The values of the created property.
     *
     * @return The newly created property.
     */
    Property createProperty(const std::string &name, const std::vector<Value> &values);

    /**
     * @brief Delete the Property identified by its name or id.
     *
     * @param name_or_id    Name or id of the property.
     *
     * @return True if the property was deleted, false otherwise.
     */
    bool deleteProperty(const std::string &name_or_id) {
        return backend()->deleteProperty(name_or_id);
    }

    /**
     * @brief Deletes the Property from this section.
     *
     * @param property      The Property to delete.
     *
     * @return True if the property was deleted, false otherwise.
     */
    bool deleteProperty(const Property &property);

    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------

    /**
     * @brief Find the DataArrays that refer to this Section in the metadata field.
     *
     * @param b The Block in which the search should be performed.
     * @return Vector of DataArrays.
     */
    std::vector<nix::DataArray> referringDataArrays(const nix::Block &b) const;

    /**
     * @brief Find the DataArrays that refer to this Section in the metadata field.
     * Search is performed in the whole file.
     *
     * @return Vector of DataArrays.
     */
    std::vector<nix::DataArray> referringDataArrays() const;


    /**
     * @brief Find the Tags that refer to this Section in the metadata field.
     * Search is performed in the whole file.
     *
     * @return std::vector of Tags.
     */
    std::vector<nix::Tag> referringTags() const;


    /**
     * @brief Find the Tags that refer to this Section in the metadata field.
     *
     * @param b The Block in which the search should be performed.
     * @return std::vector of Tags.
     */
    std::vector<nix::Tag> referringTags(const nix::Block &b) const;


    /**
     * @brief Find the DataArrays that refer to this Section in the metadata field.
     * Search is performed in the whole file.
     *
     * @return std::vector of MultiTags.
     */
    std::vector<nix::MultiTag> referringMultiTags() const;


    /**
     * @brief Find the MultiTags that refer to this Section in the metadata field.
     *
     * @param b The Block in which the search should be performed.
     * @return std::vector of MultiTags.
     */
    std::vector<nix::MultiTag> referringMultiTags(const nix::Block &b) const;


    /**
     * @brief Find the Sources that refer to this Section in the metadata field.
     *
     * @param b The Block in which the search should be performed.
     * @return std::vector of Sources.
     */
    std::vector<nix::Source> referringSources() const;


    /**
     * @brief Find the Sources that refer to this Section in the metadata field.
     * Search is performed in the whole file.
     *
     * @return std::vector of Sources.
     */
    std::vector<nix::Source> referringSources(const nix::Block &b) const;

    /**
     * @brief Find the Blocks that refer to this Section in the metadata field.
     *
     * @return std::vector of Blocks.
     */
    std::vector<nix::Block> referringBlocks() const;

    /**
     * @brief Assignment operator for none.
     */
    Section &operator=(const none_t &t) {
        ImplContainer::operator=(t);
        return *this;
    }

    /**
     * @brief Output operator
     */
    NIXAPI friend std::ostream& operator<<(std::ostream &out, const Section &ent);

private:

    std::vector<Section> findDownstream(const std::function<bool(Section)> &filter) const;

    std::vector<Section> findUpstream(const std::function<bool(Section)> &filter) const;

    std::vector<Section> findSideways(const std::function<bool(Section)> &filter, const std::string &caller_id) const;

    size_t tree_depth() const;
};

template<>
struct objectToType<nix::Section> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::Section;
    typedef nix::base::ISection backendType;
};

} // namespace nix

#endif // NIX_SECTION_H
