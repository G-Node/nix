// Copyright © 2020 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Jan Grewe <jan.grewe@g-node.org>

#include "TestDataType.hpp"

using namespace nix;

void TestDataType::setUp() {
    types = {nix::DataType::Bool, nix::DataType::Char,
             nix::DataType::Float, nix::DataType::Double,
             nix::DataType::Int8, nix::DataType::Int16,
             nix::DataType::Int32, nix::DataType::Int64,
             nix::DataType::UInt8, nix::DataType::UInt16,
             nix::DataType::UInt32, nix::DataType::UInt64,
             nix::DataType::String, nix::DataType::Nothing,
             nix::DataType::Opaque};
    names = {"Bool", "Char", "Float", "Double",
             "Int8", "Int16", "Int32", "Int64",
             "UInt8", "UInt16", "UInt32", "UInt64",
             "String", "Nothing", "Opaque"};
    namesUp = {"BooL", "CHar", "fLoat", "Double",
               "Int8", "InT16", "Int32", "inT64",
               "UINT8", "UINt16", "UInt32", "uINt64",
               "StrinG", "NOTHING",  "oPAQUE"};
    namesLow = {"bool", "char", "float", "double",
                "int8", "int16", "int32", "int64",
                "uint8", "uint16", "uint32", "uint64",
                "string", "nothing",  "opaque"};
    fixed_sizes = decltype(fixed_sizes) {{nix::DataType::Int8, 1},
                                         {nix::DataType::UInt8, 1},
                                         {nix::DataType::Int16, 2},
                                         {nix::DataType::UInt16, 2},
                                         {nix::DataType::Int32, 4},
                                         {nix::DataType::UInt32, 4},
                                         {nix::DataType::Float, 4},
                                         {nix::DataType::Int64, 8},
                                         {nix::DataType::UInt64, 8},
                                         {nix::DataType::Double, 8}};
}


void TestDataType::tearDown() {}



void TestDataType::testToString() {
    for (size_t i = 0; i < types.size(); ++i){
        CPPUNIT_ASSERT_EQUAL(data_type_to_string(types[i]), names[i]);
    }
}

void TestDataType::testFromString() {
    for (size_t i = 0; i < namesUp.size(); ++i){
        CPPUNIT_ASSERT_EQUAL(string_to_data_type(namesUp[i]), types[i]);
        CPPUNIT_ASSERT_EQUAL(string_to_data_type(namesLow[i]), types[i]);
    }
    CPPUNIT_ASSERT_THROW(string_to_data_type("unknown"), std::invalid_argument);
}

void TestDataType::testSize() {
    for (auto t : types) {
        if (fixed_sizes.find(t) != fixed_sizes.end()){
            CPPUNIT_ASSERT_EQUAL(data_type_to_size(t), static_cast<size_t>(fixed_sizes[t]));
        }
    }
}
