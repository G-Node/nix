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

using namespace pandora;

class TestDataSet:public CPPUNIT_NS::TestFixture {

public:

  void setUp() {
    unsigned int &openMode = open_mode();

    h5file = H5::H5File("test_dataset.h5", openMode);
    if (openMode == H5F_ACC_TRUNC) {
      h5group = h5file.createGroup("charon");
    } else {
      h5group = h5file.openGroup("charon");
    }
    openMode = H5F_ACC_RDWR;
  }

  void testCreation() {
    PSize dims(2);
    dims[0] = 4;
    dims[1] = 6;
    DataSet ds = DataSet::create(h5group, "dsDouble", DataType::Double, dims);

    typedef boost::multi_array<double, 2> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[4][6]);
    int values = 0;
    for(index i = 0; i != 4; ++i)
      for(index j = 0; j != 6; ++j)
        A[i][j] = values++;

    ds.write(A);

    array_type B(boost::extents[1][1]);
    ds.read(B, true);

    for(index i = 0; i != 4; ++i)
      for(index j = 0; j != 6; ++j)
        CPPUNIT_ASSERT_EQUAL(A[i][j], B[i][j]);

  }

  void tearDown() {
    h5group.close();
    h5file.close();
  }

  private:

  static unsigned int &open_mode();

  H5::H5File h5file;
  H5::Group h5group;

  CPPUNIT_TEST_SUITE(TestDataSet);
  CPPUNIT_TEST(testCreation);
  CPPUNIT_TEST_SUITE_END ();
};

unsigned int & TestDataSet::open_mode()
{
  static unsigned int openMode = H5F_ACC_TRUNC;
  return openMode;
}

