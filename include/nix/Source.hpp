// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_SOURCE_H
#define NIX_SOURCE_H

#include <nix/base/EntityWithMetadata.hpp>
#include <nix/base/ISource.hpp>

namespace nix {


// TODO inherit from EntityWithMetadata
// TODO what about TNode?
class Source : virtual public base::ISource, public base::EntityWithMetadata<base::ISource> {

public:

    Source()
        : EntityWithMetadata()
    {
    }


    Source(const Source &other)
        : EntityWithMetadata(other.impl_ptr)
    {
    }


    Source(const std::shared_ptr<base::ISource> &p_impl)
        : EntityWithMetadata(p_impl)
    {
    }

    //--------------------------------------------------
    // Methods concerning child sources
    //--------------------------------------------------

    /**
     * Checks if this source has a specific source as direct descendant.
     *
     * @param id        The id of the source.
     *
     * @return True if a source with the given id is a direct descendant, false
     *         otherwise.
     */
    bool hasSource(const std::string &id) const {
        return impl_ptr->hasSource(id);
    }

    /**
     * Retrieves a specific child source that is a direct descendant.
     *
     * @param id        The id of the source.
     *
     * @return The source with the given id. If it doesn't exist an exception
     *         will be thrown.
     */
    Source getSource(const std::string &id) const {
        return impl_ptr->getSource(id);
    }

    /**
     * Retrieves a specific source by index.
     *
     * @param index     The index of the source.
     *
     * @return The source at the specified index.
     */
    // TODO do really need this method?
    //      If not, check for similar methods in other classes.
    Source getSource(size_t index) const {
        return impl_ptr->getSource(index);
    }

    /**
     * Returns the number of sources that are direct descendants of this source.
     *
     * @return The number of direct child sources.
     */
    size_t sourceCount() const {
        return impl_ptr->sourceCount();
    }

    /**
     * Returns all sources that are direct descendant of this source as a vector.
     *
     * @return All direct child sources.
     */
    std::vector<Source> sources() const {
        return impl_ptr->sources();
    }

    /**
     * Create a new root source.
     *
     * @param name      The name of the source to create.
     * @param type      The type of the source.
     *
     * @return The created source object.
     */
    Source createSource(const std::string &name, const std::string &type) {
        return impl_ptr->createSource(name, type);
    }

    /**
     * Remove a root source and all its child sources from
     * the source.
     *
     * @param id        The id of the source to remove.
     *
     * @return True if the source was removed, false otherwise.
     */
    bool removeSource(const std::string &id) {
        return impl_ptr->removeSource(id);
    }


};


} // namespace nix

#endif // NIX_SOURCE_H
