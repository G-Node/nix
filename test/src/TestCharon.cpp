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

#include <boost/math/constants/constants.hpp>
#include <boost/iterator/zip_iterator.hpp>

class TestCharon:public CPPUNIT_NS::TestFixture {

public:

  void setUp() {
    h5file = H5::H5File("test_charon.h5", H5F_ACC_TRUNC);
    h5group = h5file.createGroup("charon");
  }

  void tearDown() {
    h5group.close();
    h5file.close();
  }

  void testBaseTypes() {
    pandora::Group group(h5group);
    group.setAttr("t_int", 42);
    int tint;
    group.getAttr("t_int", tint);
    CPPUNIT_ASSERT_EQUAL(tint, 42);

    double deps = std::numeric_limits<double>::epsilon();
    double dpi = boost::math::constants::pi<double>();

    group.setAttr("t_double", dpi);
    double dbl;
    group.getAttr("t_double", dbl);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(dpi, dbl, deps);

    //string
    const std::string testStr = "I saw the best minds of my generation destroyed by madness";
    group.setAttr("t_string", testStr);

    std::string retString;
    group.getAttr("t_string", retString);
    CPPUNIT_ASSERT_EQUAL(testStr, retString);

  }

  void testArray() {
    pandora::Group group(h5group);
    //arrays
    typedef boost::multi_array<double, 3> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[3][4][2]);

    int values = 0;
    for(index i = 0; i != 3; ++i)
      for(index j = 0; j != 4; ++j)
        for(index k = 0; k != 2; ++k)
          A[i][j][k] = values++;

    group.setAttr<array_type>("t_doubleArray", A);

    array_type B(boost::extents[1][1][1]);
    group.getAttr("t_doubleArray", B);

    int verify = 0;
    int errors = 0;
    for(index i = 0; i != 3; ++i) {
      for(index j = 0; j != 4; ++j) {
        for(index k = 0; k != 2; ++k) {
          int v = verify++;
          errors += B[i][j][k] != v;
        }
      }
    }

    CPPUNIT_ASSERT_EQUAL(errors, 0);
  }

  void testVector() {
    pandora::Group group(h5group);

    std::vector<int> iv;
    iv.push_back(7);
    iv.push_back(23);
    iv.push_back(42);
    iv.push_back(1982);

    group.setAttr("t_intvector", iv);

    std::vector<int> tiv;
    group.getAttr("t_intvector", tiv);
    assert_vectors_equal(iv, tiv);

    std::vector<std::string> sv;
    sv.push_back("Alle");
    sv.push_back("meine");
    sv.push_back("Entchen");

    group.setAttr("t_strvector", sv);

    std::vector<std::string> tsv;
    group.getAttr("t_strvector", tsv);
    assert_vectors_equal(sv, tsv);

  }

  template<typename T>
  static void assert_vectors_equal(std::vector<T> &a, std::vector<T> &b) {

    typedef const T &value_ref;
    auto ibeg = boost::make_tuple(a.begin(), b.begin());
    auto iend = boost::make_tuple(a.end(), b.end());
    auto zibeg = boost::make_zip_iterator(ibeg);
    auto ziend = boost::make_zip_iterator(iend);
    typedef const boost::tuple<const T&, const T&>& tuple_ref;

    std::for_each(zibeg, ziend, [](tuple_ref t) {
      CPPUNIT_ASSERT_EQUAL(boost::get<0>(t), boost::get<1>(t));
    });
  }

private:

  H5::H5File h5file;
  H5::Group h5group;

  CPPUNIT_TEST_SUITE(TestCharon);
  CPPUNIT_TEST(testBaseTypes);
  CPPUNIT_TEST(testVector);
  CPPUNIT_TEST(testArray);
  CPPUNIT_TEST_SUITE_END ();
};



 int main(int argc, char* argv[])
 {
  CPPUNIT_TEST_SUITE_REGISTRATION(TestCharon);

  CPPUNIT_NS::TestResult testresult;
  CPPUNIT_NS::TestResultCollector collectedresults;
  testresult.addListener(&collectedresults);

  CPPUNIT_NS::BriefTestProgressListener progress;
  testresult.addListener(&progress);

  CPPUNIT_NS::TestRunner testrunner;
  testrunner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
  testrunner.run(testresult);

  CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
  compileroutputter.write();

  return !collectedresults.wasSuccessful();
}