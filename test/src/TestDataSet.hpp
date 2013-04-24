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

  void testChunkGuessing() {

    PSize dims(2);
    dims[0] = 1024;
    dims[1] = 1024;

    PSize chunks = DataSet::guessChunking(dims, DataType::Double);
    CPPUNIT_ASSERT_EQUAL(chunks[0], 64ULL);
    CPPUNIT_ASSERT_EQUAL(chunks[1], 64ULL);
  }

  void testBasic() {
    PSize dims(2);
    dims[0] = 4;
    dims[1] = 6;

    PSize chunks = DataSet::guessChunking(dims, DataType::Double);
    PSize maxdims(dims.size());
    maxdims.fill(H5S_UNLIMITED);

    DataSet ds = DataSet::create(h5group, "dsDouble", DataType::Double, dims, &maxdims, &chunks);

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

    array_type C(boost::extents[8][12]);
    values = 0;
    for(index i = 0; i != 8; ++i)
      for(index j = 0; j != 12; ++j)
        C[i][j] = values++;

    dims[0] = 8;
    dims[1] = 12;

    ds.extend(dims);
    ds.write(C);
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
  CPPUNIT_TEST(testBasic);
  CPPUNIT_TEST(testChunkGuessing);
  CPPUNIT_TEST_SUITE_END ();
};

unsigned int & TestDataSet::open_mode()
{
  static unsigned int openMode = H5F_ACC_TRUNC;
  return openMode;
}

