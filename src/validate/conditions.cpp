// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/validate/conditions.hpp>

using namespace std;

namespace nix {
namespace validate {
    
    template<typename TFUNC, typename T>
    conditionType
    must(TFUNC const &get, typename Check<T>::type const check, const string &msg) {        
        return [get, check, msg] () -> Result {
            auto val = get();

            if (! check(val)) {
                return Result(msg, none);
            } else {
                return Result();
            }
        };
    }

    template<typename TFUNC, typename T>
    conditionType
    should(TFUNC const &get, typename Check<T>::type const check, const string &msg) {
        return [get, check, msg] () -> Result {
            auto val = get();

            if (! check(val)) {
                return Result(none, msg);
            } else {
                return Result();
            }
        };
    }

} // namespace validate
} // namespace nix
