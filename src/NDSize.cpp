// Copyright © 2015 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4996)
#include <algorithm>
#endif


#include <nix/NDSize.hpp>

namespace nix {

#ifdef _MSC_VER

void nd_fill(ndsize_t *data, size_t len, ndsize_t value) {
    std::fill_n(data, len, value);
}
void nd_fill(ndssize_t *data, size_t len, ndssize_t value) {
    std::fill_n(data, len, value);
}

void nd_copy(ndsize_t *source, size_t n, ndsize_t *dest) {
    //if n == 0, then source or dest might be nullptrs
    // which triggers an assertion on windows
	if (n == 0) {
		return;
	}
    std::copy_n(source, n, dest);
}

void nd_copy(ndssize_t *source, size_t n, ndsize_t *dest) {
    //see above for why we do this
	if (n == 0) {
		return;
	}

    std::copy_n(source, n, dest);
}

#pragma warning(pop)
#endif

}