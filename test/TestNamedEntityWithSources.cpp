#include "TestNamedEntityWithSources.hpp"

void TestNamedEntityWithSources::setUp() {
  f1 = new File("test_block.h5", FileMode::ReadWrite);
}

void TestNamedEntityWithSources::tearDown() {
  delete f1;
}


void TestNamedEntityWithSources::testAddRemoveSource() {
  
}

