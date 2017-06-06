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
#include <nix/ObjectType.hpp>

#include <nix/Platform.hpp>

namespace nix {


/**
 * @brief {@link Feature} entities are used to attach further data to a {@link nix::Tag} or
 * {@link nix::MultiTag}
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
 * This value is only valid for multi tags where it indicates that
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

    /**
     * @brief Setter for the link type.
     *
     * @param type    The link type to set.
     */
    void linkType(LinkType type) {
        backend()->linkType(type);
    }

    /**
     * @brief Getter for the link type.
     *
     * @return The current link type of the feature.
     */
    LinkType linkType() const {
        return backend()->linkType();
    }

    /**
     * @brief Sets the data array associated with this feature.
     *
     * @param name_or_id        Name or id of the data array to set.
     */
    void data(const std::string &name_or_id);

    /**
     * @brief Sets the data array associated with this feature.
     *
     * @param data    The data array to set.
     */
    void data(const DataArray &data);

    /**
     * @brief Gets the data array associated with this feature.
     *
     * @return The associated data array.
     */
    DataArray data() const {
        return backend()->data();
    }

    /**
     * @brief Destructor.
     */
    virtual ~Feature() {}

    //--------------------------------------------------
    // Other methods and functions
    //--------------------------------------------------

    /**
     * @brief Assignment operator for none.
     */
    Feature &operator=(const none_t &t) {
        ImplContainer::operator=(t);
        return *this;
    }

};

/**
 * @brief Convert a link type into string representation.
 *
 * @param ltype         The link type.
 *
 * @return A human readable name for the given type.
 */
NIXAPI std::string link_type_to_string(LinkType ltype);

/**
 * @brief Output operator for link type.
 *
 * Prints a human readable string representation of the
 * link type to an output stream.
 *
 * @param out           The output stream.
 * @param ltype         The link type to print.
 *
 * @return The output stream.
 */
NIXAPI std::ostream& operator<<(std::ostream &out, const LinkType ltype);


template<>
struct objectToType<nix::Feature> {
    static const bool isValid = true;
    static const ObjectType value = ObjectType::Feature;
    typedef nix::base::IFeature backendType;
};

} // namespace nix

#endif // NIX_FEATURE_H
