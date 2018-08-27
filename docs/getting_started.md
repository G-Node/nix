# Introduction to *NIX*

In the following we provide an introduction to the general concepts of
the *NIX* data model, try to illustrate its design principles and
provide code examples showing the usage of
the [c++](https://github.com/g-node/nix) library for reading and
writing *NIX* files.

Code examples use the c++ library but almost all functions have direct
counterparts in the [python library](https://github.com/g-node/nixpy)
or the language bindings
for [Matlab](https://github.com/g-node/nix-mx)
and [java](https://github.com/g-node/nix-java).

## Basic idea

The basic idea of the *NIX* project is to come up with a **generic**
data model that defines as few structures/entities as possible while
being able to represent a multitude of different data structures,
allows for in-depth annotation and supports standardization.

Designing a **generic** data model implies that the defined entities
are named in a way that may seem uncommon but are more general than
the *domain-specific* terms used in any given field or community.

The idea of the *NIX* data model has been implemented using
the [HDF5](https://www.hdfgroup.org) file format.

## Tutorials

* [Storing data](./storing_data.md)
* [Tagging points and regions](./tagging.md)
* [Annotating data](./annotating.md)
* [Support for standardization](./standardization.md)

* [Working with files](./file_handling.md)
* [Working with data](./data_handling.md)
* [Finding things](./finding_things.md)

* [ER-schema of data model](./data_model.md)

* [FAQs](./faq.md)
