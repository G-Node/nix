// Copyright © 2013,2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include "TestH5Group.hpp"

#include "RefTester.hpp"

#include "hdf5/FileHDF5.hpp"

unsigned int &TestH5Group::open_mode()
{
    static unsigned int openMode = H5F_ACC_TRUNC;
    return openMode;
}

void TestH5Group::setUp() {
    unsigned int &openMode = open_mode();

    if (openMode == H5F_ACC_TRUNC) {
        h5file = H5Fcreate("test_group.h5", openMode, H5P_DEFAULT, H5P_DEFAULT);
    } else {
        h5file = H5Fopen("test_group.h5", openMode, H5P_DEFAULT);
    }

    CPPUNIT_ASSERT(H5Iis_valid(h5file));

    if (openMode == H5F_ACC_TRUNC) {
        h5group = H5Gcreate2(h5file, "tstGroup", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    } else {
        h5group = H5Gopen2(h5file, "tstGroup", H5P_DEFAULT);
    }

    openMode = H5F_ACC_RDWR;
}

void TestH5Group::tearDown() {
    H5Fclose(h5file);
    H5Oclose(h5group);
}

void TestH5Group::testBaseTypes() {
    nix::hdf5::H5Group group(h5group, true);

    //int
    //attr
    group.setAttr("t_int", 42);
    int tint;
    group.getAttr("t_int", tint);
    CPPUNIT_ASSERT_EQUAL(tint, 42);

    //double
    //attr
    double deps = std::numeric_limits<double>::epsilon();
    double dpi = boost::math::constants::pi<double>();

    group.setAttr("t_double", dpi);
    double dbl;
    group.getAttr("t_double", dbl);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(dpi, dbl, deps);

    //string
    const std::string testStr = "I saw the best minds of my generation destroyed by madness";
    group.setAttr("t_string", testStr);

    std::string retString;
    group.getAttr("t_string", retString);
    CPPUNIT_ASSERT_EQUAL(testStr, retString);
}

void TestH5Group::testMultiArray() {
    nix::hdf5::H5Group group(h5group, true);
    //arrays
    typedef boost::multi_array<double, 3> array_type;
    typedef array_type::index index;
    array_type A(boost::extents[3][4][2]);

    int values = 0;
    for(index i = 0; i != 3; ++i)
        for(index j = 0; j != 4; ++j)
            for(index k = 0; k != 2; ++k)
                A[i][j][k] = values++;

    group.setAttr<array_type>("t_doubleArray", A);

    array_type B(boost::extents[1][1][1]);
    group.getAttr("t_doubleArray", B);

    int verify = 0;
    int errors = 0;
    for(index i = 0; i != 3; ++i) {
        for(index j = 0; j != 4; ++j) {
            for(index k = 0; k != 2; ++k) {
                int v = verify++;
                errors += B[i][j][k] != v;
            }
        }
    }

    CPPUNIT_ASSERT_EQUAL(errors, 0);

    //data
    group.setData("t_doubleArray", A);

    array_type C(boost::extents[1][1][1]);
    group.getData("t_doubleArray", C);

    verify = 0;
    errors = 0;

    for(index i = 0; i != 3; ++i) {
        for(index j = 0; j != 4; ++j) {
            for(index k = 0; k != 2; ++k) {
                int v = verify++;
                errors += B[i][j][k] != v;
            }
        }
    }

    CPPUNIT_ASSERT_EQUAL(errors, 0);
}

void TestH5Group::testVector() {
    nix::hdf5::H5Group group(h5group, true);

    std::vector<int> iv;
    iv.push_back(7);
    iv.push_back(23);
    iv.push_back(42);
    iv.push_back(1982);

    group.setAttr("t_intvector", iv);

    std::vector<int> tiv;
    group.getAttr("t_intvector", tiv);
    assert_vectors_equal(iv, tiv);

    std::vector<std::string> sv;
    sv.push_back("Alle");
    sv.push_back("meine");
    sv.push_back("Entchen");

    group.setAttr("t_strvector", sv);

    std::vector<std::string> tsv;
    group.getAttr("t_strvector", tsv);
    assert_vectors_equal(sv, tsv);
}

void TestH5Group::testArray() {

    nix::hdf5::H5Group group(h5group, true);
    int ia1d[5] = {1, 2, 3, 4, 5};

    group.setAttr("t_intarray1d", ia1d);
    int tia1d[5] = {0, };
    group.getAttr("t_intarray1d", tia1d);

    for (int i = 0; i < 5; i++) {
        CPPUNIT_ASSERT_EQUAL(ia1d[i], tia1d[i]);
    }

#ifndef _WIN32 //cf. issue #72
    int ia2d[3][2] = { {1, 2}, {3, 4}, {5, 6} };
    group.setAttr("t_intarray2d", ia2d);
    int tia2d[3][2] = { {0, }, };
    group.getAttr("t_intarray2d", tia2d);

    for (int i = 0; i < 3*2; i++) {
        CPPUNIT_ASSERT_EQUAL(*(ia2d[0] + i), *(tia2d[0] + i));
    }
#endif
}

void TestH5Group::testOpen() {
    nix::hdf5::H5Group root(h5group, true);

    nix::hdf5::H5Group g = root.openGroup("name_a", true);
    std::string uuid = nix::util::createId();
    g.setAttr("entity_id", uuid);

    CPPUNIT_ASSERT(root.hasGroup("name_a"));

    std::string idout;

    boost::optional<nix::hdf5::H5Group> a = root.findGroupByNameOrAttribute("entity_id", "name_a");
    CPPUNIT_ASSERT(a);
    CPPUNIT_ASSERT(a->hasAttr("entity_id"));
    a->getAttr("entity_id", idout);
    CPPUNIT_ASSERT_EQUAL(uuid, idout);

    boost::optional<nix::hdf5::H5Group> b = root.findGroupByNameOrAttribute("entity_id", uuid);
    CPPUNIT_ASSERT(b);
    CPPUNIT_ASSERT(b->hasAttr("entity_id"));
    b->getAttr("entity_id", idout);
    CPPUNIT_ASSERT_EQUAL(uuid, idout);

}

void TestH5Group::testRefCount() {

    hid_t ha = H5Gopen2(h5file, "/", H5P_DEFAULT);
    test_refcounting<nix::hdf5::H5Group>(h5group, ha);
    H5Gclose(ha);
}
