// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#ifndef NIX_CHECKS_H
#define NIX_CHECKS_H

#include <boost/logic/tribool.hpp>
#include <nix/valid/toBool.hpp>

using namespace boost::logic;

namespace nix {
namespace valid {
    
// we rename "indeterminate" to "error"
BOOST_TRIBOOL_THIRD_STATE(error)

    /**
     * One Check struct that checks whether the given value casts to true
     * using {@link toBool} or to false. "T" has got to be a type handled
     * by {@link toBool}. Returns boost tribool.
     * Does not tolerate errors, returns "error" if error occurs.
     */
    struct notFalse {
        template<typename T>
        tribool operator()(const T &val, bool errOccured) {
            return errOccured ? error : (tribool)toBool(val);
        }
        
        template<typename T>
        const tribool operator()(const T &val, bool errOccured) const {
            return const_cast<notFalse *>(this)->operator()(val, errOccured);
        }
    };
    
    /**
     * One Check struct that checks whether the given value casts to true
     * using {@link toBool} or to false. "T" has got to be a type handled
     * by {@link toBool}. Returns boost tribool.
     * Ignores errors and proceeds with test if error occurs.
     */
    struct notFalseErr {
        template<typename T>
        tribool operator()(const T &val, bool errOccured) {
            return toBool(val);
        }
        
        template<typename T>
        const tribool operator()(const T &val, bool errOccured) const {
            return const_cast<notFalseErr *>(this)->operator()(val, errOccured);
        }
    };
    
    /**
     * One Check struct that checks whether the given value casts to false
     * using {@link toBool} or to true. "T" has got to be a type handled
     * by {@link toBool}. Returns boost tribool.
     * Does not tolerate errors, returns "error" if error occurs.
     */
    struct isFalse {
        template<typename T>
        tribool operator()(const T &val, bool errOccured) {
            return errOccured ? error : (tribool)!toBool(val);
        }
        
        template<typename T>
        const tribool operator()(const T &val, bool errOccured) const {
            return const_cast<isFalse *>(this)->operator()(val, errOccured);
        }
    };
    
    /**
     * One Check struct that checks whether the given value casts to false
     * using {@link toBool} or to true. "T" has got to be a type handled
     * by {@link toBool}. Returns boost tribool.
     * Ignores errors and proceeds with test if error occurs.
     */
    struct isFalseErr {
        template<typename T>
        tribool operator()(const T &val, bool errOccured) {
            return ! toBool(val);
        }
        
        template<typename T>
        const tribool operator()(const T &val, bool errOccured) const {
            return const_cast<isFalseErr *>(this)->operator()(val, errOccured);
        }
    };
    
} // namespace valid
} // namespace nix

#endif // NIX_CHECKS_H
