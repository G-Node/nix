// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "TestValidate.hpp"

#include <nix/valid/validator.hpp>
#include <nix/valid/checks.hpp>
#include <nix/valid/conditions.hpp>
#include <nix/valid/validate.hpp>
#include <nix.hpp>

#include <ctime>
#include <functional>
#include <numeric>
#include <math.h>

#include <boost/math/constants/constants.hpp>


using namespace nix;
using namespace valid;
using namespace std;

void TestValidate::setUp() {
    startup_time = time(NULL);
    // create file & block
    file = nix::File::open("test_validate.h5", nix::FileMode::Overwrite);
    block = file.createBlock("block_one", "dataset");
    // create data array
    array1 = block.createDataArray("array_one", "testdata", nix::DataType::Double, nix::NDSize({ 0, 0, 0 }));
    array2 = block.createDataArray("array_two", "testdata", nix::DataType::Double, nix::NDSize({ 0, 0, 0 }));
    array3 = block.createDataArray("array_three", "testdata", nix::DataType::Double, nix::NDSize({ 0, 0, 0 }));
    array4 = block.createDataArray("array_four", "sindata", nix::DataType::Double, nix::NDSize({ 0, 0}));
    // set references vector
    refs = {array2, array3};
    // create positions & extents arrays
    positions = block.createDataArray("positions_DataArray", "dataArray", DataType::Double, nix::NDSize({ 0, 0 }));
    extents = block.createDataArray("extents_DataArray", "dataArray", DataType::Double, nix::NDSize({ 0, 0 }));
    // create units
    atomic_units = {"m", "cm", "mm"};
    compound_units = {"mV*cm", "m*s", "s/cm"};
    invalid_units = {"foo"};
    // create multi tag & tag
    mtag = block.createMultiTag("tag_one", "test_tag", positions);
    mtag.extents(extents);
    mtag.references(refs);
    tag = block.createTag("tag_one", "test_tag", {0.0, 2.0, 3.4});
    tag.references(refs);
    units_tmp = tag_tmp(compound_units);
    // create dimensions
    dim_set1 = array1.appendSetDimension();
    dim_set2 = array1.appendSetDimension();
    dim_set3 = array1.appendSetDimension();
    dim_range1 = array2.appendRangeDimension({1, 2, 3});
    dim_range2 = array2.appendRangeDimension({1, 2, 3, 4});
    dim_range3 = array2.appendRangeDimension({1, 2});
    dim_sample1 = array3.appendSampledDimension(42);
    dim_sample2 = array3.appendSampledDimension(42);
    dim_sample3 = array3.appendSampledDimension(42);
}

void TestValidate::tearDown() {
    return;
}

void TestValidate::setValid() {
    // fill sinus data & leave it in file for plot testing
    typedef boost::multi_array<double, 2> array2D_type;
    typedef array2D_type::index index;
    array2D_type sin_array(boost::extents[1000][1000]);
    const double PI = boost::math::constants::pi<double>();
    for (index i = 0; i < 1000; ++i) {
        for (index j = 0; j < 1000; ++j) {
            sin_array[i][j] = std::sin(PI * j / std::sqrt((i>0)?i:1));
        }
    }
    array4.setData(sin_array);
    for (ndsize_t i = array4.dimensionCount(); i > 0 ; --i) {
        array4.deleteDimension(i);
    }
    array4.appendSampledDimension(1.);
    array4.appendSetDimension();
    // fill array1 & array2
    typedef boost::multi_array<double, 3> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[3][4][2]);
    int values = 0;
    for (index i = 0; i != 3; ++i)
        for (index j = 0; j != 4; ++j)
            for (index k = 0; k != 2; ++k)
                A[i][j][k] = values++;
    array1.setData(A);
    array2.setData(A);
    for (ndsize_t i = array2.dimensionCount(); i > 0; --i) {
        array2.deleteDimension(i);
    }
    dim_range1 = array2.appendRangeDimension({1, 2, 3});
    dim_range2 = array2.appendRangeDimension({1, 2, 3, 4});
    dim_range3 = array2.appendRangeDimension({1, 2});

    array3.setData(A);

    // fill extent & position
    extent.resize(3);
    position.resize(3);
    std::iota(std::begin(extent), std::end(extent), 0);
    std::iota(std::begin(position), std::end(position), 0);

    // fill extents & positions
    array2D_type B(boost::extents[5][3]);
    for (index i = 0; i < 5; ++i) {
        for (index j = 0; j < 3; ++j) {
            B[i][j] = 100.0*i;
        }
    }
    positions.setData(B);
   
    array2D_type C(boost::extents[5][3]);
    for (index i = 0; i < 5; ++i) {
        for (index j = 0; j < 3; ++j) {
            C[i][j] = 100.0*i;
        }
    }
    extents.setData(C);
    
    // ensure correct dimension descriptors for positions
    for (ndsize_t i = positions.dimensionCount(); i > 0; --i) {
        positions.deleteDimension(i);
    }
    positions.appendSetDimension();
    positions.appendSetDimension();
    for (ndsize_t i = extents.dimensionCount(); i > 0; --i) {
        extents.deleteDimension(i);
    }
    extents.appendSetDimension();
    extents.appendSetDimension();

    // fill MultiTag
    refs = {array2, array3};
    mtag.units(atomic_units);
    // fill Tag
    tag.extent(extent);
    tag.position(position);
    tag.units(atomic_units);
    // fill dimensions
    dim_set1.labels({"label_a", "label_b", "label_c"});
    dim_set2.labels({"label_a", "label_b", "label_c", "label_d"});
    dim_set3.labels({"label_a", "label_b"});
    dim_range1.unit(atomic_units[0]);
    dim_range2.unit(atomic_units[1]);
    dim_range3.unit(atomic_units[2]);
    dim_sample1.unit(atomic_units[0]);
    dim_sample2.unit(atomic_units[1]);
    dim_sample3.unit(atomic_units[2]);
    // fill tag_tmp
    units_tmp = tag_tmp(compound_units);
    
    return;
}

void TestValidate::setInvalid() {
    // fill array1 & array2
    typedef boost::multi_array<double, 3> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[3][4][2]);
    int values = 0;
    for (index i = 0; i != 3; ++i)
        for (index j = 0; j != 4; ++j)
            for (index k = 0; k != 2; ++k)
                A[i][j][k] = values++;
    array1.setData(A);
    array2.setData(A);
    array3.setData(A);

    // fill extent & position
    extent.resize(6);
    position.resize(9);
    std::iota(std::begin(extent), std::end(extent), 0);
    std::iota(std::begin(position), std::end(position), 0);

    // fill extents & positions
    typedef boost::multi_array<double, 2> array2D_type;
    typedef array2D_type::index index;
    array2D_type B(boost::extents[4][2]);
    for (index i = 0; i < 4; ++i) {
        for (index j = 0; j < 2; ++j) {
            B[i][j] = 100.0*i;
        }
    }
    extents.setData(B);
    array2D_type C(boost::extents[5][2]);
    for (index i = 0; i < 5; ++i) {
        for (index j = 0; j < 2; ++j) {
            C[i][j] = 100.0*i;
        }
    }
    positions.setData(C);
    // fill MultiTag
    refs = {array1, array2};
    mtag.units(atomic_units);
    // fill Tag
    tag.extent(extent);
    tag.position(position);
    tag.units(atomic_units);
    // fill dimensions
    dim_set3.labels({"label_a", "label_b", "label_c"});
    dim_set1.labels({"label_a", "label_b", "label_c", "label_d"});
    dim_set2.labels({"label_a", "label_b"});
    dim_range3.ticks({1, 2, 3});
    dim_range1.ticks({1, 2, 3, 4});
    dim_range2.ticks({1, 2});
    dim_sample3.unit(atomic_units[0]);
    dim_sample1.unit(atomic_units[1]);
    dim_sample2.unit(atomic_units[2]);
    // fill tag_tmp
    units_tmp = tag_tmp(invalid_units);
    // remove dimension descriptors from array4, position and extents
    for (ndsize_t i = array4.dimensionCount(); i > 0; --i) {
        array4.deleteDimension(i);
    }
    for (ndsize_t i = positions.dimensionCount(); i > 0; --i) {
        positions.deleteDimension(i);
    }
    for (ndsize_t i = extents.dimensionCount(); i > 0; --i) {
        extents.deleteDimension(i);
    }
    return;
}

void TestValidate::test() {
    // dummy class to test empty checks
    class fooC {
    public:
        std::string getFoo () const { return std::string("I'm not empty!"); };
        std::string getBar () const { return std::string(); };
        std::vector<int> getSorted () const { return std::vector<int>({1, 2, 3}); };
        std::vector<int> getUnsorted () const { return std::vector<int>({3, 1, 2}); };
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
        should(foobar, &fooC::getBar, isEmpty(), "isEmpty()"),
        should(foobar, &fooC::getSorted, isSorted(), "isSorted()")
    });
    // have debug info
    // std::cout << myResult;
    CPPUNIT_ASSERT_EQUAL(true, myResult.ok());
    CPPUNIT_ASSERT_EQUAL(false, myResult.hasWarnings());
    CPPUNIT_ASSERT_EQUAL(false, myResult.hasErrors());
 
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
        should(foobar, &fooC::getBar, notEmpty(), "isEmpty()"),
        should(foobar, &fooC::getUnsorted, isSorted(), "isSorted()")
    });
    // uncomment this to have debug info
    // std::cout << myResult;
    CPPUNIT_ASSERT(myResult.getWarnings().size() == 5);
    CPPUNIT_ASSERT(myResult.getErrors().size() == 6);
    
    // entity success cases---------------------------------------------
    // -----------------------------------------------------------------
    setValid();
    myResult = validator({
        could(mtag, &nix::MultiTag::positions, dimEquals(2), {
            must(mtag, &nix::MultiTag::extents, dimEquals(2), "dimEquals(2)") }),
        must(  mtag,   &nix::MultiTag::extents, dimEquals(2), "dimEquals(2)"),
        should(array1, &nix::DataArray::dimensions, dimLabelsMatchData(array1), "dimLabelsMatchData(array)"),
        must(  array2, &nix::DataArray::dimensions, dimTicksMatchData(array2),  "dimTicksMatchData(array)"),
        should(tag, &nix::Tag::position, extentsMatchPositions(extent),  "extentsMatchPositions(extent)"),
        must(  mtag, &nix::MultiTag::positions,  extentsMatchPositions(extents), "extentsMatchPositions(extents)"),
        must(  tag, &nix::Tag::extent, extentsMatchRefs(refs), "extentsMatchRefs(refs); (tag)"),
        should(mtag, &nix::MultiTag::extents,  extentsMatchRefs(refs), "extentsMatchRefs(refs); (mtag)"),
        must(  tag, &nix::Tag::position, positionsMatchRefs(refs), "positionsMatchRefs(refs); (tag)"),
        must(mtag, &nix::MultiTag::positions,  positionsMatchRefs(refs), "positionsMatchRefs(refs); (mtag)"),
        should(dim_range1, &nix::RangeDimension::unit, isAtomicUnit(), "isAtomicUnit(); (dim_range1)"),
        should(tag,       &nix::Tag::units,     isAtomicUnit(), "isAtomicUnit(); (tag)"),
        must(units_tmp, &tag_tmp::unit,  isCompoundUnit(), "isCompoundUnit(); (units_tmp.unit)"),
        must(units_tmp, &tag_tmp::units, isCompoundUnit(), "isCompoundUnit(); (units_tmp.units)"),
        must(units_tmp, &tag_tmp::unito, isCompoundUnit(), "isCompoundUnit(); (units_tmp.unito)"),
        should(units_tmp, &tag_tmp::unit,  isValidUnit(), "isValidUnit(); (units_tmp.unit)"),
        must(  units_tmp, &tag_tmp::units, isValidUnit(), "isValidUnit(); (units_tmp.units)"),
        should(units_tmp, &tag_tmp::unito, isValidUnit(), "isValidUnit(); (units_tmp.unito)"),
        must(  tag, &nix::Tag::references, tagRefsHaveUnits(atomic_units),       "tagRefsHaveUnits(atomic_units); (tag)"),
        must(tag, &nix::Tag::references, tagUnitsMatchRefsUnits(atomic_units), "tagUnitsMatchRefsUnits(atomic_units); (tag)")
    });
    // have debug info
    // std::cout << myResult;
    CPPUNIT_ASSERT_EQUAL(false, myResult.hasWarnings());
    CPPUNIT_ASSERT_EQUAL(false, myResult.hasErrors());
    
    myResult = file.validate();
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), myResult.getWarnings().size());
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), myResult.getErrors().size());
    // entity failure cases---------------------------------------------
    // -----------------------------------------------------------------
    
    setInvalid();
    myResult = validator({
        could(mtag, &nix::MultiTag::positions, dimEquals(2), {
            must(mtag, &nix::MultiTag::extents, dimEquals(42), "dimEquals(42)") }),//
        must(  mtag,   &nix::MultiTag::extents, dimEquals(42), "dimEquals(42)"),//
        should(array1, &nix::DataArray::dimensions, dimLabelsMatchData(array1), "dimLabelsMatchData(array)"),
        must(tag, &nix::Tag::position, extentsMatchPositions(extent),  "extentsMatchPositions(extent)"),//
        must(  mtag, &nix::MultiTag::positions,  extentsMatchPositions(extents), "extentsMatchPositions(extents)"),
        must(  tag, &nix::Tag::extent, extentsMatchRefs(refs), "extentsMatchRefs(refs); (tag)"),
        must(mtag, &nix::MultiTag::extents,  extentsMatchRefs(refs), "extentsMatchRefs(refs); (mtag)"),
        must(  tag, &nix::Tag::position, positionsMatchRefs(refs), "positionsMatchRefs(refs); (tag)"),
        must(mtag, &nix::MultiTag::positions,  positionsMatchRefs(refs), "positionsMatchRefs(refs); (mtag)"),
        must(units_tmp, &tag_tmp::unit,  isAtomicUnit(), "isAtomicUnit(); (units_tmp.unit)"),
        must(units_tmp, &tag_tmp::units, isAtomicUnit(), "isAtomicUnit(); (units_tmp.units)"),
        must(units_tmp, &tag_tmp::unit,  isCompoundUnit(), "isCompoundUnit(); (units_tmp.unit)"),
        must(units_tmp, &tag_tmp::units, isCompoundUnit(), "isCompoundUnit(); (units_tmp.units)"),
        must(units_tmp, &tag_tmp::unito, isCompoundUnit(), "isCompoundUnit(); (units_tmp.unito)"),
        should(units_tmp, &tag_tmp::unit,  isValidUnit(), "isValidUnit(); (units_tmp.unit)"),
        must(  units_tmp, &tag_tmp::units, isValidUnit(), "isValidUnit(); (units_tmp.units)"),
        should(units_tmp, &tag_tmp::unito, isValidUnit(), "isValidUnit(); (units_tmp.unito)"),
        must(  tag, &nix::Tag::references, tagRefsHaveUnits(invalid_units),       "tagRefsHaveUnits(atomic_units); (tag)"),
        must(tag, &nix::Tag::references, tagUnitsMatchRefsUnits(invalid_units), "tagUnitsMatchRefsUnits(atomic_units); (tag)")
    });
    // std::cout << myResult;
    CPPUNIT_ASSERT(myResult.getWarnings().size() == 3);
    CPPUNIT_ASSERT(myResult.getErrors().size() == 16);

    myResult = file.validate();
    // std::cout << myResult;
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(0), myResult.getWarnings().size());
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(11), myResult.getErrors().size());
    
    // uncomment this to have debug info
    // std::cout << myResult;
    // lets leave the file clean & valid
    setValid();
}
