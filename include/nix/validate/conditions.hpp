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
     * Actual condition type, return type of conditions functionals
     */     
    typedef function<Result(void)> conditionType;

    /**
     * Creates a condition check that produces an error with the given 
     * message if the given function call's return value does not pass 
     * the test.
     * Also catches and reports any errors occuring on execution of 
     * the given function call.
     *
     * @param lambda Getter function returning TVAL
     * @param test      The test itself (e.g. notNothing or notEmpty)
     * @param msg       The message to produce if the test fails.
     *
     * @returns {Function} The created condition check.
     */
    
    template<typename TFUNC, typename TCHECK>
    conditionType
    must(TFUNC const &get, TCHECK const &check, const string &msg) {        
        return [get, check, msg] () -> Result {
            // execute getter call & check for error
            try {
                auto val = get();
                // compare value & check for validity
                if (! check(val)) {
                    return Result(msg, none);
                } else {
                    return Result();
                }
            } catch (exception e) {
                return Result(e.what(), none);
            }
        };
    }

    /**
     * Creates a condition check that produces a warning with the given
     * message if the given function call's return value does not pass 
     * the test.
     * Also catches and reports any errors occuring on execution of 
     * the given function call.
     *
     * @param field     The field name to test.
     * @param test      The test itself (e.g. notNothing or notEmpty)
     * @param msg       The message to produce if the test fails.
     *
     * @returns {Function} The created condition check.
     */

    template<typename TFUNC, typename TCHECK>
    conditionType
    should(TFUNC const &get, TCHECK const check, const string &msg) {
        return [get, check, msg] () -> Result {
            // execute getter call & check for error
            try {
                auto val = get();
                // compare value & check for validity
                if (! check(val)) {
                    return Result(none, msg);
                } else {
                    return Result();
                }
            } catch (exception e) {
                return Result(e.what(), none);
            }
        };
    }
    
    
} // namespace validate
} // namespace nix

#endif // NIX_CONDITIONS_H
