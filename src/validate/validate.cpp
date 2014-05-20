// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/validate/conditions.hpp>
#include <nix/validate/result.hpp>

using namespace std;

namespace nix {
namespace validate {
    
void test() {
    File file = File::open("test_validate.h5", FileMode::Overwrite);
    Section section = file.createSection("foo_section", "metadata");
    Block block = file.createBlock("block_one", "dataset");
    
    auto t = block.id();
    typedef decltype(t) T;
    typedef function<T()> TFUNC;
    
    std::function<bool(T)> myCheck = notFalse<T>();
    condition<TFUNC, T>::type myConditionFunc = must<TFUNC, T>;
    conditionType myCondition = myConditionFunc([block](){ return block.id(); }, myCheck, "foo is going on!");
    vector<conditionType> myConditions {myCondition};
    
    Result myResult = validator(myConditions);    

    file.close();
}

Result validator(vector<conditionType> conditions) {
    Result result = Result();

    for (int j=0; j<conditions.size(); j++) {
        result = result.concat(
            conditions[j]()
        );
    }

    return result;
}

} // namespace validate
} // namespace nix
