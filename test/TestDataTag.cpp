// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Jan Grewe <jan.grewe@g-node.org>

#include "TestDataTag.hpp"
#include <sstream>
using namespace nix;
using namespace nix::hdf5;

//TODO What about the polynomials in DataArray??

void TestDataTag::setUp() {
	file = nix::File::open("test_DataArray.h5");
}


void TestDataTag::tearDown(){

}

void TestDataTag::testCreateRemove() {
	Block b = file.createBlock("DataTagTest","Test");
	std::vector<std::string> ids;
	size_t count = b.dataTagCount();
	const char *names[5] = { "tag_a", "tag_b", "tag_c", "tag_d",
			"tag_e" };
	for (int i = 0; i < 5; i++) {
		std::string type = "Event";
		DataTag dt1 = b.createDataTag(names[i], type);
		DataTag dt2 = b.getDataTag(dt1.id());
		ids.push_back(dt1.id());

		std::stringstream errmsg;
		errmsg << "Error while accessing dataTag: dt1.id() = " << dt1.id()
            		   << " / dt2.id() = " << dt2.id();
		CPPUNIT_ASSERT_MESSAGE(errmsg.str(), dt1.id().compare(dt2.id()) == 0);
	}
	std::stringstream errmsg2;
	errmsg2 << "Error creating DataTags. Counts do not match!";
	CPPUNIT_ASSERT_MESSAGE(errmsg2.str(), b.dataTagCount() == (count+5));

	for (size_t i = 0; i < ids.size(); i++) {
		b.removeDataTag(ids[i]);
	}

	std::stringstream errmsg1;
	errmsg1 << "Error while removing dataTags!";
	CPPUNIT_ASSERT_MESSAGE(errmsg1.str(), b.dataTagCount() == count);
	file.removeBlock(b.id());
}


void TestDataTag::testReferences(){
	Block b = file.createBlock("DataTagTest","Test");
	DataArray da_1 = b.createDataArray("TestReference 1","Reference");
	DataArray da_2 = b.createDataArray("TestReference 2","Reference");
	DataTag dt = b.createDataTag("TestDataTag1","Tag");

	std::stringstream counterrmsg;
	counterrmsg << "TestDataTag::testReference: Counts do not match!";
	CPPUNIT_ASSERT_MESSAGE(counterrmsg.str(), dt.referenceCount() == 0);

	dt.addReference(da_1);
	dt.addReference(da_2);
	CPPUNIT_ASSERT_MESSAGE(counterrmsg.str(), dt.referenceCount() == 2);

	DataArray ref1 = dt.getReference(da_1.id());
	std::stringstream retrieveerrmsg;
	retrieveerrmsg << "TestDataTag::testReference: Retrieval did not work!";
	CPPUNIT_ASSERT_MESSAGE(retrieveerrmsg.str(), ref1.id() == da_1.id());

	std::vector<DataArray> arrays = dt.references();
	CPPUNIT_ASSERT_MESSAGE(retrieveerrmsg.str(), arrays.size() == 2);

	std::stringstream hasReferrmsg;
	hasReferrmsg << "TestDataTag::testReference: hadReference did not work!";
	CPPUNIT_ASSERT_MESSAGE(hasReferrmsg.str(), dt.hasReference(da_1));
	CPPUNIT_ASSERT_MESSAGE(hasReferrmsg.str(), dt.hasReference(da_1.id()));
	CPPUNIT_ASSERT_MESSAGE(hasReferrmsg.str(), dt.hasReference(da_2));

	std::stringstream delReferrmsg;
	delReferrmsg << "TestDataTag::testReference: removeReference did not work!";
	dt.removeReference(da_1);
	CPPUNIT_ASSERT_MESSAGE(delReferrmsg.str(), dt.referenceCount() == 1);
	dt.removeReference(da_2);
	CPPUNIT_ASSERT_MESSAGE(delReferrmsg.str(), dt.referenceCount() == 0);
}


void TestDataTag::testExtents(){
	Block b = file.createBlock("DataTagTest","Test");
	DataArray da = b.createDataArray("TestExtents","Extents");
	DataTag dt = b.createDataTag("TestDataTag1","Tag");

	typedef boost::multi_array<double, 1> array_type;
	typedef array_type::index index;
	array_type A(boost::extents[5]);
	for(index i = 0; i != 5; ++i){
		A[i] = 100.0*i;
	}
	da.setData(A);
	dt.extents(da);
	CPPUNIT_ASSERT_NO_THROW(dt.hasExtents());
	CPPUNIT_ASSERT(dt.extents() == da);

	CPPUNIT_ASSERT(dt.removeExtents());
	file.removeBlock(b.id());
}


void TestDataTag::testPositions(){
	Block b = file.createBlock("DataTagTest","Test");
	DataArray da = b.createDataArray("TestPosition","Position");
	DataTag dt = b.createDataTag("TestDataTag1","Tag");

	typedef boost::multi_array<double, 1> array_type;
	typedef array_type::index index;
	array_type A(boost::extents[5]);
	for(index i = 0; i != 5; ++i){
		A[i] = 100.0*i;
	}
	da.setData(A);
	dt.positions(da);
	CPPUNIT_ASSERT_NO_THROW(dt.hasPositions());
	CPPUNIT_ASSERT(dt.positions() == da);

	CPPUNIT_ASSERT(dt.removePositions());
	CPPUNIT_ASSERT(dt.hasPositions() == false);
	file.removeBlock(b.id());
}


void TestDataTag::testPositionExtents(){
	Block b = file.createBlock("DataTagTest","Test");
	DataArray da_1 = b.createDataArray("TestPosition","Position");
	typedef boost::multi_array<double, 1> array_type;
	typedef array_type::index index;
	array_type A(boost::extents[5]);
	for(index i = 0; i != 5; ++i){
		A[i] = 100.0*i;
	}
	da_1.setData(A);

	array_type B(boost::extents[10]);
	for(index i = 0; i != 10; ++i){
		B[i] = 100.0*i;
	}
	DataArray da_2 = b.createDataArray("TestExtent","Extent");
	da_2.setData(B);

	DataTag dt = b.createDataTag("TestDataTag1","Tag");
	dt.positions(da_1);
	CPPUNIT_ASSERT_THROW(dt.extents("some_unknown_data_array_id"),std::runtime_error);
	CPPUNIT_ASSERT_THROW(dt.extents(da_2),std::runtime_error);

	file.removeBlock(b.id());
}

