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

 //workaround for missing ssize_t on windows
 #ifndef ssize_t
   #ifdef _WIN64
     #include <BaseTsd.h>
     typedef SSIZE_T ssize_t;
   #else
     //hdf5 defines ssize_t to be int on 32bit Win
     typedef int ssize_t;
   #endif
 #endif
#else
 #define NIXAPI
#endif

#ifdef _MSC_VER
#define NOEXCEPT
#define PACKED
#else
#define NOEXCEPT noexcept
#define PACKED __attribute__((packed))
#endif

#define NIX_SRC_FILE __FILE__
#define NIX_SRC_LINE __LINE__

#ifndef _MSC_VER
#define NIX_SRC_FUNC  __PRETTY_FUNCTION__
#else
#define NIX_SRC_FUNC __FUNCSIG__
#endif

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif
