// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_CONDITIONS_H
#define NIX_CONDITIONS_H

#include <nix/Platform.hpp>
#include <nix/util/util.hpp>
#include <nix/valid/helper.hpp>
#include <nix/valid/result.hpp>
#include <nix/valid/checks.hpp>
#include <nix/valid/validator.hpp>

#include <string>
#include <functional>

namespace nix {
namespace valid {

    /**
     * @brief creates condition throwing error if check fails
     *
     * Creates a condition check that produces an error with the given
     * message if the given function call's return value does not pass
     * the test.
     * NOTE: the list of sub conditions is being executed in the given order
     * if and only if this conditions' check passes. The results are merged
     * and returned together.
     * Also catches any errors occuring on execution of the given
     * function call.
     *
     * @param parent     Parent object
     * @param get        Getter method in parent object (pointer-to-member)
     * @param check      The test itself (e.g. notFalse or notEmpty)
     * @param msg        The message to produce if the test fails.
     * @param subs       Init list of sub conditions to be executed only
     *                   if this check succeeds.
     *
     * @returns The created, callable condition of type condition
     */
    template<typename TOBJ, typename TBASEOBJ, typename TRET, typename TCHECK>
    condition
    must(const TOBJ &parent, TRET(TBASEOBJ::*get)(void)const, const TCHECK &check,
         const std::string &msg, const std::vector<condition> &subs = {}) {
        return [parent, get, check, msg, subs] () -> Result {
            bool errOccured = false;
            typedef decltype((parent.*get)()) return_type;
            return_type val;
            std::string id = nix::util::numToStr(
                                ID<hasID<TOBJ>::value>().get(parent)
                             );

            // execute getter call & check for error
            try {
                val = (parent.*get)();
            } catch (std::exception &e) {
                errOccured = true;
            }

            // compare value & check for validity
            if(errOccured || !check(val)) {
                return Result(Message(id, msg), none); // failed || error
            }

            // passed
            Result result = Result();
            for(auto &sub : subs) {
                result = result.concat( sub() );
            }
            return result;
        };
    }

    /**
     * @brief creates condition throwing warning if check fails
     *
     * Creates a condition check that produces a warning with the given
     * message if the given function call's return value does not pass
     * the test.
     * NOTE: the list of sub conditions is being executed in the given order
     * if and only if this conditions' check passes. The results are merged
     * and returned together.
     * Also catches any errors occuring on execution of the given
     * function call.
     *
     * @param parent     Parent object
     * @param get        Getter method in parent object (pointer-to-member)
     * @param check      The test itself (e.g. notFalse or notEmpty)
     * @param msg        The message to produce if the test fails.
     * @param subs       Init list of sub conditions to be executed only
     *                   if this check succeeds.
     *
     * @returns The created, callable condition of type condition
     */
    template<typename TOBJ, typename TBASEOBJ, typename TRET, typename TCHECK>
    condition
    should(const TOBJ &parent, TRET(TBASEOBJ::*get)(void)const, const TCHECK &check,
           const std::string &msg, const std::vector<condition> &subs = {}) {
        return [parent, get, check, msg, subs] () -> Result {
            bool errOccured = false;
            typedef decltype((parent.*get)()) return_type;
            return_type val;
            std::string id = nix::util::numToStr(
                                ID<hasID<TOBJ>::value>().get(parent)
                             );

            // execute getter call & check for error
            try {
                val = (parent.*get)();
            } catch (std::exception &e) {
                errOccured = true;
            }

            // compare value & check for validity
            if(errOccured || !check(val)) { // failed || error
                return Result(none, Message(id, msg));
            }

            // passed
            Result result = Result();
            for(auto &sub : subs) {
                result = result.concat( sub() );
            }
            return result;
        };
    }

    /**
     * @brief creates condition not throwing any message even if check fails
     *
     * Creates a condition check that produces no message even if the
     * given function call's return value does not pass the test.
     * It is meant to be used to execute sub-conditions only if test passes
     * without this condition causing errors/warnings if it fails.
     * NOTE: the list of sub conditions is being executed in the given order
     * if and only if this conditions' check passes. The results are merged
     * and returned together.
     * Also catches any errors occuring on execution of the given
     * function calls.
     *
     * @param parent     Parent object
     * @param get        Getter method in parent object (pointer-to-member)
     * @param check      The test itself (e.g. notFalse or notEmpty)
     * @param subs       Init list of sub conditions to be executed only
     *                   if this check succeeds.
     *
     * @returns The created, callable condition of type condition
     */
    template<typename TOBJ, typename TBASEOBJ, typename TRET, typename TCHECK>
    condition
    could(const TOBJ &parent, TRET(TBASEOBJ::*get)(void)const, const TCHECK &check,
          const std::vector<condition> &subs = {}) {
        return [parent, get, check, subs] () -> Result {
            bool errOccured = false;
            typedef decltype((parent.*get)()) return_type;
            return_type val;
            std::string id = nix::util::numToStr(
                                ID<hasID<TOBJ>::value>().get(parent)
                             );

            // execute getter call & check for error
            try {
                val = (parent.*get)();
            } catch (std::exception &e) {
                errOccured = true;
            }

            // compare value & check for validity
            if(errOccured || !check(val)) { // failed || error
                return Result();
            }

            // passed
            Result result = Result();
            for(auto &sub : subs) {
                result = result.concat( sub() );
            }
            return result;
        };
    }

} // namespace valid
} // namespace nix

#endif // NIX_CONDITIONS_H
