[![Build Status](https://travis-ci.org/G-Node/nix.svg?branch=master)](https://travis-ci.org/G-Node/nix)
[![Build status](https://ci.appveyor.com/api/projects/status/1qlcasjg2fpqotig/branch/master?svg=true)](https://ci.appveyor.com/project/G-Node/nix/branch/master)
[![Coverage](https://codecov.io/gh/G-Node/nix/branch/master/graph/badge.svg)](https://codecov.io/gh/G-Node/nix)

---

![nix_logo](docs/nix_logo.png "NIX")

About NIX
---------

The *NIX* project started as an initiative within the
Electrophysiology Task Force a part of the
[INCF](http://www.incf.org/) Data sharing Program.  The *NIX* data
model allows to store fully annotated scientific dataset, i.e. the
data together with its metadata within the same container. Our aim is
to achieve standardization by providing a common/generic data
structure for a multitude of data types.  [See the wiki for more
information](https://github.com/G-Node/nix/wiki)


The current implementations store the actual data using the [HDF5](http://www.hdfgroup.org/) file format as a storage backend.

Documentation --- where to find which information?
--------------------------------------------------

Project documentation is split up into three parts:
1. Technical information: Please visit the GitHub repository [Wiki](https://github.com/G-Node/nix/wiki).
2. General Introduction and tutorial: Visit the project pages on [ReadTheDocs](https://nixio.readthedocs.io/en/latest/)
3. API documentation.

Further project related information:

- We have assembled a set of
 [tutorials](http://g-node.github.io/nixpy/tutorial.html "Python Tutorial") and
 [demos](https://github.com/g-node/nix-demo "Jupiter notebooks demonstrating nix for various use-cases") using the python library.


The NIX ecosystem
-----------------

In this repository you find the C++ library for reading and writing NIX files.

**Language bindings**

We provide bindings in various languages:

- [Python bindings: *nixpy*](https://github.com/g-node/nixpy "Python library either as bindings or using h5py")
- [Matlab: *nix-mx*](https://github.com/g-node/nix-mx "Matlab language bindings, requires the C++ library")
- [Java: *nix-java*](https://github.com/g-node/nix-java "Java language bindings, requires the C++ library")

[IO class](https://github.com/python-neo-nixio) for the [NEO](http://neuralensemble.org/neo/) data model for spike train data.

**Tools**

- Viewer for *NIX* data files [nixView](https://github.com/bendalab/nixview "NixView - viewer for nix files")


Getting Started
---------------

Platform specific installation instructions can be found:

- [Linux](docs/install_linux.md)
- [macOS](docs/install_mac.md)
- [Windows](docs/install_win.md)
