// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_SOURCE_H
#define NIX_SOURCE_H

#include <limits>
#include <functional>

#include <nix/util/filter.hpp>
#include <nix/util/util.hpp>
#include <nix/base/EntityWithMetadata.hpp>
#include <nix/base/ISource.hpp>

#include <nix/Platform.hpp>

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
class NIXAPI Source : virtual public base::ISource, public base::EntityWithMetadata<base::ISource> {

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

    bool hasSource(const std::string &id) const {
        return backend()->hasSource(id);
    }

    /**
     * @brief Checks if this source has a specific source as direct descendant.
     *
     * @param source         The Source.
     *
     * @return True if a source is a direct descendant, false otherwise.
     */
    bool hasSource(const Source &source) const {
        if(source == none) {
            throw std::runtime_error("Source::hasSource: emtpy Source entity given!");
        }
        return backend()->hasSource(source.id());
    }

    Source getSource(const std::string &id) const {
        return backend()->getSource(id);
    }

    Source getSource(size_t index) const {
        return backend()->getSource(index);
    }

    size_t sourceCount() const {
        return backend()->sourceCount();
    }

    /**
     * @brief Get all direct child sources associated with this source.
     *
     * The parameter filter can be used to filter sources by various
     * criteria. By default a filter is used that accepts every source.
     *
     * @param filter    A filter function.
     *
     * @return A vector containing the filtered child sources.
     */
    std::vector<Source> sources(util::Filter<Source>::type filter = util::AcceptAll<Source>()) const
    {
        auto f = [this] (size_t i) { return getSource(i); };
        return getEntities<Source>(f,
                                   sourceCount(),
                                   filter);
    }


    /**
     * @brief Get all descendant sources of the source recursively.
     *
     * This method traverses the sub-tree of all child sources of the source. The traversal
     * is accomplished via breadth first and can be limited in depth. On each node or
     * source a filter is applied. If the filter returns true the respective source
     * will be added to the result list.
     * By default a filter is used that accepts every source.
     *
     * @param filter       A filter function.
     * @param max_depth    The maximum depth of traversal.
     *
     * @return A vector containing the filtered descendant sources.
     */
    std::vector<Source> findSources(util::Filter<Source>::type filter = util::AcceptAll<Source>(),
                                    size_t max_depth = std::numeric_limits<size_t>::max()) const;


    Source createSource(const std::string &name, const std::string &type) {
        return backend()->createSource(name, type);
    }

    bool deleteSource(const std::string &id) {
        return backend()->deleteSource(id);
    }

    /**
     * @brief Delete a root source and all its child sources from
     *        the source.
     *
     * @param source        The Source to delete.
     *
     * @return True if the source was deleted, false otherwise.
     */
    bool deleteSource(const Source &source) {
        if (source == none) {
            throw std::runtime_error("Source::deleteSource: empty Source entity given!");
        }
        return backend()->deleteSource(source.id());
    }

    //------------------------------------------------------
    // Operators and other functions
    //------------------------------------------------------

    /**
     * @brief Assignment operator to none.
     */
    virtual Source &operator=(none_t) {
        nullify();
        return *this;
    }

    /**
     * @brief Output operator
     */
    friend std::ostream& operator<<(std::ostream &out, const Source &ent);


};


} // namespace nix

#endif // NIX_SOURCE_H
