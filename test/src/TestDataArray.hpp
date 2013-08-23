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

class TestDataArray : public CPPUNIT_NS::TestFixture {

private:

	CPPUNIT_TEST_SUITE(TestDataArray);
	CPPUNIT_TEST(basic);
	CPPUNIT_TEST_SUITE_END ();
	File *f1;

public:

	void setUp() {
		f1 = new File("test_block.h5", "org.g-node", FileMode::ReadWrite);

	}

	void tearDown() {
		delete f1;
	}


	void basic() {
		Block b1 = f1->createBlock("test", "dataset");
		DataArray a = b1.createDataArray("test","analog signal");
		typedef boost::multi_array<double, 1> array_type;
		typedef array_type::index index;
		array_type A(boost::extents[10]);
		double values = 0.0;
		for(index i = 0; i != 10; ++i)
			A[i] = values++;
		a.setData(A);



	}


};

