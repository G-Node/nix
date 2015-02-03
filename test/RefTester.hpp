// Copyright © 2015 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include <nix/hdf5/BaseHDF5.hpp>

struct RefTester {
    RefTester(hid_t id) : obj(id) {
        if (H5Iis_valid(id)) {
            ref = H5Iget_ref(id);
        } else {
            ref = 0;
        }
    }

    void inc_check(nix::hdf5::BaseHDF5* wrapped = nullptr) {
        ref++;
        check(wrapped);
    }

    void dec_check(nix::hdf5::BaseHDF5* wrapped = nullptr) {
        ref--;
        check(wrapped);
    }

    void check(nix::hdf5::BaseHDF5* wrapped = nullptr) {
        CPPUNIT_ASSERT_EQUAL(H5Iget_ref(obj), ref);
        if (wrapped) {
            CPPUNIT_ASSERT_EQUAL(obj, wrapped->h5id());
            CPPUNIT_ASSERT_EQUAL(ref, wrapped->refCount());
        }
    }

    int   ref;
    hid_t obj;
};

template<typename T>
void test_refcounting(hid_t a_id, hid_t b_id) {

    RefTester a_tst = a_id;
    RefTester b_tst = b_id;

    T obj_invalid{};
    CPPUNIT_ASSERT_EQUAL(H5I_INVALID_HID, obj_invalid.h5id());

    T wa_copy(a_id, true); // +1
    a_tst.inc_check(&wa_copy);

    //non-transfering ownership test
    H5Iinc_ref(a_id); // +1, because if we transfer ownership, the need an extra one
    a_tst.inc_check();
    T wa_owner(a_id, false); // =, take ownership, no refcount change to a
    a_tst.check(&wa_owner);

    T wa_ic = a_id; // +1, currently makes a copy
    a_tst.inc_check(&wa_ic);

    {
        T tmp = T(a_id, true); //+1
        a_tst.inc_check(&tmp);
    }
    a_tst.dec_check();

    {
        T tmp = T(a_id, true); //+1
        a_tst.inc_check(&tmp);
        tmp.close();        //-1
        a_tst.dec_check();
    }

    a_tst.check();  // =

    wa_copy = wa_owner; //=, self assignment, i.e. no inc ref

    a_tst.check(&wa_copy);
    a_tst.check(&wa_owner);

    //now with two objects ...

    CPPUNIT_ASSERT(a_id != b_id);

    T wb_copy(b_id, true); // + 1
    b_tst.inc_check(&wb_copy);

    // copy-assignment ctor
    wa_copy = wb_copy; // + 1 b, - 1 a
    a_tst.dec_check();
    b_tst.inc_check(&wb_copy);
    b_tst.check(&wa_copy);
}