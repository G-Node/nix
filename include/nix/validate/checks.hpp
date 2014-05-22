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
     * One Check struct that checks whether the given value casts to true
     * using {@link TtoBool} or to false. "T" has got to be a type handled
     * by {@link TtoBool}.
     */
    struct notFalse {
        template<typename T>
        bool operator()(const T &val) {
            return TtoBool(val);
        }
        
        template<typename T>
        const bool operator()(const T &val) const {
            return const_cast<notFalse *>(this)->operator()(val);
        }
    };
    
    /**
     * One Check struct that checks whether the given value casts to false
     * using {@link TtoBool} or to true. "T" has got to be a type handled
     * by {@link TtoBool}.
     */
    struct isFalse {
        template<typename T>
        bool operator()(const T &val) {
            return ! TtoBool(val);
        }
        
        template<typename T>
        const bool operator()(const T &val) const {
            return const_cast<isFalse *>(this)->operator()(val);
        }
    };
    
} // namespace validate
} // namespace nix

#endif // NIX_CHECKS_H
