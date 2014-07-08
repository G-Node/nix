// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_CHECKS_H
#define NIX_CHECKS_H

namespace nix {
namespace valid {

    /**
     * @brief Check if later given not greater than initally defined value.
     * 
     * One Check struct that checks whether the given value is not greater
     * than the initially given other value, both of which have to be 
     * convertible to double.
     */
    struct notGreater {
        const double value;
        
        template<typename T>
        notGreater(T value) : value(static_cast<double>(value)) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return !(static_cast<double>(val) > value);
        }
    };

    /**
     * @brief Check if later given greater than initally defined value.
     * 
     * One Check struct that checks whether the given value is greater than
     * the initially given other value, both of which have to be 
     * convertible to double.
     */
    struct isGreater {
        const double value;
        
        template<typename T>
        isGreater(T value) : value(static_cast<double>(value)) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return static_cast<double>(val) > value;
        }
    };

    /**
     * @brief Check if later given not smaller than initally defined value.
     * 
     * One Check struct that checks whether the given value is not smaller
     * than the initially given other value, both of which have to be 
     * convertible to double.
     */
    struct notSmaller {
        const double value;
        
        template<typename T>
        notSmaller(T value) : value(static_cast<double>(value)) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return !(static_cast<double>(val) < value);
        }
    };

    /**
     * @brief Check if later given smaller than initally defined value.
     * 
     * One Check struct that checks whether the given value is smaller than
     * the initially given other value, both of which have to be 
     * convertible to double.
     */
    struct isSmaller {
        const double value;
        
        template<typename T>
        isSmaller(T value) : value(static_cast<double>(value)) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return static_cast<double>(val) < value;
        }
    };

    /**
     * @brief Check for un-equality of initally defined and later given value.
     * 
     * One Check struct that checks whether the given value is not equal
     * to the initially given other value.
     */
    template<typename T>
    struct notEqual {
        const T value;
        
        notEqual(T value) : value(value) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return value != val;
        }
    };

    /**
     * @brief Check for equality of initally defined and later given value.
     * 
     * One Check struct that checks whether the given value is equal to
     * the initially given other value.
     */
    template<typename T>
    struct isEqual {
        const T value;
        
        isEqual(T value) : value(value) {}
        
        template<typename T2>
        bool operator()(const T2 &val) const {
            return value == val;
        }
    };

    /**
     * @brief Check if given value casts to boolean true
     * 
     * One Check struct that checks whether the given value casts to true
     * or to false.
     * T can be: boost::optional, boost::none, nix-entity
     * and any basic type.
     */
    struct notFalse {
        // WARNING: enum will convert via int, which means 0 = false !
        template<typename T>
        bool operator()(const T &val) const {
            return !!val;
        }
    };

    /**
     * @brief Check if given value casts to boolean false
     * 
     * One Check struct that checks whether the given value casts to false
     * or to true.
     * T can be: boost::optional, boost::none, nix-entity
     * and any basic type.
     */
    struct isFalse {
        // WARNING: enum will convert via int, which means 0 = false !
        template<typename T>
        bool operator()(const T &val) const {
            return !val;
        }
    };

    /**
     * @brief Check if given class/struct returns "empty() == false"
     * 
     * One Check struct that checks whether the given value is not empty
     * or is empty.
     * T can be: any STL container.
     */
    struct notEmpty {
        template<typename T>
        bool operator()(const T &val) const {
            return !(val.empty());
        }
    };

    /**
     * @brief Check if given class/struct returns "empty() == true"
     * 
     * One Check struct that checks whether the given value is empty or
     * not.
     * T can be: any STL container.
     */
    struct isEmpty {
        template<typename T>
        bool operator()(const T &val) const {
            return val.empty();
        }
    };

} // namespace valid
} // namespace nix

#endif // NIX_CHECKS_H
