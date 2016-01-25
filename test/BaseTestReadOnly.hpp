// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BASETESTREADONLY_HPP
#define NIX_BASETESTREADONLY_HPP

#include <nix.hpp>

#include <cppunit/TestFixture.h>


class BaseTestReadOnly : public CPPUNIT_NS::TestFixture {

private:
    std::string section_id, block_id, tag_id, mtag_id, property_id, 
                feature_id, data_array_id;
    nix::ndsize_t dim_index, dim_sampled_index, dim_range_index, dim_set_index;
    std::stringstream s;
    time_t startup_time;

public:
    void init(nix::File &file);
    void testRead();

};

#endif // NIX_BASETESTREADONLY_HPP
