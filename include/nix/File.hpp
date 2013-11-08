// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_FILE_H
#define NIX_FILE_H

#include <base/ImplContainer.hpp>
#include <nix/IFile.hpp>

namespace nix {

class File : public IFile, public base::ImplContainer<IFile> {

};

}

#endif
