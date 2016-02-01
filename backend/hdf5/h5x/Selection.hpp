// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_SELECTION_H
#define NIX_SELECTION_H

#include <nix/NDSize.hpp>
#include <nix/Hydra.hpp>
#include "DataSpace.hpp"

#include <nix/Platform.hpp>

namespace nix {
namespace hdf5 {


class NIXAPI Selection {

public:

    enum class Preset {
        None = 0,
        All  = 1
    };

    enum class Mode {
        Set  = H5S_SELECT_SET,
        Or   = H5S_SELECT_OR,
        And  = H5S_SELECT_AND,
        Xor  = H5S_SELECT_XOR,
        NotA = H5S_SELECT_NOTA,
        NotB = H5S_SELECT_NOTB
    };

    Selection() : space() {}
    Selection(const DataSpace &ds) : space(ds) {}
    Selection(const Selection &sel) : space(sel.space) {}
    template<typename T> Selection (const T &value);

    Selection& operator=(const Selection &other) { space = other.space; return *this; }

    void select(const NDSize &count, const NDSize &start, Mode mode = Mode::Set);
    void offset(const NDSSize &offset);

    DataSpace& h5space() { return space; }
    const DataSpace& h5space() const { return space; }
    bool isValid() const;
    void bounds(NDSize &start, NDSize &end) const;
    NDSize size() const;
    size_t rank() const;

private:
    DataSpace space;
};

template<typename T>
Selection::Selection (const T &value)
{
    const Hydra<const T> hydra(value);
    NDSize size = hydra.shape();
    space = DataSpace::create(size, true);
}


} // namespace hdf5
} // namespace nix

#endif //NIX_SELECTION_H
