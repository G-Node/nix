// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_SOURCE_H
#define NIX_I_SOURCE_H

#include <string>
#include <vector>

#include <nix/base/IEntityWithMetadata.hpp>

namespace nix {


class Source;


namespace base {


// TODO TNode
class NIXAPI ISource : virtual public IEntityWithMetadata {

public:

    /**
     * Checks if this source has a specific source as direct descendant.
     *
     * @param id        The id of the source.
     *
     * @return True if a source with the given id is a direct descendant, false
     *         otherwise.
     */
    virtual bool hasSource(const std::string &id) const = 0;

    /**
     * Retrieves a specific child source that is a direct descendant.
     *
     * @param id        The id of the source.
     *
     * @return The source with the given id. If it doesn't exist an exception
     *         will be thrown.
     */
    virtual Source getSource(const std::string &id) const = 0;

    /**
     * Retrieves a specific source by index.
     *
     * @param index     The index of the source.
     *
     * @return The source at the specified index.
     */
    virtual Source getSource(size_t index) const = 0;

    /**
     * Returns the number of sources that are direct descendants of this source.
     *
     * @return The number of direct child sources.
     */
    virtual size_t sourceCount() const = 0;

    /**
     * Create a new root source.
     *
     * @param name      The name of the source to create.
     * @param type      The type of the source.
     *
     * @return The created source object.
     */
    virtual Source createSource(const std::string &name, const std::string &type) = 0;

    /**
     * Delete a root source and all its child sources from
     * the source.
     *
     * @param id        The id of the source to remove.
     *
     * @return True if the source was deleted, false otherwise.
     */
    virtual bool deleteSource(const std::string &id) = 0;


    virtual ~ISource() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_SOURCE_H
