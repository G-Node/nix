// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_HELPER_H
#define NIX_HELPER_H

#include <string>

namespace nix {
namespace valid {

    /**
     * Helper class that checks whether a given class or struct has
     * a method called "id"
     * USAGE: hasID<TOBJ>::value
     */
    template <typename T>
    class hasID
    {
        typedef char one;
        typedef long two;

        template <typename C> static one test( decltype(&C::id) ) ;
        template <typename C> static two test(...);

    public:
        enum { value = sizeof(test<T>(0)) == sizeof(char) };
    };

    /**
     * Helper class with "get" method that will execute a given
     * objects "id" method & return result if "HAS_ID" template param
     * is true and return string "unknown" otherwise.
     * NOTE: id can be of any type, use "auto" type to store it.
     * USAGE: ID< hasID<TOBJ>::value >().get(obj)
     */
    template <bool HAS_ID>
    class ID {
    public:
        template <typename TOBJ>
        auto get(TOBJ parent) -> decltype(parent.id()) {
            return parent.id();
        }
    };
    template <>
    class ID<false> {
    public:
        template <typename TOBJ>
        std::string get(TOBJ parent) {
            return std::string("unknown");
        }
    };

} // namespace valid
} // namespace nix

#endif // NIX_HELPER_H
