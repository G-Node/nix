// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_GROUP_HPP
#define NIX_GROUP_HPP

#include <nix/base/EntityWithSources.hpp>
#include <nix/base/IGroup.hpp>
#include <nix/DataArray.hpp>
#include <nix/Platform.hpp>


namespace nix {

class NIXAPI Group : public base::EntityWithSources<base::IGroup> {

public:

/**
 * @brief Constructor that creates an uninitialized Group.
 *
 * Calling any method on an uninitialized group will throw a {@link nix::UninitializedEntity}
 * exception. The following code illustrates how to check if a group is initialized:
 *
 * ~~~
 * Group g = ...;
 * if (g) {
 *     // g is initialised
 * } else {
 *     // g is uninitialized
 * }
 * ~~~
 */
    Group()
        : EntityWithSources() { }

/**
 * @brief Copy constructor.
 *
 * Copying of all NIX front facing objects like Group is a rather cheap operation.
 * Semantically this is equivalent to the creation of another reference to the original
 * object.
 *
 * @param other     The group to copy.
 */
    Group(const Group &other)
        : EntityWithSources(other.impl()) {
    }

/**
 * @brief Constructor that creates a new group from a shared pointer to
 * an implementation instance.
 *
 * This constructor should only be used in the back-end.
 */
    Group(const std::shared_ptr<base::IGroup> &p_impl)
        : EntityWithSources(p_impl) {
    }

/**
 * @brief Constructor with move semantics that creates a new group from a shared pointer to
 * an implementation instance.
 *
 * This constructor should only be used in the back-end.
 */
    Group(std::shared_ptr<base::IGroup> &&ptr)
        : EntityWithSources(std::move(ptr)) {
    }


    bool hasDataArray(const std::string &name_or_id) const {
        return backend()->hasDataArray(name_or_id);
    }


    bool hasDataArray(const DataArray &data_array) const;

/**
 * @brief Assignment operator for none.
 */
    Group &operator=(const none_t &t) {
        ImplContainer::operator=(t);
        return *this;
    }

/**
 * @brief Output operator
 */
    NIXAPI friend std::ostream &operator<<(std::ostream &out, const Group &ent);

};

} // namespace nix

#endif //NIX_GROUP_HPP
