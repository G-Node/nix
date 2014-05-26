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

using namespace std;

namespace nix {
namespace valid {

class Result {
    vector<string> errors;
    vector<string> warnings;
    static const char* prefixErr;
    static const char* prefixWarn;

    /**
     * Takes a pair of vector<string> vars, the first with errors the
     * second with warnings, and sets according prefix on all strings
     * in both vectors. 
     * NOTE: does _not_ check if prefixes already set and thus will 
     * produce duplicate prefixes.
     * NOTE: since declared "const" will refuse to operate on class
     * own vectors.
     *
     * @param vector<string> vector of error strings
     * @param vector<string> vector of warning strings
     * @return void
     */
    void setPrefixes(vector<string> &_errors, vector<string> &_warnings) const;

public:
    
    /**
     * Standard ctor.
     *
     * @param vector Vector of error messages
     * @param vector Vector of waning messages
     */
    Result( const vector<string> &errors = vector<string>(), 
            const vector<string> &warnings = vector<string>());
    
    /**
     * Only warnings ctor.
     *
     * @param boost::none_t boost none
     * @param vector Vector of waning messages
     */
    Result(none_t t, const vector<string> &_warnings);
    
    /**
     * Only errors ctor.
     *
     * @param vector Vector of waning messages
     * @param boost::none_t boost none
     */
    Result(const vector<string> &_errors, none_t t);
    
    /**
     * Non-vector standard ctor.
     *
     * @param vector Vector of error messages
     * @param vector Vector of waning messages
     */
    Result( const string &errors, 
            const string &warnings);
    
    /**
     * Non-vector only warnings ctor.
     *
     * @param boost::none_t boost none
     * @param vector Vector of waning messages
     */
    Result(none_t t, const string &_warnings);
    
    /**
     * Non-vector only errors ctor.
     *
     * @param vector Vector of waning messages
     * @param boost::none_t boost none
     */
    Result(const string &_errors, none_t t);
    
    /**
     * Neither errors nor warnings ctor.
     *
     * @param vector Vector of waning messages
     * @param boost::none_t boost none
     */
    Result(none_t t, none_t u);
    
    /**
     * Returns the {@link errors} and {@link warnings} vectors
     * concatenated in one vector.
     *
     * @return vector
     */
    vector<string> all() const;

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
    friend std::ostream& operator<<(ostream &out, const Result &res) {
        auto msgs = res.all();
        for(auto it = msgs.begin(); it != msgs.end(); ++it) {
            out << *it << endl;
        }
        
        return out;
    }
};

} // namespace valid
} // namespace nix

#endif // NIX_RESULT_H
