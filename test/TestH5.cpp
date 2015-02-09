// Copyright © 2015 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include "TestH5.hpp"

#include <nix/hdf5/FileHDF5.hpp>
#include <nix/hdf5/ExceptionHDF5.hpp>

#include "RefTester.hpp"

#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <limits>


unsigned int & TestH5::open_mode()
{
    static unsigned int openMode = H5F_ACC_TRUNC;
    return openMode;
}


void TestH5::setUp() {
    unsigned int &openMode = open_mode();

    if (openMode == H5F_ACC_TRUNC) {
        h5file = H5Fcreate("test_h5.h5", openMode, H5P_DEFAULT, H5P_DEFAULT);
    } else {
        h5file = H5Fopen("test_h5.h5", openMode, H5P_DEFAULT);
    }

    CPPUNIT_ASSERT(H5Iis_valid(h5file));

    hid_t g;
    if (openMode == H5F_ACC_TRUNC) {
        g = H5Gcreate2(h5file, "h5", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    } else {
        g = H5Gopen2(h5file, "h5", H5P_DEFAULT);
    }

    CPPUNIT_ASSERT(H5Iis_valid(g));
    h5group = nix::hdf5::Group(g);

    openMode = H5F_ACC_RDWR;
}

void TestH5::tearDown() {
    h5group.close();
    H5Fclose(h5file);
}


void TestH5::testBase() {
    CPPUNIT_ASSERT(h5group.isValid());

    nix::hdf5::Group g_invalid{};
    CPPUNIT_ASSERT_EQUAL(false, g_invalid.isValid());
    CPPUNIT_ASSERT_THROW(g_invalid.check("Error"), nix::hdf5::H5Exception);

    // check check, heh
    CPPUNIT_ASSERT_THROW(check_h5_arg_name("foo/bar"), std::invalid_argument);

    // check HTri

    typedef nix::hdf5::HTri::value_type tri_type;

    nix::hdf5::HTri htri_true(static_cast<tri_type >(1));
    nix::hdf5::HTri htri_false(static_cast<tri_type >(0));
    nix::hdf5::HTri htri_error(static_cast<tri_type >(-1));

    CPPUNIT_ASSERT_EQUAL(static_cast<tri_type >(1), htri_true.value);
    CPPUNIT_ASSERT_EQUAL(true, htri_true.result());
    CPPUNIT_ASSERT_EQUAL(false, htri_true.isError());
    CPPUNIT_ASSERT_EQUAL(false, !htri_true);
    CPPUNIT_ASSERT_EQUAL(true, htri_true.check("Error"));

    CPPUNIT_ASSERT_EQUAL(static_cast<tri_type >(0), htri_false.value);
    CPPUNIT_ASSERT_EQUAL(false, htri_false.result());
    CPPUNIT_ASSERT_EQUAL(false, htri_false.isError());
    CPPUNIT_ASSERT_EQUAL(true, !htri_false);
    CPPUNIT_ASSERT_EQUAL(false, htri_false.check("Error"));

    CPPUNIT_ASSERT_EQUAL(static_cast<tri_type >(-1), htri_error.value);
    CPPUNIT_ASSERT_EQUAL(false, htri_error.result());
    CPPUNIT_ASSERT_EQUAL(true, htri_error.isError());
    CPPUNIT_ASSERT_EQUAL(true, !htri_error);
    CPPUNIT_ASSERT_THROW(htri_error.check("Error"), nix::hdf5::H5Exception);

    // check HErr

    typedef nix::hdf5::HErr::value_type err_type;

    nix::hdf5::HErr herr_success(static_cast<err_type>(1));
    nix::hdf5::HErr herr_fail(static_cast<err_type>(-1));

    CPPUNIT_ASSERT_EQUAL(static_cast<tri_type >(1), herr_success.value);
    CPPUNIT_ASSERT_EQUAL(false, herr_success.isError());
    CPPUNIT_ASSERT_EQUAL(false, !herr_success);
    CPPUNIT_ASSERT_EQUAL(true, herr_success.check("Error"));

    CPPUNIT_ASSERT_EQUAL(static_cast<tri_type >(-1), herr_fail.value);
    CPPUNIT_ASSERT_EQUAL(true, herr_fail.isError());
    CPPUNIT_ASSERT_EQUAL(true, !herr_fail);
    CPPUNIT_ASSERT_THROW(herr_fail.check("Error"), nix::hdf5::H5Error);

    nix::hdf5::HErr herr_default;
    CPPUNIT_ASSERT_EQUAL(true, herr_default.isError());

    //check BaseHDF5

    //ref counting
    hid_t ga = H5Gopen(h5file, "/", H5P_DEFAULT);
    hid_t gb = H5Gopen(h5file, "/h5", H5P_DEFAULT);

    CPPUNIT_ASSERT(ga > 0);
    CPPUNIT_ASSERT(gb > 0);

    test_refcounting<nix::hdf5::BaseHDF5>(ga, gb);

    test_refcounting<nix::hdf5::LocID>(ga, gb);

    //name()
    std::string name = h5group.name();

    CPPUNIT_ASSERT_EQUAL(std::string("/h5"), name);
    CPPUNIT_ASSERT_EQUAL(std::string{}, g_invalid.name());
}

void TestH5::testDataType() {
    namespace h5x = nix::hdf5::h5x;

    hid_t t_int = H5Tcopy(H5T_NATIVE_INT);
    hid_t t_dbl = H5Tcopy(H5T_NATIVE_DOUBLE);

    CPPUNIT_ASSERT(t_int > 0);
    CPPUNIT_ASSERT(t_dbl > 0);

    test_refcounting<h5x::DataType>(t_int, t_dbl);

    H5Tclose(t_int);
    H5Tclose(t_dbl);
    
    h5x::DataType v_str = h5x::DataType::makeStrType();
    CPPUNIT_ASSERT_EQUAL(true, v_str.isVariableString());

    h5x::DataType str_255 = h5x::DataType::makeStrType(255);
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(255), str_255.size());

}

void TestH5::testDataSpace() {

    std::vector<hsize_t> dims = {4, 6};
    hid_t ds = H5Screate_simple(static_cast<int>(dims.size()), dims.data(), nullptr);
    nix::hdf5::DataSpace space(ds);

    nix::NDSize es = space.extent();
    CPPUNIT_ASSERT_EQUAL(es.size(), dims.size());
    CPPUNIT_ASSERT_EQUAL(es[0], dims[0]);
    CPPUNIT_ASSERT_EQUAL(es[1], dims[1]);

    CPPUNIT_ASSERT_EQUAL(1, H5Iget_ref(ds));
    space.close();
}
