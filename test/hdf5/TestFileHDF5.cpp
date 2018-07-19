// Copyright © 2016 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include "TestFileHDF5.hpp"

#include "hdf5/h5x/H5Object.hpp"
#include "hdf5/h5x/H5Group.hpp"
#include "hdf5/FileHDF5.hpp"

#include <sstream>
#include <nix/util/util.hpp>

namespace h5x = nix::hdf5;

static std::string make_file_with_version(int x, int y, int z) {
    std::stringstream buf;
    buf << "nix-version-" << x << "." << y << "." << z << ".nix";

    std::string fn = buf.str();
    h5x::H5Object file = H5Fcreate (fn.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    file.check("Could not create plain h5 file :(");

    h5x::H5Group root = H5Gopen(file.h5id(), "/", H5P_DEFAULT);
    root.check("Could not open root group");
    root.setAttr("format", std::string("nix"));
    root.setAttr("version", std::vector<int>{x, y, z});

    root.openGroup("data");
    root.openGroup("metadata");

    time_t t = time(NULL);
    root.setAttr("created_at", nix::util::timeToStr(t));
    root.setAttr("updated_at", nix::util::timeToStr(t));

    root.close();
    file.close();
    return fn;
}

#define ASSERT_NOOPEN(str__, mode__) CPPUNIT_ASSERT_THROW(static_cast<void>(nix::File::open(str__, mode__)),  nix::InvalidFile)

void TestFileHDF5::testVersion() {

    nix::FormatVersion ver = HDF5_FF_VERSION;

    // simulate OLDER files
    // non-breaking change
    if (ver.z() > 0) {
        std::string nbc = make_file_with_version(ver.x(), ver.y(), ver.z() - 1);
        nix::File f = nix::File::open(nbc.c_str(), nix::FileMode::ReadOnly);
        CPPUNIT_ASSERT(f.isOpen());
        f.close();

        ASSERT_NOOPEN(nbc.c_str(), nix::FileMode::ReadWrite);
    }

    if (ver.y() > 0) {
        std::string nbc = make_file_with_version(ver.x(), ver.y() - 1, ver.z());
        nix::File f = nix::File::open(nbc.c_str(), nix::FileMode::ReadOnly);
        CPPUNIT_ASSERT(f.isOpen());
        f.close();

        ASSERT_NOOPEN(nbc.c_str(), nix::FileMode::ReadWrite);
    }

    // simulate NEWER files
    // z change, can still read
    std::string nbc = make_file_with_version(ver.x(), ver.y(), ver.z() + 1);
    nix::File f = nix::File::open(nbc.c_str(), nix::FileMode::ReadOnly);
    CPPUNIT_ASSERT(f.isOpen());
    f.close();
    // but cannot write
    ASSERT_NOOPEN(nbc.c_str(), nix::FileMode::ReadWrite);

    // newer y (major breaking change), neither read nor write
    std::string mbc = make_file_with_version(ver.x(), ver.y() + 1, ver.z());
    ASSERT_NOOPEN(mbc.c_str(), nix::FileMode::ReadWrite);
    ASSERT_NOOPEN(mbc.c_str(), nix::FileMode::ReadOnly);

    // newer x (huge breaking change), all hope is lost
    std::string hbc = make_file_with_version(ver.x() + 1, ver.y(), ver.z());
    ASSERT_NOOPEN(hbc.c_str(), nix::FileMode::ReadWrite);
    ASSERT_NOOPEN(hbc.c_str(), nix::FileMode::ReadOnly);

    // so all hope is lost ... or is it? Lets force it!
    {
        nix::File f = nix::File::open(mbc.c_str(),
                                      nix::FileMode::ReadOnly,
                                      "hdf5",
                                      nix::Compression::Auto,
                                      nix::OpenFlags::Force);
        CPPUNIT_ASSERT(f.isOpen());
        f.close();
    }
}
