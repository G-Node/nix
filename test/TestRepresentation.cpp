#include <ctime>

#include <nix/util/util.hpp>
#include "TestRepresentation.hpp"

using namespace std;
using namespace nix;


void TestRepresentation::setUp() {
    file = File::open("test_representation.h5", FileMode::Overwrite);
    block = file.createBlock("representationTest","test");

    vector<string> array_names = { "data_array_a", "data_array_b", "data_array_c",
                                   "data_array_d", "data_array_e" };
    vector<DataArray> refs;
    for (auto it = array_names.begin(); it != array_names.end(); it++) {
        refs.push_back(block.createDataArray(*it, "reference"));
    }

    data_array = block.createDataArray("representationTest", "Test");
    tag = block.createSimpleTag("representationTest", "Test", refs);
}


void TestRepresentation::tearDown() {
    file.deleteBlock(block.id());
    file.close();
}


void TestRepresentation::testId() {
    Representation rp = tag.createRepresentation(data_array, nix::LinkType::Indexed);
    CPPUNIT_ASSERT(rp.id().size() == 31);
    tag.deleteRepresentation(rp.id());
}


void TestRepresentation::testLinkType(){
    Representation rp = tag.createRepresentation(data_array, nix::LinkType::Indexed);
    CPPUNIT_ASSERT(rp.linkType() == nix::LinkType::Indexed);
    rp.linkType(nix::LinkType::Tagged);
    CPPUNIT_ASSERT(rp.linkType() == nix::LinkType::Tagged);
    rp.linkType(nix::LinkType::Untagged);
    CPPUNIT_ASSERT(rp.linkType() == nix::LinkType::Untagged);
    tag.deleteRepresentation(rp.id());
}


void TestRepresentation::testData() {
    Representation rp = tag.createRepresentation(data_array, nix::LinkType::Indexed);
    DataArray da_2 = block.createDataArray("array2", "Test");
    CPPUNIT_ASSERT(rp.data().id() == data_array.id());
    rp.data(da_2);
    CPPUNIT_ASSERT(rp.data().id() == da_2.id());
    block.deleteDataArray(da_2.id());
    tag.deleteRepresentation(rp.id());
}
