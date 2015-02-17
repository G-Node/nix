[![Build Status](https://travis-ci.org/G-Node/nix.svg?branch=master)](https://travis-ci.org/G-Node/nix)
[![Build status](https://ci.appveyor.com/api/projects/status/cdupf2np8ffg5hjt/branch/master?svg=true)](https://ci.appveyor.com/project/stoewer/nix/branch/master)
[![Coverage Status](https://coveralls.io/repos/G-Node/nix/badge.svg?branch=master)](https://coveralls.io/r/G-Node/nix?branch=master)

--

About NIX
-------------

The *NIX* project (previously called Pandora) started as an initiative of the Electrophysiology Task Force which is part
of the INCF Datasharing Program. As such the project aims to develop standardized methods and models for storing
electrophysiology and other neuroscience data together with their metadata in one common file format based on HDF5.

[See the wiki for more information](https://github.com/G-Node/nix/wiki)

Development Status
------------------

The *NIX* project is now in a [Beta](https://github.com/G-Node/nix/releases) stage and should not be seen as a final product. However, the data model as well as the HDF schema and the C++ api are feature complete and should undergo little to no changes in the future.

Getting Started (Linux)
-----------------------

**Debian PPA**

You can get a ready to install package for Ubuntu "trusty". You have to add & install the following software source to your system:

```
deb http://ppa.launchpad.net/gnode/nix/ubuntu trusty main 
deb-src http://ppa.launchpad.net/gnode/nix/ubuntu trusty main 
```
by executing 

```
sudo add-apt-repository ppa:gnode/nix
sudo apt-get update
sudo apt-get install nixlib-dev
```

**Build NIX under Ubuntu 14.04**

_Dependencies_

In order to build the NIX library a recent C++11 compatible compiler is needed (g++ >= 4.8, clang >= 3.4)
as well as the build tool CMake (>= 2.8.9). Further nix depends on the following third party libraries:

- HDF5 (version 1.8.13 or higher)
- Boost (version 1.49 or higher)
- CppUnit (version 1.12.1 or higher)

_Instructions_

```bash
# install dependencies
sudo apt-get install libboost-all-dev libhdf5-serial-dev libcppunit-dev cmake build-essential

**Note:** If the standard version of the boost libraries in your distribution is less than 1.49, 
# manually install a version larger than 1.49 from the launchad (https://launchpad.net/~boost-latest/+archive/ubuntu/ppa)

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

Getting Started (Windows)
-------------------------

**Windows Installer**

You can get a windows installer for both, 32- and 64-bit: [nix - Beta 1](https://github.com/G-Node/nix/releases)

**Build NIX under Windows**

To build NIX under windows follow the guide provided under: [Win32.md](https://github.com/G-Node/nix/blob/master/Win32.md)

NIX API Documentation
---------------------

The API documentation can be found [here](http://g-node.github.io/nix/)

NIX Tutorials
-------------

If you want to learn about the features of the NIX data model and how
it can be used to represent various kinds of data you may want to
check out the tutorials.  We have assembled a set of tutorial
[here](http://g-node.github.io/nixpy/tutorial.html) using the python
wrapper.
