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


/**
 * @brief Base class for entities that can be associated with one or more sources.
 *
 * In order to describe the provenance of data some entities of the NIX data model can
 * be associated with {@link nix::Source} entities. This class serves as a base class for
 * those.
 */
template <typename T>
class NIXAPI EntityWithSources : virtual public base::IEntityWithSources, public base::EntityWithMetadata<T> {

public:

    /**
     * @brief Constructor that creates a null entity.
     */
    EntityWithSources()
        : EntityWithMetadata<T>()
    {
    }

    /**
     * @brief Constructor that creates a new entity from a shared pointer to
     * an implementation instance.
     */
    EntityWithSources(const std::shared_ptr<T> &p_impl)
        : EntityWithMetadata<T>(p_impl)
    {
    }

    /**
     * @brief Constructor with move semantics that creates a new entity from a
     * shared pointer to an implementation instance.
     */
    EntityWithSources(std::shared_ptr<T> &&ptr)
        : EntityWithMetadata<T>(std::move(ptr))
    {
    }

    size_t sourceCount() const {
        return EntityWithMetadata<T>::backend()->sourceCount();
    }

    bool hasSource(const std::string &id) const {
        return EntityWithMetadata<T>::backend()->hasSource(id);
    }

    /**
     * @brief Checks if a specific source is associated with this entity.
     *
     * @param source    The source to check.
     *
     * @return True if the source is associated with this entity, false otherwise.
     */
    bool hasSource(const Source &source) const {
        return EntityWithMetadata<T>::backend()->hasSource(source.id());
    }

    Source getSource(const std::string &id) const {
        return EntityWithMetadata<T>::backend()->getSource(id);
    }

    Source getSource(const size_t index) const {
        return EntityWithMetadata<T>::backend()->getSource(index);
    }

    /**
     * @brief Get all sources associated with this entity.
     *
     * The parameter filter can be used to filter sources by various
     * criteria. By default a filter that accepts all sources is used.
     *
     * @param filter    A filter function.
     *
     * @return All associated sources that match the given filter as a vector
     */
    std::vector<Source> sources(util::Filter<Source>::type filter = util::AcceptAll<Source>()) const
    {
        auto f = [this] (size_t i) { return getSource(i); };
        return nix::base::ImplContainer<T>::template getEntities<nix::Source, decltype(f)>(f,sourceCount(),filter);
    }

    virtual void sources(const std::vector<Source> &sources) {
        EntityWithMetadata<T>::backend()->sources(sources);
    }

    void addSource(const std::string &id) {
        EntityWithMetadata<T>::backend()->addSource(id);
    }


    /**
     * @brief Associate a new source with the entity.
     *
     * Calling this method will have no effect if the source is already associated to this entity.
     *
     * @param source    The source to add.
     */
    void addSource(const Source &source) {
        EntityWithMetadata<T>::backend()->addSource(source.id());
    }


    bool removeSource(const std::string &id) {
        return EntityWithMetadata<T>::backend()->removeSource(id);
    }

    /**
     * @brief Remove a source from the list of associated sources.
     *
     * This method just removes the association between the entity and the source.
     * The source itself will not be deleted from the file.
     *
     * @param source    The source to remove.
     *
     * @return True if the source was removed, false otherwise.
     */
    bool removeSource(const Source &source) {
        return EntityWithMetadata<T>::backend()->removeSource(source.id());
    }

    /**
     * Destructor
     */
    virtual ~EntityWithSources() {}

};

} // namespace base
} // namespace nix


#endif // NIX_ENTITY_WITH_SOURCES_H
