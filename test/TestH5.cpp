// Copyright © 2015 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include <nix/hdf5/FileHDF5.hpp>
#include <nix/hdf5/ExceptionHDF5.hpp>
#include "TestH5.hpp"

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
    H5Idec_ref(g);

    openMode = H5F_ACC_RDWR;
}

void TestH5::tearDown() {
    h5group.close();
    H5Fclose(h5file);
}


void TestH5::testBase() {

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

    //check BaseHDF5

    std::string name = h5group.name();

    CPPUNIT_ASSERT_EQUAL(std::string("/h5"), name);

    nix::hdf5::Group g_invalid{};
    CPPUNIT_ASSERT_EQUAL(std::string{}, g_invalid.name());

}

void TestH5::testDataSpace() {

    std::vector<hsize_t> dims = {4, 6};
    hid_t ds = H5Screate_simple(static_cast<int>(dims.size()), dims.data(), nullptr);
    nix::hdf5::DataSpace space(ds);

    nix::NDSize es = space.extent();
    CPPUNIT_ASSERT_EQUAL(es.size(), dims.size());
    CPPUNIT_ASSERT_EQUAL(es[0], dims[0]);
    CPPUNIT_ASSERT_EQUAL(es[1], dims[1]);

    space.close();
    CPPUNIT_ASSERT_EQUAL(1, H5Iget_ref(ds));

    H5Sclose(ds);

}
