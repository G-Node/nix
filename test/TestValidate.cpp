// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>
#include <functional>

#include <nix/valid/validator.hpp>
#include <nix/valid/checks.hpp>
#include <nix/valid/conditions.hpp>
#include <nix/valid/validate.hpp>

#include <nix.hpp>

#include "TestValidate.hpp"

using namespace nix;
using namespace valid;
using namespace std;

void TestValidate::setUp() {
    startup_time = time(NULL);
}

void TestValidate::tearDown() {
    return;
}

void TestValidate::test() {
    // dummy class to test empty checks
    class fooC {
    public:
        std::string getFoo () const { return std::string("I'm not empty!"); };
        std::string getBar () const { return std::string(); };
    };
    
    std::vector<std::string> vect = {"foo", "bar"};
    std::vector<std::string> vect2;
    fooC foobar;
    
    // success cases----------------------------------------------------
    // -----------------------------------------------------------------
    valid::Result myResult = validator({
        could(vect, &std::vector<std::string>::empty, isFalse(), {
            must(vect, &std::vector<std::string>::size, notSmaller(2), "notSmaller(2)") }),
        must(vect, &std::vector<std::string>::size, notSmaller(2), "notSmaller(2)"),
        must(vect2, &std::vector<std::string>::size, isSmaller(2), "isSmaller(2)"),
        should(vect, &std::vector<std::string>::size, notGreater(2), "notGreater(2)"),
        should(vect, &std::vector<std::string>::size, isGreater(0), "isGreater(0)"),
        must(vect, &std::vector<std::string>::size, notEqual<size_t>(0), "notEqual<size_t>(0)"),
        should(vect, &std::vector<std::string>::size, isEqual<size_t>(2), "isEqual<size_t>(2)"),
        must(vect2, &std::vector<std::string>::size, isFalse(), "isFalse()"),
        must(foobar, &fooC::getFoo, notEmpty(), "notEmpty()"),
        should(foobar, &fooC::getBar, isEmpty(), "isEmpty()")
    });
    // uncomment this to have debug info
    // std::cout << myResult;
    CPPUNIT_ASSERT(myResult.hasWarnings() == false);
    CPPUNIT_ASSERT(myResult.hasErrors() == false);
    
    // failure cases----------------------------------------------------
    // -----------------------------------------------------------------
    myResult = validator({
        could(vect, &std::vector<std::string>::empty, isFalse(), {
            must(vect, &std::vector<std::string>::size, notSmaller(3), "notSmaller(3)") }),
        must(vect, &std::vector<std::string>::size, notSmaller(3), "notSmaller(3)"),
        must(vect2, &std::vector<std::string>::size, isSmaller(0), "isSmaller(0)"),
        should(vect, &std::vector<std::string>::size, notGreater(1), "notGreater(1)"),
        should(vect, &std::vector<std::string>::size, isGreater(2), "isGreater(2)"),
        must(vect, &std::vector<std::string>::size, notEqual<size_t>(2), "notEqual<size_t>(2)"),
        should(vect, &std::vector<std::string>::size, isEqual<size_t>(0), "isEqual<size_t>(0)"),
        must(vect2, &std::vector<std::string>::size, notFalse(), "notFalse()"),
        must(foobar, &fooC::getFoo, isEmpty(), "notEmpty()"),
        should(foobar, &fooC::getBar, notEmpty(), "isEmpty()")
    });
    // uncomment this to have debug info
    // std::cout << myResult;
    CPPUNIT_ASSERT(myResult.getWarnings().size() == 4);
    CPPUNIT_ASSERT(myResult.getErrors().size() == 6);
    
    // entity success cases---------------------------------------------
    // -----------------------------------------------------------------
    // create file & block
    nix::File file = nix::File::open("test_validate.h5", nix::FileMode::Overwrite);
    nix::Block block = file.createBlock("block_one", "dataset");
    // create data array
    nix::DataArray array1 = block.createDataArray("array_one", "testdata", nix::DataType::Double, {0, 0, 0});
    nix::DataArray array2 = block.createDataArray("array_two", "testdata", nix::DataType::Double, {0, 0, 0});
    nix::DataArray array_tmp = block.createDataArray("array_tmp", "testdata", nix::DataType::Double, {0});
    typedef boost::multi_array<double, 3> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[3][4][2]);
    int values = 0;
    for(index i = 0; i != 3; ++i)
        for(index j = 0; j != 4; ++j)
            for(index k = 0; k != 2; ++k)
                A[i][j][k] = values++;
    array1.setData(A);
    array2.setData(A);
    // create references vector
    std::vector<nix::DataArray> refs = {array1, array2};
    // create position & extent vectors
    std::vector<double> extent, position;
    for(index i = 0; i < 3; ++i) {
        extent.push_back(i);
        position.push_back(i);
    }
    // create positions & extents arrays
    nix::DataArray positions = block.createDataArray("positions_DataArray", "dataArray", DataType::Double, {0, 0});
    nix::DataArray extents = block.createDataArray("extents_DataArray", "dataArray", DataType::Double, {0, 0});
    typedef boost::multi_array<double, 2> array2D_type;
    typedef array2D_type::index index;
    array2D_type B(boost::extents[5][3]);
    for(index i = 0; i < 5; ++i) {
        for(index j = 0; j < 3; ++j) {
            B[i][j] = 100.0*i;
        }
    }
    positions.setData(B);
    array2D_type C(boost::extents[5][3]);
    for(index i = 0; i < 5; ++i) {
        for(index j = 0; j < 3; ++j) {
            C[i][j] = 100.0*i;
        }
    }
    extents.setData(C);
    // create units
    std::vector<std::string> atomic_units = {"m", "cm", "mm"};
    std::vector<std::string> compound_units = {"mV*cm", "m*s", "s/cm"};
    // create data tag & simple tag
    nix::DataTag dtag = block.createDataTag("tag_one", "test_tag", positions);
    dtag.extents(extents);
    dtag.references(refs);
    dtag.units(atomic_units);
    nix::SimpleTag stag = block.createSimpleTag("tag_one", "test_tag", refs);
    stag.extent(extent);
    stag.position(position);
    stag.units(atomic_units);
    // create some tag like class with units- & unit-getter that allows compound units
    struct tag_tmp {
        const std::vector<std::string> &units_ref;
        tag_tmp(std::vector<std::string> &units) : units_ref(units) {}
        std::vector<std::string> units() const { return units_ref; }
        std::string unit() const { return units_ref.front(); }
        boost::optional<std::string> unito() const { 
            boost::optional<std::string> ret = units_ref.front(); 
            return ret; 
        }
    };
    tag_tmp units_tmp = tag_tmp(compound_units);
    // create set dimensions with correct number of labels
    nix::SetDimension dim_set1 = array1.appendSetDimension();
    nix::SetDimension dim_set2 = array1.appendSetDimension();
    nix::SetDimension dim_set3 = array1.appendSetDimension();
    dim_set1.labels({"label_a", "label_b", "label_c"});
    dim_set2.labels({"label_a", "label_b", "label_c", "label_d"});
    dim_set3.labels({"label_a", "label_b"});
    // create range dimensions with correct number of labels & correct unit
    nix::RangeDimension dim_range1 = array2.appendRangeDimension({1, 2, 3});
    nix::RangeDimension dim_range2 = array2.appendRangeDimension({1, 2, 3, 4});
    nix::RangeDimension dim_range3 = array2.appendRangeDimension({1, 2});
    dim_range1.unit(atomic_units[0]);
    dim_range2.unit(atomic_units[1]);
    dim_range3.unit(atomic_units[2]);
    
    
    myResult = validator({
        could(dtag, &nix::DataTag::positions, dimEquals(2), {
            must(dtag, &nix::DataTag::extents, dimEquals(2), "dimEquals(2)") }),
        must(  dtag,   &nix::DataTag::extents, dimEquals(2), "dimEquals(2)"),
        should(array1, &nix::DataArray::dimensions, dimLabelsMatchData(array1), "dimLabelsMatchData(array)"),
        must(  array2, &nix::DataArray::dimensions, dimTicksMatchData(array2),  "dimTicksMatchData(array)"),
        should(stag, &nix::SimpleTag::position, extentsMatchPositions(extent),  "extentsMatchPositions(extent)"),
        must(  dtag, &nix::DataTag::positions,  extentsMatchPositions(extents), "extentsMatchPositions(extents)"),
        must(  stag, &nix::SimpleTag::extent, extentsMatchRefs(refs), "extentsMatchRefs(refs); (stag)"),
        should(dtag, &nix::DataTag::extents,  extentsMatchRefs(refs), "extentsMatchRefs(refs); (dtag)"),
        must(  stag, &nix::SimpleTag::position, positionsMatchRefs(refs), "positionsMatchRefs(refs); (stag)"),
        should(dtag, &nix::DataTag::positions,  positionsMatchRefs(refs), "positionsMatchRefs(refs); (dtag)"),
        should(dim_range1, &nix::RangeDimension::unit, isAtomicUnit(), "isAtomicUnit(); (dim_range1)"),
        should(stag,       &nix::SimpleTag::units,     isAtomicUnit(), "isAtomicUnit(); (stag)"),
        must(units_tmp, &tag_tmp::unit,  isCompoundUnit(), "isCompoundUnit(); (units_tmp.unit)"),
        must(units_tmp, &tag_tmp::units, isCompoundUnit(), "isCompoundUnit(); (units_tmp.units)"),
        must(units_tmp, &tag_tmp::unito, isCompoundUnit(), "isCompoundUnit(); (units_tmp.unito)"),
        should(units_tmp, &tag_tmp::unit,  isValidUnit(), "isValidUnit(); (units_tmp.unit)"),
        must(  units_tmp, &tag_tmp::units, isValidUnit(), "isValidUnit(); (units_tmp.units)"),
        should(units_tmp, &tag_tmp::unito, isValidUnit(), "isValidUnit(); (units_tmp.unito)"),
        must(  stag, &nix::SimpleTag::references, tagRefsHaveUnits(atomic_units),       "tagRefsHaveUnits(atomic_units); (stag)"),
        should(stag, &nix::SimpleTag::references, tagUnitsMatchRefsUnits(atomic_units), "tagUnitsMatchRefsUnits(atomic_units); (stag)")
    });
    // uncomment this to have debug info
    std::cout << myResult;
    CPPUNIT_ASSERT(myResult.hasWarnings() == false);
    CPPUNIT_ASSERT(myResult.hasErrors() == false);
}
