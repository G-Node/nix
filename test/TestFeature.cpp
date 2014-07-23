#include <ctime>

#include <nix/util/util.hpp>
#include "TestFeature.hpp"

using namespace std;
using namespace nix;


void TestFeature::setUp() {
    file = File::open("test_feature.h5", FileMode::Overwrite);
    block = file.createBlock("featureTest","test");

    vector<string> array_names = { "data_array_a", "data_array_b", "data_array_c",
                                   "data_array_d", "data_array_e" };
    vector<DataArray> refs;
    for (const auto &name : array_names) {
        refs.push_back(block.createDataArray(name,
                                             "reference",
                                             DataType::Double,
                                             {0}));
    }

    data_array = block.createDataArray("featureTest", "Test",
                                       DataType::Double, {0});
    tag = block.createSimpleTag("featureTest", "Test", refs);
}


void TestFeature::tearDown() {
    file.deleteBlock(block.id());
    file.close();
}


void TestFeature::testValidate() {
    Feature rp = tag.createFeature(data_array, nix::LinkType::Tagged);
    
    std::cout << std::endl << rp.validate();
}


void TestFeature::testId() {
    Feature rp = tag.createFeature(data_array, nix::LinkType::Tagged);
    CPPUNIT_ASSERT(rp.id().size() == 24);
    tag.deleteFeature(rp.id());
}


void TestFeature::testLinkType(){
    Feature rp = tag.createFeature(data_array, nix::LinkType::Tagged);
    CPPUNIT_ASSERT(rp.linkType() == nix::LinkType::Tagged);
    rp.linkType(nix::LinkType::Untagged);
    CPPUNIT_ASSERT(rp.linkType() == nix::LinkType::Untagged);
    rp.linkType(nix::LinkType::Tagged);
    CPPUNIT_ASSERT(rp.linkType() == nix::LinkType::Tagged);

    CPPUNIT_ASSERT_THROW(tag.createFeature(data_array, nix::LinkType::Indexed), std::runtime_error);
    rp.linkType(nix::LinkType::Indexed);
    CPPUNIT_ASSERT(rp.linkType() == nix::LinkType::Indexed);

    tag.deleteFeature(rp.id());
}


void TestFeature::testData() {
    Feature rp = tag.createFeature(data_array, nix::LinkType::Tagged);
    DataArray da_2 = block.createDataArray("array2", "Test",
                                           DataType::Double, {0});
    CPPUNIT_ASSERT(rp.data().id() == data_array.id());
    rp.data(da_2);
    CPPUNIT_ASSERT(rp.data().id() == da_2.id());
    block.deleteDataArray(da_2.id());
    tag.deleteFeature(rp.id());
}
