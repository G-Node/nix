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

#include "hdf5/FileHDF5.hpp"
#include "hdf5/h5x/H5Exception.hpp"
#include "hdf5/h5x/H5PList.hpp"

#include "RefTester.hpp"

#include <iostream>
#include <sstream>
#include <iterator>
#include <stdexcept>
#include <limits>
#include <cstring>


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
    h5group = nix::hdf5::H5Group(g);

    openMode = H5F_ACC_RDWR;
}

void TestH5::tearDown() {
    h5group.close();
    H5Fclose(h5file);
}


void TestH5::testBase() {
    CPPUNIT_ASSERT(h5group.isValid());

    nix::hdf5::H5Group g_invalid{};
    CPPUNIT_ASSERT_EQUAL(false, g_invalid.isValid());
    CPPUNIT_ASSERT_EQUAL(H5I_BADID, g_invalid.type());
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

    //check H5Object
    hid_t gcpl = H5Pcreate(H5P_GROUP_CREATE);
    hid_t dcpl = H5Pcreate(H5P_DATASET_CREATE);

    CPPUNIT_ASSERT(gcpl > 0);
    CPPUNIT_ASSERT(dcpl > 0);

    test_refcounting<nix::hdf5::H5Object>(gcpl, dcpl);

    CPPUNIT_ASSERT_EQUAL(1, H5Iget_ref(gcpl));
    CPPUNIT_ASSERT_EQUAL(1, H5Iget_ref(dcpl));

    //ref counting
    hid_t ga = H5Gopen(h5file, "/", H5P_DEFAULT);
    hid_t gb = H5Gopen(h5file, "/h5", H5P_DEFAULT);

    CPPUNIT_ASSERT(ga > 0);
    CPPUNIT_ASSERT(gb > 0);

    test_refcounting<nix::hdf5::H5Object>(ga, gb);

    CPPUNIT_ASSERT_EQUAL(1, H5Iget_ref(ga));
    CPPUNIT_ASSERT_EQUAL(1, H5Iget_ref(gb));

    test_refcounting<nix::hdf5::LocID>(ga, gb);

    CPPUNIT_ASSERT_EQUAL(1, H5Iget_ref(ga));
    CPPUNIT_ASSERT_EQUAL(1, H5Iget_ref(gb));

    H5Gclose(ga);
    H5Gclose(gb);

    test_refcounting<nix::hdf5::PList>(gcpl, dcpl);

    CPPUNIT_ASSERT_EQUAL(1, H5Iget_ref(gcpl));
    CPPUNIT_ASSERT_EQUAL(1, H5Iget_ref(dcpl));

    //name()
    std::string name = h5group.name();

    CPPUNIT_ASSERT_EQUAL(std::string("/h5"), name);
    CPPUNIT_ASSERT_EQUAL(std::string{}, g_invalid.name());

    //FIXME: this should be somewhere lese

    size_t sz_a = 10;
    CPPUNIT_ASSERT_EQUAL(sz_a, nix::check::fits_in_size_t(sz_a, "OOB"));

    //this is a bit of a hack to get a size that will be bigger then
    //size_t's max value on 64bit systems
    double sz_b = static_cast<double>(std::numeric_limits<size_t>::max()) * 2.0;
    CPPUNIT_ASSERT_THROW(nix::check::fits_in_size_t(sz_b, "OOB"), nix::OutOfBounds);
}

void TestH5::testDataType() {
    namespace h5x = nix::hdf5::h5x;

    hid_t t_int = H5Tcopy(H5T_NATIVE_INT);
    hid_t t_dbl = H5Tcopy(H5T_NATIVE_DOUBLE);

    CPPUNIT_ASSERT(t_int > 0);
    CPPUNIT_ASSERT(t_dbl > 0);

    test_refcounting<h5x::DataType>(t_int, t_dbl);

    CPPUNIT_ASSERT_EQUAL(1, H5Iget_ref(t_int));
    CPPUNIT_ASSERT_EQUAL(1, H5Iget_ref(t_dbl));

    h5x::DataType dt_int(t_int, false); // take ownership
    h5x::DataType dt_dbl(t_dbl, false);

    CPPUNIT_ASSERT_NO_THROW(dt_int.check("Should not be thrown!"));
    CPPUNIT_ASSERT_EQUAL(H5I_DATATYPE, dt_int.type());
    CPPUNIT_ASSERT_EQUAL(true, dt_int.isValid());
    CPPUNIT_ASSERT_EQUAL(H5Tget_class(t_int), dt_int.class_t());
    CPPUNIT_ASSERT_EQUAL(false, dt_int.isCompound());

    CPPUNIT_ASSERT_EQUAL(H5T_SGN_2, dt_int.sign());

    CPPUNIT_ASSERT(dt_int.equal(H5T_NATIVE_INT));
    CPPUNIT_ASSERT(!dt_int.equal(H5T_NATIVE_DOUBLE));

    h5x::DataType v_str = h5x::DataType::makeStrType();
    CPPUNIT_ASSERT_EQUAL(true, v_str.isVariableString());
    CPPUNIT_ASSERT_EQUAL(H5T_CSET_UTF8, v_str.cset());

    h5x::DataType str_255 = h5x::DataType::makeStrType(255);
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(255), str_255.size());

    h5x::DataType str_ascii = h5x::DataType::makeStrType(255, H5T_CSET_ASCII);
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(255), str_ascii.size());
    CPPUNIT_ASSERT_EQUAL(H5T_CSET_ASCII, str_ascii.cset());

    h5x::DataType otype = h5x::DataType::make(H5T_OPAQUE, 42);
    CPPUNIT_ASSERT_EQUAL(H5T_OPAQUE, otype.class_t());
    CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(42), otype.size());

    struct TestStruct {
        double d;
        int i;
    };

    h5x::DataType cmpd = h5x::DataType::makeCompound(sizeof(TestStruct));

    cmpd.insert("d", offsetof(TestStruct, d), dt_dbl);
    cmpd.insert("i", offsetof(TestStruct, i), dt_int);

    CPPUNIT_ASSERT_EQUAL(true, cmpd.isCompound());
    CPPUNIT_ASSERT_EQUAL(cmpd.size(), cmpd.size());
    CPPUNIT_ASSERT_EQUAL(2U, cmpd.member_count());
    CPPUNIT_ASSERT_EQUAL(std::string("d"), cmpd.member_name(0));
    CPPUNIT_ASSERT_EQUAL(std::string("i"), cmpd.member_name(1));
    CPPUNIT_ASSERT_EQUAL(cmpd.member_index("d"), unsigned(0));
    CPPUNIT_ASSERT_EQUAL(cmpd.member_index("i"), unsigned(1));
    CPPUNIT_ASSERT_EQUAL(offsetof(TestStruct, d), cmpd.member_offset(0));
    CPPUNIT_ASSERT_EQUAL(offsetof(TestStruct, i), cmpd.member_offset(1));
    CPPUNIT_ASSERT_EQUAL(H5T_FLOAT, cmpd.member_class(0));
    CPPUNIT_ASSERT_EQUAL(H5T_INTEGER, cmpd.member_class(1));
    CPPUNIT_ASSERT(cmpd.member_type(0).equal(cmpd.member_type("d")));
    CPPUNIT_ASSERT(cmpd.member_type(1).equal(cmpd.member_type("i")));
    CPPUNIT_ASSERT(cmpd.member_type(0).equal(H5T_NATIVE_DOUBLE));
    CPPUNIT_ASSERT(cmpd.member_type(1).equal(H5T_NATIVE_INT));
    CPPUNIT_ASSERT(cmpd.member_type(0) == H5T_NATIVE_DOUBLE);
    CPPUNIT_ASSERT(cmpd.member_type(1) == H5T_NATIVE_INT);

    {
        CPPUNIT_ASSERT_EQUAL(0, H5Iget_ref(H5T_NATIVE_DOUBLE));
        h5x::DataType nativeDouble = H5T_NATIVE_DOUBLE;
        CPPUNIT_ASSERT_NO_THROW(nativeDouble.check("Oh noes!"));
        CPPUNIT_ASSERT_EQUAL(0, H5Iget_ref(H5T_NATIVE_DOUBLE));
    }
    CPPUNIT_ASSERT_EQUAL(0, H5Iget_ref(H5T_NATIVE_DOUBLE));


    // enum test
    //  enum type int bool
    h5x::DataType etib = h5x::DataType::makeEnum(H5T_STD_I8LE);
    CPPUNIT_ASSERT_EQUAL(etib.class_t(), H5T_ENUM);
    CPPUNIT_ASSERT_EQUAL(etib.size(), h5x::DataType(H5T_STD_I8LE).size());

    CPPUNIT_ASSERT_EQUAL(etib.member_count(), 0U);
    bool b = true;
    etib.insert("TRUE", &b);
    CPPUNIT_ASSERT_EQUAL(etib.member_count(), 1U);
    etib.insert("FALSE", false);
    CPPUNIT_ASSERT_EQUAL(etib.member_count(), 2U);

    std::vector<std::string> nms = etib.member_names();
    auto rt = std::find(std::begin(nms), std::end(nms), "TRUE");
    auto rf = std::find(std::begin(nms), std::end(nms), "FALSE");

    CPPUNIT_ASSERT(rt != std::end(nms));
    CPPUNIT_ASSERT(rf != std::end(nms));

    b = false;
    etib.enum_valueof("TRUE", &b);
    CPPUNIT_ASSERT_EQUAL(true, b);

    h5x::DataType etnb = h5x::DataType::make(H5T_ENUM, sizeof(bool));
    CPPUNIT_ASSERT_EQUAL(etnb.class_t(), H5T_ENUM);
    etnb.insert("FALSE", false);
    etnb.insert("TRUE", true);

    CPPUNIT_ASSERT(etnb.enum_equal(etib));
    CPPUNIT_ASSERT(etib.enum_equal(etnb));

    h5x::DataType etlb = h5x::DataType::make(H5T_ENUM, sizeof(unsigned long));
    etlb.insert("FALSE", 0UL);
    etlb.insert("TRUE", 1UL);
    CPPUNIT_ASSERT(etlb.enum_equal(etib));

    h5x::DataType blt = nix::hdf5::data_type_to_h5_memtype(nix::DataType::Bool);

    struct ConverTest {
        char input;
        bool output;
    } cttests[] = {
        {0, false},
        {1, true},
        {2, true}
    };

    char buf[sizeof(bool)];

    for (size_t i = 0; i < (sizeof(cttests)/sizeof(ConverTest)); i++) {
        ConverTest tt = cttests[i];

        buf[0] = tt.input;
        nix::hdf5::HErr res = H5Tconvert(H5T_STD_B8LE, blt.h5id(), 1, buf, nullptr, H5P_DEFAULT);
        CPPUNIT_ASSERT(!res.isError());
        memcpy(&b, buf, sizeof(bool));
        bool lvout = tt.output;

        int eq = memcmp(&buf, &lvout, sizeof(bool));
        CPPUNIT_ASSERT_EQUAL(0, eq);
    }

    //const bool lvfalse = 0;
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

void TestH5::testPropertyList() {
    nix::hdf5::PList pl = nix::hdf5::PList::create(H5P_LINK_CREATE);
    pl.charEncoding(H5T_CSET_ASCII);
    CPPUNIT_ASSERT_EQUAL(H5T_CSET_ASCII, pl.charEncoding());

    pl.charEncoding(H5T_CSET_UTF8);
    CPPUNIT_ASSERT_EQUAL(H5T_CSET_UTF8, pl.charEncoding());

    nix::hdf5::PList pl_utf8 = nix::hdf5::PList::linkUTF8();
    CPPUNIT_ASSERT_EQUAL(H5T_CSET_UTF8, pl_utf8.charEncoding());
}

void TestH5::testUTF8() {
    nix::hdf5::H5Group g = h5group.openGroup("utf8");

    // Check the name of the group is in UTF-8
    H5L_info_t li;
    h5group.linkInfo("utf8", li);
    CPPUNIT_ASSERT_EQUAL(H5T_CSET_UTF8, li.cset);

    // Check the attribute name and contents are in UTF-8
    g.setAttr("str", std::string{"hopefully utf8"});
    nix::hdf5::Attribute attr = g.openAttr("str");

    nix::hdf5::PList apl = attr.createPList();
    CPPUNIT_ASSERT_EQUAL(H5T_CSET_UTF8, apl.charEncoding());

    nix::hdf5::h5x::DataType attr_dtype = attr.dataType();
    CPPUNIT_ASSERT_EQUAL(H5T_CSET_UTF8, attr_dtype.cset());

    std::vector<std::string> strs = {"a", "b", "c"};
    g.setData("dataStr", strs);

    // Check the DataSet name and contents are in UTF-8
    nix::hdf5::DataSet ds = g.openData("dataStr");

    nix::hdf5::h5x::DataType ds_dtype = ds.dataType();
    CPPUNIT_ASSERT_EQUAL(H5T_CSET_UTF8, ds_dtype.cset());

    g.linkInfo("dataStr", li);
    CPPUNIT_ASSERT_EQUAL(H5T_CSET_UTF8, li.cset);

    // Check link names are UTF-8
    h5group.createLink(g, "zelda");
    h5group.linkInfo("zelda", li);
    CPPUNIT_ASSERT_EQUAL(H5T_CSET_UTF8, li.cset);
}
