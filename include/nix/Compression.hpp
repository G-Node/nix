// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
#ifndef I_COMPRESSION_H
#define I_COMPRESSION_H

namespace nix {

/**
 * @brief Data Compression modes
 */
enum class Compression {
    None = 0,
    DeflateNormal,
    Auto = DeflateNormal
};
}

#endif // NIX_COMPRESSION_H
