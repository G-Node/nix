How to compile nix on Win32
===========================

NB: Currently only Relase type builds on Windows x64 have been successfully tested.

Dependencies
------------

0) Required build tools
   - **Visual Studio 2013 Express**
     Download link: http://www.visualstudio.com/en-us/downloads/download-visual-studio-vs.aspx
     The required edition is "Visual Studio Express 2013 for Windows Desktop" (the download is free, but registration is needed)
     From now on exectue commands (marked with "`>`") from the "Developer Command Prompt for VS2013" found under the Visual Studio Tools submenu in the Visual Studio 2013 start menu.
   - **CMake**
     Needed for HDF5 and nix
	   Obtain from http://www.cmake.org and install the binary for windows

1) **HDF5**
   - Obtain sources from: http://www.hdfgroup.org/HDF5/release/obtainsrc.html
   - Create a build folder (e.g. build)
   - From within the build folder execute:
     `> cmake -C ../config/cmake/cacheinit.cmake -G"Visual Studio 12 Win64" -DHDF5_ENABLE_SZIP_SUPPORT:BOOL=OFF -DHDF5_ENABLE_Z_LIB_SUPPORT:BOOL=OFF  -DHDF5_BUILD_FORTRAN:BOOL=OFF ..`
   - Build HDF5 via: `> cmake --build . --config Release`
   - Install Nullsoft installer from: http://nsis.sourceforge.net
   - Create a installer for HDF5 via `> cpack -C Release CPackConfig.cmake`
   - Install HDF5 via the installer and set `HDF5_ROOT` to it (`> set HDF5_ROOT=<DIRECTORY>`)

2) **Boost**:
   - Obtain sources from: http://www.boost.org/users/download/
   - Boostrap with: `> bootstrap`
   - Build needed libraries via (and install to PREFIX):
     ```
      > .\b2 --toolset=msvc-12.0 threading=multi architecture=x86 address-model=64
      --prefix=PREFIX --libdir=PREFIX\lib --include-dir=E:\PREFIX\include
      -j4 --build-type=complete --with-date_time --with-regex install

     ```
   - Set `BOOST_ROOT` to PREFIX (`> set BOOST_ROOT=PREFIX`)

3) **CppUnit**:
   - Obtain sources: http://sourceforge.net/apps/mediawiki/cppunit/
   - Open src/CppUnitLibraries.dsw in VS2013 (and agree to conversion)
   - Set build configratuion to Release (make sure Configuration Properties -> C/C++ -> Code Generation -> Runtime Library is set to "Multi-threaded DLL (/MD)"

4) **Nix**:
   - Obtain sources from git (https://github.com/G-Node/pandora)
   - Create build folder (e.g. nix/build)
   - Run CMake: `> cmake .. -G"Visual Studio 12 Win64"`
   - Build via CMake: `> cmake --build . --config Release`
   - If all went well exectue the tests: `> ctest .`

