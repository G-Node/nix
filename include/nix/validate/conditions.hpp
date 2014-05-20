// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#ifndef NIX_CONDITIONS_H
#define NIX_CONDITIONS_H

#include <string>
#include <functional>
#include <nix/validate/result.hpp>
#include <nix/validate/checks.hpp>

namespace nix {
namespace validate {

    /**
     * Struct to yield the meta-type / decltype of a condition
     */
    template<typename TFUNC, typename T>
    struct condition {
        typedef std::function<
            std::function<Result(void)>
            (const TFUNC&, 
            typename Check<T>::type, 
            const string&)> 
            type;
    };
    
    /**
     * Actual condition type, return type of conditions functionals
     */     
    typedef std::function<Result(void)> conditionType;

    /**
     * Creates a condition check that produces an error with the given message if
     * the fields value does not pass the test.
     *
     * @param lambda Getter function returning TVAL
     * @param test      The test itself (e.g. notNothing or notEmpty)
     * @param msg       The message to produce if the test fails.
     *
     * @returns {Function} The created condition check.
     */
    template<typename TFUNC, typename T>
    conditionType
    must(TFUNC const &get, typename notFalse<T>::type const check, const string &msg);

    /**
     * Creates a condition check that produces a warning with the given message if
     * the fields value does not pass the test.
     *
     * @param field     The field name to test.
     * @param test      The test itself (e.g. notNothing or notEmpty)
     * @param msg       The message to produce if the test fails.
     *
     * @returns {Function} The created condition check.
     */
    template<typename TFUNC, typename T>
    conditionType
    should(TFUNC const &get, typename notFalse<T>::type const check, const string &msg);
    
} // namespace validate
} // namespace nix

#endif // NIX_CONDITIONS_H
