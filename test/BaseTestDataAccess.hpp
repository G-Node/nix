// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_BASETESTDATAACCESS_H
#define NIX_BASETESTDATAACCESS_H

#include <nix.hpp>


class BaseTestDataAccess : public CPPUNIT_NS::TestFixture {

protected:
    nix::File file;
    nix::DataArray data_array, alias_array;
    nix::Tag position_tag, segment_tag, times_tag;
    nix::MultiTag multi_tag, mtag2, pointmtag;
    nix::Block block;
    nix::SampledDimension sampledDim;
    nix::RangeDimension rangeDim, aliasDim;
    nix::SetDimension setDim;

public:
    void testPositionToIndexSetDimension();
    void testPositionToIndexSampledDimension();
    void testPositionToIndexRangeDimension();
    void testOffsetAndCount();
    void testPositionInData();
    void testRetrieveData();
    void testTagFeatureData();
    void testMultiTagFeatureData();
    void testMultiTagUnitSupport();
    void testDataView();
    void testDataSlice();
    void testFlexibleTagging();
};

#endif // NIX_BASETESTDATAACCESS_H
