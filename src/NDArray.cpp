// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
// Copyright (c) 2013, Christian Kellner <kellner@bio.lmu.de>
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/NDArray.hpp>

namespace nix {


NDArray::NDArray(DataType dtype, NDSize dims) : dataType(dtype), extends(dims) {
    allocate_space();
}


void NDArray::allocate_space() {
    size_t type_size = data_type_to_size(dataType);
    dstore.resize(extends.nelms() * type_size);

    calc_strides();
}


void NDArray::resize(const NDSize &new_size) {
    extends = new_size;
    allocate_space();
}


void NDArray::calc_strides() {
    size_t _rank = rank();

    strides = NDSize(_rank, 1);

    for (size_t i = 1; i < _rank; i++) {
        size_t lst = _rank - i;
        size_t cur = lst - 1;
        strides[cur] = strides[lst] * extends[lst];
    }
}


size_t NDArray::sub2index(const NDSize &sub) const {
    size_t index = strides.dot(sub);
    return index;
}

} // namespace nix
