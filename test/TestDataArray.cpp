
#include "TestDataArray.hpp"

void TestDataArray::setUp() {
  f1 = new File("test_block.h5", FileMode::ReadWrite);
}

void TestDataArray::tearDown() {
  delete f1;
}

void TestDataArray::testCreateRemove(){
  Block b = f1->createBlock("sources test","test");
  DataArray a = b.createDataArray("test", "analog signal");

  CPPUNIT_ASSERT(b.dataArrayCount() == 1);
  CPPUNIT_ASSERT(b.hasDataArray(a.id()));
  CPPUNIT_ASSERT(b.dataArrays()[0].id().compare(a.id()) == 0);
  CPPUNIT_ASSERT(a.expansionOrigin() == 0.0);
  CPPUNIT_ASSERT(a.polynomCoefficients().size() == 2);
  CPPUNIT_ASSERT(a.polynomCoefficients()[0] == 0.0);
  b.removeDataArray(a.id());
  CPPUNIT_ASSERT(b.dataArrayCount() == 0);

  f1->removeBlock(b.id());
}

void TestDataArray::testSoures() {
  Block b = f1->createBlock("sources test","test");
  DataArray a = b.createDataArray("test", "analog signal");
  Source s1 = b.createSource("S1","test");
  a.addSource(s1);

  Source s2 = b.createSource("S2","test");
  a.addSource(s2);

  Source s3 = s1.createSource("S3","test");
  Source s4 = s1.createSource("S4","test");
  Source s5 = s2.createSource("S5","test");

  std::vector<Source> source = a.sources();

  CPPUNIT_ASSERT_EQUAL(static_cast<std::vector<Source>::size_type>(2), source.size());

  CPPUNIT_ASSERT(std::find_if(source.begin(), source.end(), [&](const Source &s){ return s.id() == s1.id(); }) != source.end());

  CPPUNIT_ASSERT(std::find_if(source.begin(), source.end(), [&](const Source &s){ return s.id() == s2.id(); }) != source.end());

  f1->removeBlock(b.id());
}

void TestDataArray::testCalibrationSettings(){
  Block b = f1->createBlock("testBlock","test");
  DataArray a = b.createDataArray("DataArray","sampledData");
  std::vector<double> coefficients;
  coefficients.push_back(2.0);
  coefficients.push_back(1.0);
  a.polynomCoefficients(coefficients);
  std::vector<double> readCs = a.polynomCoefficients();
  CPPUNIT_ASSERT(readCs.size() == 2);
  CPPUNIT_ASSERT(readCs[0] == 2.0 && readCs[1] == 1.0);

  a.expansionOrigin(10.0);
  CPPUNIT_ASSERT(a.expansionOrigin() == 10.0);
  f1->removeBlock(b.id());
}

void TestDataArray::testData(){
  Block b = f1->createBlock("testBlock","test");
  DataArray a = b.createDataArray("DataArray","sampledData");
  std::vector<double> coefficients;
  coefficients.push_back(0.0);
  coefficients.push_back(2.0);
  a.polynomCoefficients(coefficients);

  typedef boost::multi_array<double, 1> array_type;
  typedef array_type::index index;
  array_type A(boost::extents[5]);

  for(index i = 0; i != 5; ++i){
    A[i] = 100.0*i;
  }
  a.setRawData(A);

  array_type convertedData, rawData;
  a.getData(convertedData);
  a.getRawData(rawData);
  int errors = 0;
  for(size_t i = 0; i < rawData.size();i++){
    if ((rawData[i]*2.0) != convertedData[i])
      errors++;
  }
  CPPUNIT_ASSERT(errors == 0.0);
  f1->removeBlock(b.id());
}
