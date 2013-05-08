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

  void testPSize() {
    PSize a = {23, 42, 1982};
    
    typedef typename PSize::value_type value_type;
    
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(23),   a[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(42),   a[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1982), a[2]);
    
    CPPUNIT_ASSERT_THROW(a[3], std::out_of_range);
    
    a++;
    
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(24),   a[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(43),   a[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1983), a[2]);
    
    a--;
    
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(23),   a[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(42),   a[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1982), a[2]);
    
    a += 13;

    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(36),   a[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(55),   a[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1995), a[2]);
    
    a -= 13;
    
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(23),   a[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(42),   a[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1982), a[2]);
    
    
    PSize b = {19, 1940, 18};
    
    PSize c = a + b;
    
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(42),   c[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1982), c[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(2000), c[2]);
    
    PSize d = c - b;
    
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(23),   d[0]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(42),   d[1]);
    CPPUNIT_ASSERT_EQUAL(static_cast<value_type>(1982), d[2]);
    
    PSize f = {1, 2, 3, 4};
    CPPUNIT_ASSERT_THROW(a + f, std::out_of_range);
    
  }
  
  void testChunkGuessing() {

    PSize dims = {1024, 1024};

    PSize chunks = DataSet::guessChunking(dims, DataType::Double);
    CPPUNIT_ASSERT_EQUAL(chunks[0], 64ULL);
    CPPUNIT_ASSERT_EQUAL(chunks[1], 64ULL);
  }

  void testBasic() {
    PSize dims = {4, 6};

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



  void testSelection() {
    PSize dims = {15, 15};

    PSize chunks = DataSet::guessChunking(dims, DataType::Double);
    PSize maxdims(dims.size());
    maxdims.fill(H5S_UNLIMITED);
    DataSet ds = DataSet::create(h5group, "dsDoubleSelection", DataType::Double, dims, &maxdims, &chunks);

    typedef boost::multi_array<double, 2> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[5][5]);
    int values = 1;
    for(index i = 0; i != 5; ++i)
      for(index j = 0; j != 5; ++j)
        A[i][j] = values++;

    Selection memSelection(A);
    Selection fileSelection = ds.createSelection();

    PSize fileCount(dims.size());
    PSize fileStart(dims.size());
    fileCount.fill(5ULL);
    fileStart.fill(5ULL);
    fileSelection.select(fileCount, fileStart);

    PSize boundsStart(dims.size());
    PSize boundsEnd(dims.size());

    fileSelection.bounds(boundsStart, boundsEnd);
    PSize boundsSize = fileSelection.size();

    ds.write(A, fileSelection, memSelection);

    array_type B(boost::extents[5][5]);
    ds.read(B, fileSelection); //NB: no mem-selection

    for(index i = 0; i != 5; ++i)
      for(index j = 0; j != 5; ++j)
        CPPUNIT_ASSERT_EQUAL(A[i][j], B[i][j]);

    PSSize offset(dims.size(), 5);
    fileSelection.offset(offset);
    ds.write(A, fileSelection);

    array_type C(boost::extents[5][5]);
    ds.read(C, fileSelection, true);

    for(index i = 0; i != 5; ++i)
      for(index j = 0; j != 5; ++j)
        CPPUNIT_ASSERT_EQUAL(A[i][j], C[i][j]);
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
  CPPUNIT_TEST(testPSize);
  CPPUNIT_TEST(testChunkGuessing);
  CPPUNIT_TEST(testBasic);
  CPPUNIT_TEST(testSelection);
  CPPUNIT_TEST_SUITE_END ();
};

unsigned int & TestDataSet::open_mode()
{
  static unsigned int openMode = H5F_ACC_TRUNC;
  return openMode;
}

