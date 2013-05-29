#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <limits>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include <pandora.hpp>
#include <pandora/NDBuffer.hpp>

using namespace pandora;

class TestNDBuffer : public CPPUNIT_NS::TestFixture {

public:

  void setUp() {
    unsigned int &openMode = open_mode();

    h5file = H5::H5File("test_ndbuffer.h5", openMode);
    if (openMode == H5F_ACC_TRUNC) {
      h5group = h5file.createGroup("charon");
    } else {
      h5group = h5file.openGroup("charon");
    }
    openMode = H5F_ACC_RDWR;
  }

  void basic() {
    PSize dims(2, 5);
    NDBuffer A(DataType::Double, dims);

    int values = 0;
    for(size_t i = 0; i != 5; ++i)
      for(size_t j = 0; j != 5; ++j)
        A.set<double>(i*5 + j, values++);

    values = 0;
    for(size_t i = 0; i != 5; ++i)
      for(size_t j = 0; j != 5; ++j)
        CPPUNIT_ASSERT_EQUAL(A.get<double>(i*5 + j), static_cast<double>(values++));

    CPPUNIT_ASSERT_EQUAL(static_cast<double>(5), A.get<double>({1, 0}));
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(18), A.get<double>({3, 3}));
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(24), A.get<double>({4, 4}));
    
    dims = {3, 4, 5};
    NDBuffer B(DataType::Double, dims);
    values = 0;
    for(size_t i = 0; i != dims[0]; ++i)
      for(size_t j = 0; j != dims[1]; ++j)
        for(size_t k = 0; k != dims[2]; ++k)
          B.set<double>({i,j,k}, values++);
    
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(23), B.get<double>({1, 0, 3}));
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(42), B.get<double>({2, 0, 2}));
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(26), B.get<double>({1, 1 ,1}));
    
  }

  void testCharon() {
    PSize dims(2, 5);
    NDBuffer A(DataType::Double, dims);
    Charon<NDBuffer> charon(A);

    int values = 0;
    for(size_t i = 0; i != 5; ++i)
      for(size_t j = 0; j != 5; ++j)
        A.set<double>(i*5 + j, values++);
    
    DataSet ds = DataSet::create(h5group, "a2x5", A);
    ds.write(A);
    
    dims = {3, 4, 5};
    NDBuffer B(DataType::Double, dims);
    values = 0;
    std::cout << std::endl;
    for(size_t i = 0; i != dims[0]; ++i)
      for(size_t j = 0; j != dims[1]; ++j)
        for(size_t k = 0; k != dims[2]; ++k)
          B.set<double>({i,j,k}, values++);

    ds = DataSet::create(h5group, "a3x4x5", B);
    ds.write(B);

  }

  void tearDown() {
    h5group.close();
    h5file.close();
  }

  private:

  static unsigned int &open_mode();

  H5::H5File h5file;
  H5::Group h5group;

  CPPUNIT_TEST_SUITE(TestNDBuffer);
  CPPUNIT_TEST(basic);
  CPPUNIT_TEST(testCharon);
  CPPUNIT_TEST_SUITE_END ();
};

unsigned int & TestNDBuffer::open_mode()
{
  static unsigned int openMode = H5F_ACC_TRUNC;
  return openMode;
}

