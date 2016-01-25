// Copyright (c) 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#include <ctime>
#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <limits>

#include <nix/util/util.hpp>
#include <nix/valid/validate.hpp>

#include "BaseTestReadOnly.hpp"

#include <cppunit/extensions/HelperMacros.h>
#include <boost/math/constants/constants.hpp>

using namespace std;
using namespace nix;
using namespace valid;


void BaseTestReadOnly::init(nix::File &file) {
    startup_time = time(NULL);
    std::vector<nix::Value> values = { nix::Value(1.0),
                                       nix::Value(2.0),
                                       nix::Value(-99.99) };
    std::vector<double> ticks = {1.0, 2.0, 3.4, 42.0};

    Section section = file.createSection("foo_section", "metadata");
    Block block = file.createBlock("block_one", "dataset");
    DataArray positions = block.createDataArray("positions_DataArray", "dataArray",
                                      DataType::Double, NDSize({ 0, 0 }));
    DataArray data_array = block.createDataArray("array_one",
                                   "testdata",
                                   nix::DataType::Double,
                                   nix::NDSize({ 0, 0, 0 }));
    Dimension dim = data_array.appendSampledDimension(3.14);
    SampledDimension dim_sampled = data_array.appendSampledDimension(3.14);
    RangeDimension dim_range = data_array.appendRangeDimension(ticks);
    SetDimension dim_set = data_array.appendSetDimension();
    Tag tag = block.createTag("featureTest", "Test", {0.0, 2.0, 3.4});
    MultiTag mtag = block.createMultiTag("tag_one", "test_tag", positions);
    Feature feature = tag.createFeature(data_array, nix::LinkType::Tagged);
    Property property = section.createProperty("doubleProperty", values);

    section_id = section.id(); feature_id = feature.id(); tag_id = tag.id();
    mtag_id = mtag.id(); property_id = property.id(); block_id = block.id();
    data_array_id = data_array.id(); dim_index = dim.index();
    dim_sampled_index = dim_sampled.index(); dim_range_index = dim_range.index();
    dim_set_index = dim_set.index();

    file.close();
}


void BaseTestReadOnly::testRead() {
    File file = File::open("test_read_only.h5", FileMode::ReadOnly);

    Section section = file.getSection(section_id);
    Block block = file.getBlock(block_id);
    DataArray data_array = block.getDataArray(data_array_id);
    Dimension dim = data_array.getDimension(dim_index);
    SampledDimension dim_sampled = data_array.getDimension(dim_sampled_index).asSampledDimension();
    RangeDimension dim_range = data_array.getDimension(dim_range_index).asRangeDimension();
    SetDimension dim_set = data_array.getDimension(dim_set_index).asSetDimension();
    Tag tag = block.getTag(tag_id);
    MultiTag mtag = block.getMultiTag(mtag_id);
    Feature feature = tag.getFeature(feature_id);
    Property property = section.getProperty(property_id);

    // TODO use assertions here
    s << block.id() << block.name();
    s << data_array.id() << data_array.name();
    s << tag.id() << tag.name();
    s << mtag.id() << mtag.name();
    s << feature.id();
    s << property.id() << property.name();
    s << dim.index();
    s << dim_sampled.index();
    s << dim_range.index();
    s << dim_set.index();
    
    file.close();
}
