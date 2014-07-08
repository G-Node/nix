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

#include <nix/Platform.hpp>
#include <nix/util/util.hpp>
#include <nix/valid/helper.hpp>
#include <nix/valid/result.hpp>
#include <nix/valid/checks.hpp>

namespace nix {
namespace valid {

    /**
     * Actual condition type, return type of conditions functionals
     */
    typedef std::function<Result(void)> condition;

    /**
     * Creates a condition check that produces an error with the given
     * message if the given function call's return value does not pass
     * the test.
     * Also catches any errors occuring on execution of the given
     * function call.
     *
     * @param pointer-to-object Parent object
     * @param pointer-to-member Getter method in parent object
     * @param check      The test itself (e.g. notFalse or notEmpty)
     * @param string     The message to produce if the test fails.
     *
     * @returns {Function} The created condition check.
     */
    template<typename TOBJ, typename TBASEOBJ, typename TRET, typename TCHECK>
	NIXAPI
    condition
    must(const TOBJ &parent, TRET(TBASEOBJ::*get)(void)const, const TCHECK &check, const std::string &msg) {
        return [parent, get, check, msg] () -> Result {
            bool errOccured = false;
            typedef decltype((parent.*get)()) return_type;
            return_type val;
            std::string id = nix::util::numToStr(
                                ID<hasID<TOBJ>::value>().get(parent)
                             );

            // execute getter call & check for error
            try {
                val = (parent.*get)();
            } catch (std::exception e) {
                errOccured = true;
            }

            // compare value & check for validity
            if(errOccured || !check(val)) {
                return Result(Message(id, msg), none); // failed || error
            }

            return Result(); // passed
        };
    }

    /**
     * Creates a condition check that produces a warning with the given
     * message if the given function call's return value does not pass
     * the test.
     * Also catches any errors occuring on execution of the given
     * function call.
     *
     * @param pointer-to-object Parent object
     * @param pointer-to-member Getter method in parent object
     * @param check      The test itself (e.g. notFalse or notEmpty)
     * @param string     The message to produce if the test fails.
     *
     * @returns {Function} The created condition check.
     */
    template<typename TOBJ, typename TBASEOBJ, typename TRET, typename TCHECK>
	NIXAPI
    condition
    should(const TOBJ &parent, TRET(TBASEOBJ::*get)(void)const, const TCHECK &check, const std::string &msg) {
        return [parent, get, check, msg] () -> Result {
            bool errOccured = false;
            typedef decltype((parent.*get)()) return_type;
            return_type val;
            std::string id = nix::util::numToStr(
                                ID<hasID<TOBJ>::value>().get(parent)
                             );

            // execute getter call & check for error
            try {
                val = (parent.*get)();
            } catch (std::exception e) {
                errOccured = true;
            }

            // compare value & check for validity
            if(errOccured || !check(val)) { // failed || error
                return Result(none, Message(id, msg));
            }

            return Result(); // passed
        };
    }

} // namespace valid
} // namespace nix

#endif // NIX_CONDITIONS_H
