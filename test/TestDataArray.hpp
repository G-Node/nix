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


#include <pandora/Util.hpp>
#include <pandora/Group.hpp>
#include <pandora/File.hpp>
#include <pandora/Block.hpp>
#include <pandora/Source.hpp>
#include <pandora/SimpleTag.hpp>
#include <pandora/DataArray.hpp>


using namespace pandora;

class TestDataArray : public CPPUNIT_NS::TestFixture {

private:

	CPPUNIT_TEST_SUITE(TestDataArray);
	CPPUNIT_TEST(testCreateRemove);
	CPPUNIT_TEST(testCalibrationSettings);
	CPPUNIT_TEST(testSoures);
	CPPUNIT_TEST(testData);
  CPPUNIT_TEST_SUITE_END ();
	File *f1;

public:

	void setUp();
	void tearDown();
	void testCreateRemove();
	void testCalibrationSettings();
  void testSoures();
  void testData();
};

