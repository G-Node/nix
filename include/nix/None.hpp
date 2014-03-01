// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#ifndef NIX_NONE_H
#define NIX_NONE_H

#include <boost/optional.hpp>

namespace nix {

typedef boost::none_t none_t;
const none_t none = boost::none;

}


#endif // NIX_NONE_H

