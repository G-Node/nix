#include "TestDataTag.hpp"

void TestDataTag::setUp() {
  f1 = new File("test_block.h5", FileMode::ReadWrite);
}

void TestDataTag::tearDown() {
  delete f1;
}

void TestDataTag::testCreateRemove() {
  Block b = f1->createBlock("DataTagTest","Test");
  std::vector<std::string> ids;
  size_t count = b.dataTagCount();

  const char *names[5] = { "tag_a", "tag_b", "tag_c", "tag_d",
      "tag_e" };
  for (int i = 0; i < 5; i++) {
    std::string type = "Event";
    DataTag dt1 = b.createDataTag(names[i], type);
    DataTag dt2 = b.getDataTag(dt1.id());
    ids.push_back(dt1.id());
    stringstream errmsg;
    errmsg << "Error while accessing dataTag: dt1.id() = " << dt1.id()
        << " / dt2.id() = " << dt2.id();
    CPPUNIT_ASSERT_MESSAGE(errmsg.str(), dt1.id().compare(dt2.id()) == 0);
  }
  stringstream errmsg2;
  errmsg2 << "Error creating DataTags. Counts do not match!";
  CPPUNIT_ASSERT_MESSAGE(errmsg2.str(), b.dataTagCount() == (count+5));

  for (size_t i = 0; i < ids.size(); i++) {
    b.removeDataTag(ids[i]);
  }

  stringstream errmsg1;
  errmsg1 << "Error while removing dataTags!";
  CPPUNIT_ASSERT_MESSAGE(errmsg1.str(), b.dataTagCount() == count);
  f1->removeBlock(b.id());
}

void TestDataTag::testReferences(){
  Block b = f1->createBlock("DataTagTest","Test");
  DataArray da_1 = b.createDataArray("TestReference 1","Reference");
  DataArray da_2 = b.createDataArray("TestReference 2","Reference");
  DataTag dt = b.createDataTag("TestDataTag1","Tag");

  stringstream counterrmsg;
  counterrmsg << "TestDataTag::testReference: Counts do not match!";
  CPPUNIT_ASSERT_MESSAGE(counterrmsg.str(), dt.referenceCount() == 0);

  dt.addReference(da_1);
  dt.addReference(da_2);
  CPPUNIT_ASSERT_MESSAGE(counterrmsg.str(), dt.referenceCount() == 2);

  DataArray ref1 = dt.getReference(da_1.id());
  stringstream retrieveerrmsg;
  retrieveerrmsg << "TestDataTag::testReference: Retrieval did not work!";
  CPPUNIT_ASSERT_MESSAGE(retrieveerrmsg.str(), ref1.id() == da_1.id());

  std::vector<DataArray> arrays = dt.references();
  CPPUNIT_ASSERT_MESSAGE(retrieveerrmsg.str(), arrays.size() == 2);

  stringstream hasReferrmsg;
  hasReferrmsg << "TestDataTag::testReference: hadReference did not work!";
  CPPUNIT_ASSERT_MESSAGE(hasReferrmsg.str(), dt.hasReference(da_1));
  CPPUNIT_ASSERT_MESSAGE(hasReferrmsg.str(), dt.hasReference(da_1.id()));
  CPPUNIT_ASSERT_MESSAGE(hasReferrmsg.str(), dt.hasReference(da_2));

  stringstream delReferrmsg;
  delReferrmsg << "TestDataTag::testReference: removeReference did not work!";
  dt.removeReference(da_1);
  CPPUNIT_ASSERT_MESSAGE(delReferrmsg.str(), dt.referenceCount() == 1);
  dt.removeReference(da_2);
  CPPUNIT_ASSERT_MESSAGE(delReferrmsg.str(), dt.referenceCount() == 0);


}

void TestDataTag::testPositionExtents(){
  Block b = f1->createBlock("DataTagTest","Test");
  DataArray da_1 = b.createDataArray("TestReference 1","Reference");

  DataArray da_2 = b.createDataArray("TestReference 2","Reference");
  DataTag dt = b.createDataTag("TestDataTag1","Tag");

  f1->removeBlock(b.id());
}
