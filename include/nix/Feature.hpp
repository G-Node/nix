// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_FEATURE_H
#define NIX_FEATURE_H

#include <nix/base/Entity.hpp>
#include <nix/base/IFeature.hpp>
#include <nix/DataArray.hpp>

#include <nix/Platform.hpp>

namespace nix {


/**
 * @brief {@link Feature} entities are used to attach further data to a {@link nix::SimpleTag} or
 * {@link nix::DataTag}
 *
 * A {@link Feature} entity contains a link to an existing {@link nix::DataArray} containing additional
 * data that belongs to the respective tag. The way how data and feature are connected is specified by the
 * link type.
 *
 * ### Tagged
 *
 * This link type  indicates, that only a certain subset of the linked {@link nix::DataArray}
 * belongs to the {@link Feature}. This subset is defined by the position and extent of the
 * respective tag.
 *
 * ### Untagged
 *
 * This implies that the whole data stored in the linked {@link nix::DataArray} belongs to
 * the {@link Feature}.
 *
 * ### Indexed
 *
 * This value is only valid for data tags where it indicates that
 * the data linked via this {@link Feature} has to be accessed according
 * to the index in the respective position entry.
 */
class NIXAPI Feature : public base::Entity<base::IFeature> {

public:

    /**
     * @brief Constructor that creates an uninitialized Feature.
     *
     * Calling any method on an uninitialized feature will throw a {@link nix::UninitializedEntity}
     * exception. The following code illustrates how to check if a feature is initialized:
     *
     * ~~~
     * Feature e = ...;
     * if (e) {
     *     // e is initialised
     * } else {
     *     // e is uninitialized
     * }
     * ~~~
     */
    Feature()
        : Entity()
    {
    }

    /**
     * @brief Copy constructor.
     *
     * Copying of all NIX front facing objects like Feature is a rather cheap operation.
     * Semantically this is equivalent to the creation of another reference to the original
     * object.
     *
     * @param other     The feature to copy.
     */
    Feature(const Feature &other)
        : Entity(other.impl())
    {
    }

    /**
     * @brief Constructor that creates a new entity from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    Feature(const std::shared_ptr<base::IFeature> &p_impl)
        : Entity(p_impl)
    {
    }

    /**
     * @brief Constructor with move semantics that creates a new entity from a shared pointer to
     * an implementation instance.
     *
     * This constructor should only be used in the back-end.
     */
    Feature(std::shared_ptr<base::IFeature> &&ptr)
        : Entity(std::move(ptr))
    {
    }

    void linkType(LinkType type) {
        backend()->linkType(type);
    }

    LinkType linkType() const {
        return backend()->linkType();
    }

    void data(const std::string &id) {
        backend()->data(id);
    }

    /**
     * @brief Sets the data array associated with this feature.
     *
     * @param data    The data array to set.
     */
    void data(const DataArray &data);

    DataArray data() const {
        return backend()->data();
    }

    /**
     * @brief Destructor.
     */
    virtual ~Feature() {}
    
};


} // namespace nix

#endif // NIX_FEATURE_H
