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
}
