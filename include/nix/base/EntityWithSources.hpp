// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_ENTITY_WITH_SOURCES_H
#define NIX_ENTITY_WITH_SOURCES_H


#include <nix/base/EntityWithMetadata.hpp>
#include <nix/base/IEntityWithSources.hpp>
#include <nix/Source.hpp>

namespace nix {
namespace base {

// TODO what about TNode?
template <typename T>
class NIXAPI EntityWithSources : virtual public base::IEntityWithSources, public base::EntityWithMetadata<T> {

public:

    EntityWithSources()
        : EntityWithMetadata<T>()
    {
    }


    EntityWithSources(const std::shared_ptr<T> &p_impl)
        : EntityWithMetadata<T>(p_impl)
    {
    }

    EntityWithSources(std::shared_ptr<T> &&ptr)
        : EntityWithMetadata<T>(std::move(ptr))
    {
    }

    /**
     * Returns the number of sources that are direct descendants of this source.
     *
     * @return The number of direct child sources.
     */
    size_t sourceCount() const {
        return EntityWithMetadata<T>::backend()->sourceCount();
    }

    /**
     * Checks if this source has a specific source as direct descendant.
     *
     * @param id        The id of the source.
     *
     * @return True if a source with the given id is a direct descendant, false
     *         otherwise.
     */
    bool hasSource(const std::string &id) const {
        return EntityWithMetadata<T>::backend()->hasSource(id);
    }

    /**
     * Checks if this source has a specific source as direct descendant.
     *
     * @param Source        The source.
     *
     * @return True if a source with the given id is a direct descendant, false
     *         otherwise.
     */
    bool hasSource(const Source &source) const {
        return EntityWithMetadata<T>::backend()->hasSource(source.id());
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
        return EntityWithMetadata<T>::backend()->getSource(id);
    }

    /**
     * Retrieves a specific source by index.
     *
     * @param index     The index of the source.
     *
     * @return The source at the specified index.
     */
    Source getSource(const size_t index) const {
        return EntityWithMetadata<T>::backend()->getSource(index);
    }
    
    /**
     * Get sources associated with this entity.
     *
     * The parameter "filter" is defaulted to giving back all entities.
     * To use your own filter pass a lambda that accepts an "EntityWithSources"
     * as parameter and returns a bool telling whether to get it or not.
     *
     * @param object filter function of type {@link nix::util::Filter::type}
     * @return object entities as a vector
     */    
    std::vector<Source> sources(util::AcceptAll<Source>::type filter
                                      = util::AcceptAll<Source>()) const
    {
        auto f = [this] (size_t i) { return getSource(i); };
        // NOTE: we need to use special notation here since compiler otherwise
        // reads: ((this->getEntities) < Source) > f
        /*return this->template getEntities<Source>(f,
                                   sourceCount(),
                                   filter);*/
        return nix::base::ImplContainer<T>::template getEntities<nix::Source, decltype(f)>(f,sourceCount(),filter);
    }

    /**
     * Set all sources associated with this entity. All previously
     * associated sources, that are not in the vector will be removed.
     *
     * @param sources     A vector with all sources.
     */
    virtual void sources(const std::vector<Source> &sources) {
        EntityWithMetadata<T>::backend()->sources(sources);
    }


    /**
     * Add a Source to this entity.
     *
     * @param std::string     The id of the source.
     *
     */
    void addSource(const std::string &id) {
        EntityWithMetadata<T>::backend()->addSource(id);
    }


    /**
     * Add a Source to this entity.
     *
     * @param Source     The source.
     *
     */
    void addSource(const Source &source) {
        EntityWithMetadata<T>::backend()->addSource(source.id());
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
        return EntityWithMetadata<T>::backend()->removeSource(id);
    }

    /**
     * Remove a root source and all its child sources from
     * the source.
     *
     * @param Source      The source to remove.
     *
     * @return True if the source was removed, false otherwise.
     */
    bool removeSource(const Source &source) {
        return EntityWithMetadata<T>::backend()->removeSource(source.id());
    }

    virtual ~EntityWithSources() {}

};

} // namespace base
} // namespace nix


#endif // NIX_ENTITY_WITH_SOURCES_H
