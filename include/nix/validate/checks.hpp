// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#ifndef NIX_CHECKS_H
#define NIX_CHECKS_H

#include <nix/validate/toBool.hpp>

namespace nix {
namespace validate {
    
    /**
     * Base struct to be inherited by all test implementations.
     * Child classes will have to implement ()-operator and will
     * all inherit typedef "type" which corresponds to the type of "()". 
     */
    template<typename T>
    struct Check : public std::unary_function<T, bool> {

        virtual bool operator()(const T&) = 0;

        typedef std::function<bool(const T&)> type;

    };
    
    /**
     * One Check struct that checks whether the given value casts to true
     * using {@link TtoBool} or to false. "T" has got to be a type handled
     * by {@link TtoBool}.
     * Use "notFalse<T>()" to pass it on as check and "::type" to define 
     * its' type.
     */
    template<typename T>
    struct notFalse : public Check<T> {

        virtual bool operator()(const T &val) {
            return TtoBool(val);
        }

    };
    
    /**
     * One Check struct that checks whether the given value casts to false
     * using {@link TtoBool} or to true. "T" has got to be a type handled
     * by {@link TtoBool}.
     * Use "notFalse<T>()" to pass it on as check and "::type" to define 
     * its' type.
     */
    template<typename T>
    struct isFalse : public Check<T> {

        virtual bool operator()(const T &val) {
            return ! TtoBool(val);
        }

    };
    
} // namespace validate
} // namespace nix

#endif // NIX_CHECKS_H
