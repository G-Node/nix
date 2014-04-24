// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestDataAccess.hpp"

using namespace std;
using namespace nix;


void TestDataAccess::testPositionToIndex() {
    File file = File::open("test_dataAccess.h5", FileMode::Overwrite);
    Block block = file.createBlock("dimensionTest","test");
    DataArray data_array = block.createDataArray("dimensionTest", "Test");

    typedef boost::multi_array<double, 2> array_type;
    typedef array_type::index index;
    array_type data(boost::extents[2][10]);

    int value;
    for(index i = 0; i != 2; ++i) {
        value = 0;
        for(index j = 0; j != 10; ++j) {
            data[i][j] = value++;
        }
    }
    data_array.setData(data);
    SetDimension setDim;
    SampledDimension sampledDim;
    setDim = data_array.appendSetDimension();
    std::vector<std::string> labels = {"label_a", "label_b"};
    setDim.labels(labels);
    double samplingInterval = 1.0;
    sampledDim = data_array.appendSampledDimension(samplingInterval);
}

