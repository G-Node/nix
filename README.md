About NIX
-------------

The NIX project (previously called Pandora) started as an initiative of the Electrophysiology Task Force which is part
of the INCF Datasharing Program. As such the project aims to develop standardized methods and models for storing
electrophysiology and other neuroscience data together with their metadata in one common file format based on HDF5.

[See the wiki for more information](https://github.com/G-Node/nix/wiki)


Build Dependencies
------------------

In order to build the NIX library a recent C++11 compatible compiler is needed (g++ 4.8, clang 3.4 or Visual Studio 2013)
as well as the build tool CMake. Further nix depends on the following third party libraries:

- HDF5 (version 1.8 or higher)
- Boost
- CppUnit


Build NIX under Ubuntu 14.04
----------------------------

```bash
# install dependencies
sudo apt-get install libboost-all-dev libhdf5-serial-dev libcppunit-dev cmake build-essential

# clone NIX
git clone https://github.com/G-Node/nix
cd nix

# make a build dir and build nix
mkdir build
cd build
cmake ..
make all

# run the unit tests
ctest

# install
sudo make install
```

NIX API Documentation
---------------------

The API documentation can be found [here](http://g-node.github.io/nix/)

--

[![Build Status](https://travis-ci.org/G-Node/nix.png?branch=master)](https://travis-ci.org/G-Node/nix)
