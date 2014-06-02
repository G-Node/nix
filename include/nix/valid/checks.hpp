// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#ifndef NIX_CHECKS_H
#define NIX_CHECKS_H

#include <string>
#include <vector>

using namespace std;

namespace nix {
namespace valid {

    /**
     * One Check struct that checks whether the given value casts to true
     * using {@link toBool} or to false. 
     * T can be: boost::optional, boost::none, nix-entity, vector, string
     * and any basic type.
     */
    struct notFalse {
        // should cover: none, nix-entity, basic types
        // NOTE: enum will convert via int, which means 0 = false !
        template<typename T>
        bool operator()(const T &val) {
            return (bool)val;
        }
        
        template<typename T>
        bool operator()(const vector<T> &val) {
            return !(val.empty());
        }
        
        bool operator()(const string &val) {
            return !(val.empty());
        }
        
        template<typename T>
        const bool operator()(const T &val) const {
            return const_cast<notFalse *>(this)->operator()(val);
        }
    };
    
    /**
     * One Check struct that checks whether the given value casts to false
     * using {@link toBool} or to true. 
     * T can be: boost::optional, boost::none, nix-entity, vector, string
     * and any basic type.
     */
    struct isFalse {
        template<typename T>
        bool operator()(const T &val) {
            return !(bool)(val);
        }
          
        template<typename T>
        bool operator()(const vector<T> &val) {
            return val.empty();
        }
        
        bool operator()(const string &val) {
            return val.empty();
        }
        
        template<typename T>
        const bool operator()(const T &val) const {
            return const_cast<isFalse *>(this)->operator()(val);
        }
    };
    
    /**
     * One Check struct that checks whether the given value is not empty 
     * or is empty.
     * T can be: any STL container.
     */
    struct notEmpty {          
        template<typename T>
        bool operator()(const T &val) {
            return !(val.empty());
        }
        
        template<typename T>
        const bool operator()(const T &val) const {
            return const_cast<notEmpty *>(this)->operator()(val);
        }
    };
    
    /**
     * One Check struct that checks whether the given value is empty or
     * not.
     * T can be: any STL container.
     */
    struct isEmpty {
        template<typename T>
        bool operator()(const T &val) {
            return val.empty();
        }
        
        template<typename T>
        const bool operator()(const T &val) const {
            return const_cast<isEmpty *>(this)->operator()(val);
        }
    };
    
} // namespace valid
} // namespace nix

#endif // NIX_CHECKS_H
