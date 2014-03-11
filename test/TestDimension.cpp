// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in Section and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>

#include <nix/util/util.hpp>
#include "TestDimension.hpp"

using namespace std;
using namespace nix;


void TestDimension::setUp() {
	file = File::open("test_dimension.h5", FileMode::Overwrite);
	block = file.createBlock("dimensionTest","test");
	data_array = block.createDataArray("dimensionTest", "Test");
}


void TestDimension::tearDown() {
	file.deleteBlock(block.id());
	file.close();

}


void TestDimension::testId() {
	Dimension sd = data_array.appendDimension(nix::DimensionType::Sample);
	CPPUNIT_ASSERT(data_array.dimensionCount() == 1 && sd.id() == 1);
	data_array.deleteDimension(sd.id());
	CPPUNIT_ASSERT(data_array.dimensionCount() == 0);
	//FIXME there probably is a problem when e.g. the first of two dimensions is removed. There will be a clash in
	//ids...
}


void TestDimension::testSampledDimLabel(){
	std::string label = "aLabel";
	std::string other_label = "anotherLabel";

	Dimension d = data_array.appendDimension(nix::DimensionType::Sample);
	CPPUNIT_ASSERT(d.dimensionType() == nix::DimensionType::Sample);
	if(d.dimensionType() == nix::DimensionType::Sample){
		SampledDimension sd;
		sd = d;
		sd.label(label);
		CPPUNIT_ASSERT(sd.label() == label);
		sd.label(other_label);
		CPPUNIT_ASSERT(sd.label() == other_label);
	}
	data_array.deleteDimension(d.id());
}


void TestDimension::testSampledDimUnit() {
	Dimension d = data_array.appendDimension(nix::DimensionType::Sample);
	SampledDimension sd;
	std::string invalidUnit = "invalidunit";
	std::string validUnit = "mV^2";

	CPPUNIT_ASSERT(d.dimensionType() == nix::DimensionType::Sample);
	if(d.dimensionType() == nix::DimensionType::Sample){
		sd = d;
		CPPUNIT_ASSERT_THROW(sd.unit(invalidUnit), nix::InvalidUnitException);
		CPPUNIT_ASSERT_NO_THROW(sd.unit(validUnit));
		CPPUNIT_ASSERT(sd.unit() == validUnit);
	}
	data_array.deleteDimension(d.id());
}


void TestDimension::testSampledDimSamplingInterval() {
	double impossible_sampling_interval = -1.0;
	double invalid_sampling_interval = 0.0;
	double sampling_interval = 1.0;
	Dimension d = data_array.appendDimension(nix::DimensionType::Sample);
	SampledDimension sd;
	if(d.dimensionType() == nix::DimensionType::Sample){
		sd = d;
		CPPUNIT_ASSERT_THROW(sd.samplingInterval(impossible_sampling_interval), std::runtime_error);
		CPPUNIT_ASSERT_THROW(sd.samplingInterval(invalid_sampling_interval), std::runtime_error);
		CPPUNIT_ASSERT_NO_THROW(sd.samplingInterval(sampling_interval));
		CPPUNIT_ASSERT(sd.samplingInterval() == sampling_interval);
	}
	data_array.deleteDimension(d.id());
}


void TestDimension::testSampledDimOffset() {
	double offset = 1.0;
	Dimension d = data_array.appendDimension(nix::DimensionType::Sample);
	SampledDimension sd;
	if(d.dimensionType() == nix::DimensionType::Sample){
		sd = d;
		CPPUNIT_ASSERT_NO_THROW(sd.offset(offset));
		CPPUNIT_ASSERT(sd.offset() == offset);
	}
	data_array.deleteDimension(d.id());
}


void TestDimension::testSampledDimOperators() {
	Dimension d = data_array.appendDimension(nix::DimensionType::Sample);
	Dimension d2 = data_array.appendDimension(nix::DimensionType::Sample);
	SampledDimension sd1, sd2, sd3;
	if (d.dimensionType() == nix::DimensionType::Sample && d2.dimensionType() == nix::DimensionType::Sample){
		sd1 = d;
		sd2 = d2;
		sd3 = data_array.getDimension(d.id());
		CPPUNIT_ASSERT(sd1.id() == d.id() && sd2.id() == d2.id());
		CPPUNIT_ASSERT(sd1 != sd2);
		CPPUNIT_ASSERT(sd1 != sd3);
	}
	data_array.deleteDimension(d.id());
	data_array.deleteDimension(d2.id());
}


void TestDimension::testSetDimLabels(){
	std::vector<std::string> labels = {"label_a", "label_b","label_c","label_d","label_e"};
	std::vector<std::string> new_labels = {"new label_a", "new label_b","new label_c"};
	SetDimension sd;
	Dimension d = data_array.appendDimension(nix::DimensionType::Set);

	CPPUNIT_ASSERT(d.dimensionType() == nix::DimensionType::Set);
	if(d.dimensionType() == nix::DimensionType::Set){
		sd = d;
		CPPUNIT_ASSERT(sd);
		sd.labels(labels);
		std::vector<std::string> retrieved_labels = sd.labels();
		CPPUNIT_ASSERT(retrieved_labels.size() == labels.size());
		for (size_t i = 0; i < labels.size(); i++){
			CPPUNIT_ASSERT(labels[i] == retrieved_labels[i]);
		}

		sd.labels(new_labels);
		retrieved_labels = sd.labels();
		CPPUNIT_ASSERT(retrieved_labels.size() == new_labels.size());
		for (size_t i = 0; i < new_labels.size(); i++){
			CPPUNIT_ASSERT(new_labels[i] == retrieved_labels[i]);
		}
	}
	data_array.deleteDimension(d.id());
}


void TestDimension::testRangeDimLabel(){
	std::string label = "aLabel";
	std::string other_label = "anotherLabel";
	Dimension d = data_array.appendDimension(nix::DimensionType::Range);
	RangeDimension rd;

	CPPUNIT_ASSERT(d.dimensionType() == nix::DimensionType::Range);
	if(d.dimensionType() == nix::DimensionType::Range){
		rd = d;
		rd.label(label);
		CPPUNIT_ASSERT(rd.label() == label);
		rd.label(other_label);
		CPPUNIT_ASSERT(rd.label() == other_label);
	}
	data_array.deleteDimension(d.id());
}


void TestDimension::testRangeDimUnit(){
	Dimension d = data_array.appendDimension(nix::DimensionType::Range);
	RangeDimension rd;
	std::string invalidUnit = "invalidunit";
	std::string validUnit = "ms";

	CPPUNIT_ASSERT(d.dimensionType() == nix::DimensionType::Range);
	if(d.dimensionType() == nix::DimensionType::Range){
		rd = d;
		CPPUNIT_ASSERT_THROW(rd.unit(invalidUnit), nix::InvalidUnitException);
		CPPUNIT_ASSERT_NO_THROW(rd.unit(validUnit));
		CPPUNIT_ASSERT(rd.unit() == validUnit);
	}
	data_array.deleteDimension(d.id());
}


void TestDimension::testRangeTics(){
	Dimension d = data_array.appendDimension(nix::DimensionType::Range);
	RangeDimension rd;
	std::vector<double> tics = {1.0, 2.0, 3.4, 42.0};
	std::vector<double> new_tics = {-100.0, -10.0, 0.0, 10.0, 100.0};

	CPPUNIT_ASSERT(d.dimensionType() == nix::DimensionType::Range);
	if(d.dimensionType() == nix::DimensionType::Range){
		rd = d;
		CPPUNIT_ASSERT_NO_THROW(rd.tics(tics));
		std::vector<double> retrieved_tics = rd.tics();
		CPPUNIT_ASSERT(retrieved_tics.size() == tics.size());
		for (size_t i = 0; i < tics.size(); i++){
			CPPUNIT_ASSERT(tics[i] == retrieved_tics[i]);
		}

		rd.tics(new_tics);
		retrieved_tics = rd.tics();
		CPPUNIT_ASSERT(retrieved_tics.size() == new_tics.size());
		for (size_t i = 0; i < new_tics.size(); i++){
			CPPUNIT_ASSERT(new_tics[i] == retrieved_tics[i]);
		}
	}
	data_array.deleteDimension(d.id());
}
