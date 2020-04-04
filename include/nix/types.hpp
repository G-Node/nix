// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_TYPES_H
#define NIX_TYPES_H

namespace nix {

// Can be used for forward declaration
class Block;
class Feature;
class File;
class DataArray;
class DataSet;
class DataView;
class DataFrameDimension;
class Dimension;
class MultiTag;
class NDArray;
class Property;
class RangeDimension;
class SampledDimension;
class Section;
class SetDimension;
class Source;
class Tag;
class Value;

//Ideally we would use unit64_t (and int64_t) here to directly specify
//the size we want, but for now we stick with how the hdf5 library
//defines hsize_t, otherwise we will run into issues when on plaforms
// where unit64_t is an incompatible type to the type of hsize_t
//(e.g. Ubuntu 12.04 LTS Server Edition 64 bit.)

typedef unsigned long long int ndsize_t;
typedef long long int          ndssize_t;

}

#endif
