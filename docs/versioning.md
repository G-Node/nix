# NIX API and File Format Versioning

The NIX project consists of different components: At the core of it
is the **data model**, i.e. the description of the entities and their
connection and relationships. The NIX project also specifies how this
data model is saved to on-disk storage in different **file formats**,
most notably how it is represented in *HDF5*. Additionally a set of
standard software libraries for different languages have been created
that provide an **API** for reading and writing said *file formats*.

All three components, i.e. **data model**, **file format** and **API**
carry their own version number and some can change independently from
each other.

## File format versioning scheme

The file format is versioned with a triplet of integers, that carries
semantics, thus can and indeed *must* be used by libraries that want to
open NIX files to determine if the version of the file is compatible
with the library version (i.e. the file can be read or written to).

The meaning of the individual numbers are as follows:

```
X.Y.Z
    ^---- non-breaking change        [ 1: R, 2: R ]
  ^------ breaking change, backward  [ 1: R, 2: - ]
^-------- breaking change, both ways [ 1: -, 2: - ]
```

To understand what a *non-breaking* change is one needs to consider
the two critical scenarios that might occur:

   1. A new library (NL) wants to open a file that is older (OF)
   2. An old library (OL) wants to open a file that is newer (NF)


If all components but the last, i.e `Z`, match, it means for the
scenarios *1* and *2* that the library must be able to **read**
but **not write** the file. (The addition of the Group
entity or the DataFrame entity would be such a change.)

If `X` matches but `Y` is different, (1.) a new library (*NL*) must be
able to read the old file (*OF*), but (2.) an old library *OL* must not
read or write a new file *NF*. (To change the way booleans are stored
would be such a change)

If `X` is different there is no backwards or forwards compatibility and
the version number must match between file and library for it to open the
file. (Changing where the metadata is rooted or something along the lines
would be such a change)

## Library and API versioning scheme

The NIX library is versioned with a triplet of integers (like the file format).
The meaning of each integer in the version triplet defines that version's
compatibility with code written against other versions. Specifically, a change
in each number `X`, `Y`, or `Z` signifies the following:

```
X.Y.Z
    ^---- minor (non-breaking) change
  ^------ API change
^-------- Major API
```

### Minor changes

A change in `Z` (minor change) signifies that the library has been updated but
the API has not been affected. This may signify bug fixes, optimisations, or
minor changes in the code that do not have any effect on how programs are
written against NIX. For example, any program written against NIX 1.3.0 should
not require changes to work against 1.3.1.

### API changes

A change in `Y` signifies that the API has changed. A change in the API
means that programs written for older versions of NIX may not work with the
newer version (depending on whether the program uses functions that have
changed or not). When an API change occurs, the binding libraries need to be
updated to reflect the changes and therefore a change in `Y` in NIX should be
accompanied by a respective change in the binding library versions.

### Major API changes

A change in `X` signifies that major changes have been made to the API and the
library. Such major changes make all aspects of NIX incompatible with older
versions. Essentially, this kind of change means that both the library and the
file format have been redesigned.

Changing the `X` value of the library version is accompanied by the same change
in the file format version.  In other words, when the file format version
changes to version `2`, the library will also be changed to version `2`.

### Bindings

Bindings for other languages (Python, Java, Matlab) should work for all
versions that share the same API. Therefore, any bindings version should work
with any NIX version as long as `X` and `Y` match. Differences in the `Z` value
should not be considered when checking if a given binding library works against
a given NIX version.
