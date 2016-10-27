# NIX API and file format versioning

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

## file format versioning scheme

The file format is versioned with a triplet of integers, that carry
semantics and can and must be used libraries that are about to open
NIX files to determine if it indeed can read or write the file.

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


If all components but the last, **Z** match, it means for the
for the scenarios *1* and *2* that the library must be able to
**read** but **not write** the file. (The addition of the Group
entity or the DataFrame entity would be such a change.)

If X matches but **Y** is different, (1.) a new library (*NL*) must be
able to read the old file (*OF*), but (2.) an old library *OL* must not
read or write a new file *NF*. (To change the way booleans are stored
would be such a change)

If **X** is different there is no backwards or forwards compatibility and
the version number must match between file and library for it to open the
file. (Changing where the metadata is rooted or something along the lines
would be such a change)
