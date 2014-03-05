// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#ifdef _WIN32
 #ifdef NIXEXPORT
 #define NIXAPI __declspec(dllexport)
 #else
 #define NIXAPI __declspec(dllimport)
 #endif
#pragma warning(disable: 4250 4251)
#else
 #define NIXAPI
#endif
