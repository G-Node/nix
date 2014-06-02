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
#include <nix/valid/result.hpp>
#include <nix/valid/checks.hpp>

namespace nix {
namespace valid {
    
    /**
     * Actual condition type, return type of conditions functionals
     */     
    typedef function<Result(void)> condition;

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
    condition
    must(TFUNC const &get, TCHECK const &check, const string &msg) {        
        return [get, check, msg] () -> Result {
            bool errOccured = false;
            string errMsg;
            typedef typename std::result_of<decltype(get)()>::type return_type;
            return_type val;
            
            // execute getter call & check for error
            try {
                val = get();
            } catch (exception e) {
                errOccured = true;
                errMsg = e.what();
            }        

            // compare value & check for validity
            if(errOccured) {
                return Result(msg, none); // error
            }
            else if (!check(val)) { 
                return Result(msg, none); // failed
            }
            
            return Result(); // passed
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
    condition
    should(TFUNC const &get, TCHECK const check, const string &msg) {
        return [get, check, msg] () -> Result {
            bool errOccured = false;
            string errMsg;
            typedef typename std::result_of<decltype(get)()>::type return_type;
            return_type val;
            
            // execute getter call & check for error
            try {
                val = get();
            } catch (exception e) {
                errOccured = true;
                errMsg = e.what();
            }

            // compare value & check for validity
            if(errOccured) {
                return Result(none, msg); // error
            }
            else if (!check(val)) { 
                return Result(none, msg); // failed
            }
            
            return Result(); // passed
        };
    }
   
} // namespace valid
} // namespace nix

#endif // NIX_CONDITIONS_H
