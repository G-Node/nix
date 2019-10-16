![Nix_logo](./nix_logo.png "NIX")

C++ library for storing scientific data in the *NIX* data model.

The *NIX* data model allows to store fully annotated scientific
datasets, i.e. the data together with its metadata within the same
container. Our aim is to achieve standardization by providing a
common/generic data structure for a multitude of data
types.


Documentation --- where to find which information?
--------------------------------------------------

Project documentation is split up into three parts:
1. General Introduction and tutorial: Visit the project pages on [ReadTheDocs](https://nixio.readthedocs.io/en/latest/)
2. Technical information: Please visit the [Wiki](https://github.com/G-Node/nix/wiki).
3. API documentations.

Further project related information:

- We have assembled a set of
 [tutorials](http://g-node.github.io/nixpy/tutorial.html "Python Tutorial") and
 [demos](https://github.com/g-node/nix-demo "Jupiter notebooks demonstrating nix for various use-cases") using the python library.


The NIX ecosystem
-----------------

In this repository you find the C++ library for reading and writing NIX files.

**Further libraries**
- [Native python library: *nixpy*](https://github.com/g-node/nixpy "Python library using h5py")
- [Matlab: *nix-mx*](https://github.com/g-node/nix-mx "Matlab language bindings, requires the C++ library")
- [Java: *nix-java*](https://github.com/g-node/nix-java "Java language bindings, requires the C++ library")

[IO class](https://github.com/python-neo-nixio) for the [NEO](http://neuralensemble.org/neo/) data model for spike train data.

**Tools**

- Viewer for *NIX* data files [nixView](https://github.com/bendalab/nixview "NixView - viewer for nix files")




[![Build Status](https://travis-ci.org/G-Node/nix.svg?branch=master)](https://travis-ci.org/G-Node/nix)
[![Build status](https://ci.appveyor.com/api/projects/status/1qlcasjg2fpqotig/branch/master?svg=true)](https://ci.appveyor.com/project/G-Node/nix/branch/master)
[![Coverage Status](https://coveralls.io/repos/G-Node/nix/badge.svg?branch=master)](https://coveralls.io/r/G-Node/nix?branch=master)
