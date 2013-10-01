
#include "TestDataArray.hpp"

void TestDataArray::setUp() {
  f1 = new File("test_block.h5", FileMode::ReadWrite);
}

void TestDataArray::tearDown() {
  delete f1;
}

void TestDataArray::testSoures() {
  Block b1 = f1->createBlock("sources test","test");
  DataArray a = b1.createDataArray("test", "analog signal");
  Source s1 = b1.createSource("S1","test");
  a.addSource(s1);
  
  Source s2 = b1.createSource("S2","test");
  a.addSource(s2);
  
  Source s3 = s1.createSource("S3","test");
  Source s4 = s1.createSource("S4","test");
  Source s5 = s2.createSource("S5","test");
  
  
  std::vector<Source> source = a.sources();
  
  CPPUNIT_ASSERT_EQUAL(static_cast<std::vector<Source>::size_type>(2), source.size());
  
  CPPUNIT_ASSERT(std::find_if(source.begin(), source.end(), [&](const Source &s){ return s.id() == s1.id(); }) != source.end());
  
  CPPUNIT_ASSERT(std::find_if(source.begin(), source.end(), [&](const Source &s){ return s.id() == s2.id(); }) != source.end());
}
