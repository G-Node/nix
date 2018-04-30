# Introduction to NIX (under construction)

In the following we provide a (gentle) introduction to the general
concepts behind the NIX data model and try to illustrate its design
and provide code examples showing its usage.

Code examples use the c++ library but almost all functions have direct
counterparts in the python library or the language bindings for Matlab
and java.

## Basic idea

The basic idea is to come up with a **generic** data model that
defines as few structures/entities as possible while being able to
represent a multitude of different data structures.  This implies that
the defined entities may be named in a way that may seem uncommon but
are more general than the *domain-specific* terms used in any
community.

* [Storing data](./storing_data.md)
* [Tagging points and regions](./tagging.md)
* [Annotating data](./annotating.md)

* [Working with files](./file_handling.md)
* [Working with data](./data_handling.md)
