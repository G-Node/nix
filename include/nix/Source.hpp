// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_SOURCE_H
#define NIX_SOURCE_H

#include <nix/util/filter.hpp>
#include <nix/types.hpp>
#include <nix/base/EntityWithMetadata.hpp>
#include <nix/base/ISource.hpp>
#include <nix/ObjectType.hpp>

#include <nix/Platform.hpp>

#include <ostream>
#include <string>

namespace nix {

/**
 * @brief A class that describes the provenance of other entities of the NIX data model.
 *
 * The Source is conceptually a rather simple entity. I t is used to note the provenance of
 * the data and offers the opportunity to bind additional metadata.
 * One special feature of the Source is the possibility to contain other sources as children
 * thus building up a tree of sources.
 * This can, for example, be used to specify that a source electrode array contains
 * multiple electrodes as its child sources.
 */
class NIXAPI Source : public base::EntityWithMetadata<base::ISource> {

public:

    /**
     * @brief Constructor that creates an uninitialized Source.
     *
     * Calling any method on an uninitialized source will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a source is initialized:
     *
     * ~~~
     * Source e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    Source();

    /**
     * @brief Copy constructor.
     *
     * Copying of all NIX front facing objects like Source is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The source to copy.
     */
    Source(const Source &other);

    /**
     * @brief Constructor that creates a new source from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    Source(const std::shared_ptr<base::ISource> &p_impl);

    /**
     * @brief Constructor with move semantics that creates a new source from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    Source(std::shared_ptr<base::ISource> &&ptr);

    //--------------------------------------------------
    // Methods concerning child sources
    //--------------------------------------------------

    /**
     * @brief Checks if this source has a specific source as direct descendant.
     *
     * @param name_or_id        The name or id of the source.
     *
     * @return True if a source with the given name/id is a direct descendant, false
     *         otherwise.
     */
    bool hasSource(const std::string &name_or_id) const {
        return backend()->hasSource(name_or_id);
    }

    /**
     * @brief Checks if this source has a specific source as direct descendant.
     *
     * @param source         The Source.
     *
     * @return True if a source is a direct descendant, false otherwise.
     */
    bool hasSource(const Source &source) const;

    /**
     * @brief Retrieves a specific child source that is a direct descendant.
     *
     * @param name_or_id        The name or id of the source.
     *
     * @return The source with the given name/id. If it doesn't exist an exception
     *         will be thrown.
     */
    Source getSource(const std::string &name_or_id) const {
        return backend()->getSource(name_or_id);
    }

    /**
     * @brief Retrieves a specific source by index.
     *
     * @param index     The index of the source.
     *
     * @return The source at the specified index.
     */
    Source getSource(ndsize_t index) const {
        return backend()->getSource(index);
    }

    /**
     * @brief Returns the number of sources that are direct descendants of this source.
     *
     * @return The number of direct child sources.
     */
    ndsize_t sourceCount() const {
        return backend()->sourceCount();
    }

    /**
     * @brief Get all direct child sources associated with this source.
     *
     * The parameter filter can be used to filter sources by various
     * criteria. By default a filter is used that accepts all sources.
     *
     * @param filter    A filter function.
     *
     * @return A vector containing the matching child sources.
     */
    std::vector<Source> sources(const util::Filter<Source>::type &filter = util::AcceptAll<Source>()) const;

    /**
     * @brief Get all descendant sources of the source recursively.
     *
     * This method traverses the sub-tree of all child sources of the source. The traversal
     * is accomplished via breadth first and can be limited in depth. On each node or
     * source a filter is applied. If the filter returns true the respective source
     * will be added to the result list.
     * By default a filter is used that accepts all sources.
     *
     * @param filter       A filter function.
     * @param max_depth    The maximum depth of traversal.
     *
     * @return A vector containing the matching descendant sources.
     */
    std::vector<Source> findSources(const util::Filter<Source>::type &filter = util::AcceptAll<Source>(),
                                    size_t max_depth = std::numeric_limits<size_t>::max()) const;

    /**
     * @brief Create a new root source.
     *
     * @param name      The name of the source to create.
     * @param type      The type of the source.
     *
     * @return The created source object.
     */
    Source createSource(const std::string &name, const std::string &type);

    /**
     * @brief Delete a root source and all its child sources from
     *        the source.
     *
     * @param name_or_id        The name or id of the source to remove.
     *
     * @return True if the source was deleted, false otherwise.
     */
    bool deleteSource(const std::string &name_or_id) {
        return backend()->deleteSource(name_or_id);
    }

    /**
     * @brief Delete a root source and all its child sources from
     *        the source.
     *
     * @param source        The Source to delete.
     *
     * @return True if the source was deleted, false otherwise.
     */
    bool deleteSource(const Source &source);

    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------

    /**
     * @brief Returns the parent Source of this Source. Method performs a search,
     * may thus not be the most efficient way.
     *
     * @return The parent Source if there is any, an empty Source otherwise.
     */
    nix::Source parentSource() const;

    /**
     * Returns all DataArrays that refer to this Source.
     *
     * @return std::vector of DataArrays.
     */
    std::vector<nix::DataArray> referringDataArrays() const;


    /**
     * Returns all Tags that refer to this Source.
     *
     * @return std::vector of Tags.
     */
    std::vector<nix::Tag> referringTags() const;


    /**
     * Returns all MultiTags that refer to this Source.
     *
     * @return std::vector of MultiTags.
     */
    std::vector<nix::MultiTag> referringMultiTags() const;


    /**
     * @brief Assignment operator for none.
     */
    Source &operator=(const none_t &t) {
        ImplContainer::operator=(t);
        return *this;
    }

    /**
     * @brief Output operator
     */
    NIXAPI friend std::ostream& operator<<(std::ostream &out, const Source &ent);

};

template<>
struct objectToType<nix::Source> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::Source;
    typedef nix::base::ISource backendType;
};

} // namespace nix

#endif // NIX_SOURCE_H
