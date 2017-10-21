Linux installation
------------------

**Debian PPA**

You can get a ready to install package for Ubuntu "trusty"(14.04) and "xenial" (16.04). You have to add & install the following software source to your system:

```
deb http://ppa.launchpad.net/gnode/nix/ubuntu trusty main
deb-src http://ppa.launchpad.net/gnode/nix/ubuntu trusty main
```
by executing

```
sudo add-apt-repository ppa:gnode/nix
sudo apt-get update
sudo apt-get install libnix-dev
```

*Note:* replace trusty with xenial for 16.04


**Fedora copr**

A fedora 25+ package can be installed via copr. To do so:

```
sudo dnf copr enable gicmo/nix
sudo dnf install nixio-devel
```

**Build NIX under Ubuntu 14.04/16.04**

_Dependencies_

In order to build the NIX library a recent C++11 compatible compiler is needed (g++ >= 4.8, clang >= 3.4)
as well as the build tool CMake (>= 2.8.9). Further nix depends on the following third party libraries:

- HDF5 (version 1.8.13 or higher)
- Boost (version 1.49 or higher)
- CppUnit (version 1.12.1 or higher)

_Instructions_

```bash
# 1 install dependencies
sudo apt-get install libboost-all-dev libhdf5-serial-dev libcppunit-dev cmake build-essential

**Note:** If the standard version of the boost libraries in your distribution is less than 1.49,
# manually install a version larger than 1.49 from the launchad (https://launchpad.net/~boost-latest/+archive/ubuntu/ppa)

# 2 clone NIX
git clone https://github.com/G-Node/nix
cd nix

# 3 make a build dir and build nix
mkdir build
cd build
cmake ..
make all

# 4 run the unit tests
ctest

# 5 install
sudo make install
```



In case building nix fails because libboost-*** libraries are not found try the following:

```bash
# 3 make a build dir and build nix
mkdir build
cd build
cmake -DBoost_NO_BOOST_CMAKE=TRUE ..
make all
```
