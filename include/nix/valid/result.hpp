// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_RESULT_H
#define NIX_RESULT_H

#include <nix/None.hpp>

namespace nix {
namespace valid {

typedef typename std::pair<std::string, std::string> strPair;

class Result {

    std::vector<strPair> errors;
    std::vector<strPair> warnings;
    static const char* prefixErr;
    static const char* prefixWarn;
    static const char* prefixID;

    /**
     * Takes a pair of vector<strPair> vars, the first with errors the
     * second with warnings, and sets according prefix on all strings
     * in both vectors.
     * NOTE: does _not_ check if prefixes already set and thus will
     * produce duplicate prefixes.
     * NOTE: since declared "const" will refuse to operate on class
     * own vectors.
     *
     * @param vector<strPair> vector of error strings
     * @param vector<strPair> vector of warning strings
     * @return void
     */
    void setPrefixes(std::vector<strPair> &errs,
                     std::vector<strPair> &warns) const;

    /**
     * Takes a pair of vector<strPair> vars, the first with errors the
     * second with warnings, and sets each msgs id as prefix in its msg
     * string in both vectors.
     * NOTE: does _not_ check if id prefixes already set and thus will
     * produce duplicate prefixes.
     * NOTE: since declared "const" will refuse to operate on class
     * own vectors.
     *
     * @param vector<strPair> vector of error messages
     * @param vector<strPair> vector of warning messages
     * @return void
     */
    void setIdPrefixes(std::vector<strPair> &errs,
                       std::vector<strPair> &warns) const;

public:

    /**
     * Standard ctor.
     *
     * @param vector Vector of error messages
     * @param vector Vector of waning messages
     */
    Result(const std::vector<strPair> &errs = std::vector<strPair>(),
           const std::vector<strPair> &warns = std::vector<strPair>());

    /**
     * Only warnings ctor.
     *
     * @param boost::none_t boost none
     * @param vector Vector of waning messages
     */
    Result(none_t t, const std::vector<strPair> &warns);

    /**
     * Only errors ctor.
     *
     * @param vector Vector of waning messages
     * @param boost::none_t boost none
     */
    Result(const std::vector<strPair> & errs, none_t t);

    /**
     * Non-vector standard ctor.
     *
     * @param vector Vector of error messages
     * @param vector Vector of waning messages
     */
    Result( const strPair &err,
            const strPair &warn);

    /**
     * Non-vector only warnings ctor.
     *
     * @param boost::none_t boost none
     * @param vector Vector of waning messages
     */
    Result(none_t t, const strPair &warn);

    /**
     * Non-vector only errors ctor.
     *
     * @param vector Vector of waning messages
     * @param boost::none_t boost none
     */
    Result(const strPair &err, none_t t);

    /**
     * Neither errors nor warnings ctor.
     *
     * @param vector Vector of waning messages
     * @param boost::none_t boost none
     */
    Result(none_t t, none_t u);

    /**
     * Returns the {@link warnings} vector.
     *
     * @return vector
     */
    std::vector<strPair> getWarnings() const;

    /**
     * Returns the {@link errors} vector.
     *
     * @return vector
     */
    std::vector<strPair> getErrors() const;

    /**
     * Concatenates the {@link errors} and {@link warnings} vectors
     * of the given {@link Result} object to those if this {@link Result}
     * object and returns a reference to this object.
     *
     * @return Result
     */
    Result concat(const Result &result);

    /**
     * Returns true if neither errors nor warnings have been added,
     * thus both vectors are empty. Returns false otherwise.
     *
     * @return bool
     */
    bool ok() const;

    /**
     * Returns false if no errors have been added, thus vector is empty.
     * Returns true otherwise.
     *
     * @return bool
     */
    bool hasErrors() const;

    /**
     * Returns false if no warnings have been added, thus vector is empty.
     * Returns true otherwise.
     *
     * @return bool
     */
    bool hasWarnings() const;

    /**
     * Output operator
     *
     * @return std::ostream&
     */
    friend std::ostream& operator<<(std::ostream &out, const Result &res) {
        // make temp copies to set prefixes on
        std::vector<strPair> tmp_errors = res.getErrors();
        std::vector<strPair> tmp_warnings = res.getWarnings();
        // set prefixes
        res.setPrefixes(tmp_errors, tmp_warnings);
        // set ID prefixes
        res.setIdPrefixes(tmp_errors, tmp_warnings);
        // output messages with prefixes
        for(auto it = tmp_warnings.begin(); it != tmp_warnings.end(); ++it) {
            out << (*it).second << std::endl;
        }
        for(auto it = tmp_errors.begin(); it != tmp_errors.end(); ++it) {
            out << (*it).second << std::endl;
        }

        return out;
    }
};

} // namespace valid
} // namespace nix

#endif // NIX_RESULT_H
