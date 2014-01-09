#include "TestRepresentation.hpp"

void TestRepresentation::setUp() {
    f1 = new File("test_block.h5", FileMode::ReadWrite);

}

void TestRepresentation::tearDown() {
    delete f1;
}


void TestRepresentation::testCreate(){

}
