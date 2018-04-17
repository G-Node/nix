![Nix_logo](./nix_logo.png "NIX")

C++ library for storing scientific data in the *NIX* data model.

<!-- The *NIX* project started as an initiative within the -->
<!-- Electrophysiology Task Force a part of the -->
<!-- [INCF](http://www.incf.org/) Data sharing Program.   -->

The *NIX* data model allows to store fully annotated scientific
datasets, i.e. the data together with its metadata within the same
container. Our aim is to achieve standardization by providing a
common/generic data structure for a multitude of data
types.
[See the wiki for more information](https://github.com/G-Node/nix/wiki)

<!-- NIX originates from neurosciences but the generic nature of the
underlying data model makes it applicable to a much wider range. In
fact we would be happy to know of datasets that cannot be represented
in NIX. -->

The current implementations store the actual data using
the [HDF5](http://www.hdfgroup.org/) file format as a storage backend.

*NIX* emerged from the activities of the Electrophysiology Task Force
of the [INCF](http://www.incf.org) Datasharing Program (2010-2015). It
is a registered research resource with
the
[RRID:SCR_016196](https://scicrunch.org/resources/Any/record/nlx_144509-1/SCR_016196/resolver?q=SCR_016196&l=SCR_016196).

* * *

# The NIX ecosystem

## APIs

- [*nix* c++ library](https://github.com/g-node/nix "C++ api for nix files"). C++ library for reading and writing NIX files.
- [*nixpy*](https://github.com/g-node/nixpy "Python library either as bindings or using h5py") native
  Python implementation of the NIX data model using h5py.

## Language bindings

We provide bindings for:

- [Matlab: *nix-mx*](https://github.com/g-node/nix-mx "Matlab language bindings, requires the C++ library")
- [Java: *nix-java*](https://github.com/g-node/nix-java "Java language bindings, requires the C++ library")

## Viewer

- Viewer for *NIX* data files [nixView](https://github.com/bendalab/nixview "NixView - viewer for nix files")


## Used by
- [relacs](http://relacs.sourceforge.net "Relacs - enjoy your recordings") is
  a software platform for closed-loop data acquisition, online
  analysis, and stimulus generation specifically designed for (but not
  limited to) electrophysiological recordings. Writes out NIX files if
  the nix c++-library is installed.

- [IO class](https://github.com/python-neo-nixio) for the [NEO](http://neuralensemble.org/neo/) data model for spike train data.

- Output format for [EEGBase](http://eegdatabase.kiv.zcu.cz)

* * *

# Getting started

## Installation

Platform specific installation instructions can be found:

- [GNU/Linux](./install_linux.md)
- [macOS](./install_mac.md)
- [Windows](./install_win.md)

<!-- ## NIX API Documentation -->

<!-- The API documentation for the C++ library can be found [here](http://g-node.github.io/nix/) -->

## Tutorial and examples

- We have assembled a set
 of
 [tutorials](http://g-node.github.io/nixpy/tutorial.html "Python Tutorial") and
 [demos](https://github.com/g-node/nix-demo "Jupiter notebooks demonstrating nix for various use-cases") using
 nixpy.

# Getting support

If you experience problems using *NIX* feel free to join our IRC channel
[#gnode at FreeNode](irc://irc.freenode.net/gnode) or write an email to <dev@g-node.org>. If you find a
bug please report it using
the [project issue tracker](https://github.com/G-Node/nix/issues "NIX issue tracker").


# Contributing

Any kind of contribution is welcome! This includes reporting bugs and
issues. If you want to contribute to code or documentation please
refer to
the
[contributing guide](https://github.com/G-Node/nix/blob/master/CONTRIBUTING.md "Contributing guide").


# License

This project is open source published under the BSD-3 license see [license file](https://github.com/G-Node/nix/blob/master/LICENSE) for details.

* * *

## Citing

If you use *NIX*, it would be much appreciated if you would cite it in publications with its identifier RRID:SCR_016196 and/or the reference:

*Stoewer A, Kellner CJ, Benda J, Wachtler T and Grewe J (2014). File format and library for neuroscience data and metadata. Front. Neuroinform. Conference Abstract: Neuroinformatics 2014. doi: 10.3389/conf.fninf.2014.18.00027*


### Referenced By

* Dragly et al (2018) [doi:10.3389/fninf.2018.000169](https://doi.org/10.3389/fninf.2018.000169)
* Papez et al (2017) [doi:10.3389/fninf.2017.00024](https://doi.org/10.3389/fninf.2017.00024)
* Grewe et al (2017) [doi:10.1073/pnas.1615561114](https://doi.org/10.1073/pnas.1615561114)
* Vanek et al (2016) [doi:10.1109/informatics.2015.7377849](https://doi.org/10.1109/informatics.2015.7377849)
* Rübel et al (2016) [doi:10.3389/fninf.2016.00048](https://doi.org/10.3389/fninf.2016.00048)
* Denker et al (2016) [doi:10.1007/978-3-319-50862-7_5](https://doi.org/doi:10.1007/978-3-319-50862-7_5)
* Teeters et al (2015) [doi:10.1016/j.neuron.2015.10.025](https://doi.org/doi:10.1016/j.neuron.2015.10.025)


[![Build Status](https://travis-ci.org/G-Node/nix.svg?branch=master)](https://travis-ci.org/G-Node/nix)
[![Build status](https://ci.appveyor.com/api/projects/status/cdupf2np8ffg5hjt/branch/master?svg=true)](https://ci.appveyor.com/project/stoewer/nix/branch/master)
[![Coverage Status](https://coveralls.io/repos/G-Node/nix/badge.svg?branch=master)](https://coveralls.io/r/G-Node/nix?branch=master)
