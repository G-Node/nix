#include "TestSource.hpp"

void TestSource::setUp() {
  f1 = new File("test_block.h5", FileMode::ReadWrite);
}

void TestSource::tearDown() {
  delete f1;
}


void TestSource::testCreateAndRemove() {
  Block b1 = f1->createBlock("test block","test");
  Source s1 = b1.createSource("S1","test");
  Source s2 = b1.createSource("S2","test");
  
  stringstream msg;
  msg << "Creating s1 or s2 failed!" ;
  CPPUNIT_ASSERT_MESSAGE(msg.str(), b1.hasSource(s1.id()) && b1.hasSource(s2.id()) );
  
  size_t count = b1.sourceCount();
  stringstream errmsg;
  errmsg << "Source count does not match! Found " << count << " should have been 2";
  CPPUNIT_ASSERT_MESSAGE(errmsg.str(), count == 2);
  
  b1.removeSource(s1.id());
  stringstream msg2;
  msg2 << "Removing s1 failed!" ;
  CPPUNIT_ASSERT_MESSAGE(msg2.str(), !b1.hasSource(s1.id()));
  b1.removeSource(s2.id());
  f1->removeBlock(b1.id());
}

void TestSource::testIterators(){
  Block b1 = f1->createBlock("test block","test");
  Source s1 = b1.createSource("S1","test");
  s1.createSource("S3","test");
  s1.createSource("S4","test");
  Source s2 = b1.createSource("S2","test");
  s2.createSource("S5","test");
  
  size_t count = s1.childCount();
  CPPUNIT_ASSERT_EQUAL(count,(size_t)2);
  count = s2.childCount();
  CPPUNIT_ASSERT_EQUAL(count,(size_t)1);
  
  b1.removeSource(s1.id());
  b1.removeSource(s2.id());
  f1->removeBlock(b1.id());
  
}

void TestSource::testFindSources(){
  Block b1 = f1->createBlock("test block","test");
  Source s1 = b1.createSource("S1","test");
  Source s2 = b1.createSource("S2","test");
  Source s3 = s1.createSource("S3","test");
  Source s4 = s1.createSource("S4","test");
  Source s5 = s2.createSource("S5","test");
  
  //sanity check
  vector<Source> res = s1.collectIf([&](const Source &source) {
    return false;
  });
  
  CPPUNIT_ASSERT_EQUAL(static_cast<vector<Source>::size_type>(0), res.size());
  
  
  //now some actual work
  res = s1.collectIf([&](const Source &source) {
    bool found = source.id() == s4.id();
    return found;
  });
  
  CPPUNIT_ASSERT_EQUAL(static_cast<vector<Source>::size_type>(1), res.size());
  CPPUNIT_ASSERT_EQUAL(s4.id(), res[0].id());
  
  res = s1.collectIf([&](const Source &source) {
    return true;
  }, true, 1);
  
  CPPUNIT_ASSERT_EQUAL(res.size(), s1.childCount());
  vector<Source> children = s1.sources();
  
  for (size_t i = 0; i < res.size(); i++) {
    CPPUNIT_ASSERT_EQUAL(res[i].id(), children[i].id());
  }
  
  
  //    CPPUNIT_ASSERT_EQUAL(b1.hasSource("invalid_id"),false);
  //    CPPUNIT_ASSERT_EQUAL(b1.hasSource(s3.id()),true);
  //    CPPUNIT_ASSERT_EQUAL(b1.existsSource(s3.id(),"test"),true);
  //    CPPUNIT_ASSERT_EQUAL(b1.existsSource(s3.id(),"no test"),false);
  //    CPPUNIT_ASSERT_EQUAL(b1.existsSource(s3.id(),"test",1),false);
  //    CPPUNIT_ASSERT_EQUAL(b1.existsSource(s3.id(),"test",2),true);
  
  b1.removeSource(s1.id());
  b1.removeSource(s2.id());
  f1->removeBlock(b1.id());
}