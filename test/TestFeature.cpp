#include <ctime>

#include <nix/util/util.hpp>
#include "TestFeature.hpp"

#include <nix/valid/validate.hpp>

using namespace std;
using namespace nix;
using namespace valid;


void TestFeature::setUp() {
    file = File::open("test_feature.h5", FileMode::Overwrite);
    block = file.createBlock("featureTest","test");

    data_array = block.createDataArray("featureTest", "Test",
                                       DataType::Double, nix::NDSize({ 0 }));
    
    tag = block.createTag("featureTest", "Test", {0.0, 2.0, 3.4});
}


void TestFeature::tearDown() {
    file.deleteBlock(block.id());
    file.close();
}


void TestFeature::testValidate() {
    Feature rp = tag.createFeature(data_array, nix::LinkType::Tagged);
    
    valid::Result result = validate(rp);
    CPPUNIT_ASSERT(result.getErrors().size() == 0);
    CPPUNIT_ASSERT(result.getWarnings().size() == 0);
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
                                           DataType::Double, nix::NDSize({ 0 }));
    CPPUNIT_ASSERT(rp.data().id() == data_array.id());
    rp.data(da_2);
    CPPUNIT_ASSERT(rp.data().id() == da_2.id());
    block.deleteDataArray(da_2.id());
    // make sure link is gone with deleted data array
    CPPUNIT_ASSERT_THROW(rp.data(), std::runtime_error);
    tag.deleteFeature(rp.id());
}
