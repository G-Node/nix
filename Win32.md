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
     Expressions inside brackets "`< >`" are to be replaced (together with brackets) by a value: "`<A>`" => "`15`" if A was 15.
     If "Developer Command Prompt for VS2013" is missing in your installation follow these steps: https://stackoverflow.com/questions/21476588/where-is-developer-command-prompt-for-vs2013
   - **CMake**
     Needed for HDF5 and nix
	   Obtain from http://www.cmake.org and install the binary for windows
	   If "cmake" not added to PATH (command not found when typed in cmd window): reboot. If still missing follow these steps: https://www.java.com/en/download/help/path.xml

1) **HDF5**
   - Obtain sources from: http://www.hdfgroup.org/HDF5/release/obtainsrc.html
   - Create a build sub-folder (e.g. "build") in the HDF5 folder
   - Find a file named "release" in the hdf5 "/bin" folder and rename it to "_release"
   - From within the build folder execute:
     ```> cmake -C"../config/cmake/cacheinit.cmake" -G"Visual Studio 12 Win64" -DHDF5_ENABLE_SZIP_SUPPORT:BOOL=OFF -DHDF5_ENABLE_Z_LIB_SUPPORT:BOOL=OFF -DHDF5_BUILD_FORTRAN:BOOL=OFF ..```
   - From within the hdf5 folder execute:
       Build HDF5 via: `> cmake --build . --config Release`
   - Install Nullsoft installer version 2.x from: http://nsis.sourceforge.net - newer versions 3.x will _not_ work!
   - From within the hdf5 folder execute:
       Create a installer for HDF5 via: `> cpack -C Release CPackConfig.cmake`
   - Install HDF5 via the installer which now should have been created in the hdf5 folder named e.g. "HDF5-1.8.12-win64.exe". Set `HDF5_ROOT` to hdf5 directory (`> set HDF5_ROOT=<DIRECTORY>` and `> setx HDF5_ROOT <DIRECTORY> /m`)

2) **Boost**:
   - Obtain sources from: http://www.boost.org/users/download/
   - Boostrap with: `> bootstrap`
   - Build needed libraries via (and install to PREFIX):
     ```
      > .\b2 --toolset=msvc-12.0 threading=multi architecture=x86 address-model=64
      --prefix=<PREFIX> --libdir=<PREFIX>\lib --include-dir=<PREFIX>\include
      -j4 --build-type=complete --with-date_time --with-regex install

     ```
   - Set `BOOST_ROOT` to PREFIX (`> set BOOST_ROOT=<PREFIX>` and `> setx BOOST_ROOT <PREFIX> /m`)

3) **CppUnit**:
   - Obtain sources: http://sourceforge.net/apps/mediawiki/cppunit/
   - Use 7-zip to extract ".tar.gz" file: http://www.7-zip.org/
   - Open src/CppUnitLibraries.dsw in VS2013 (and agree to conversion, ignore error on creation of backup file)
   - Set build configratuion to Release via: BUILD->Configuration Manager
   - Mark all projects in solution (on the right), right click them, select properties and make sure "Configuration Properties -> C/C++ -> Code Generation -> Runtime Library" is set to "Multi-threaded DLL (/MD)"
   - If you get errors about "Multibyte Character Set" being used, try the following measures: 
       Download and install "Multibyte MFC Library for Visual Studio 2013" from: https://www.microsoft.com/en-us/download/details.aspx?id=40770
       If still getting errors (even after reboot): Mark all projects in solution (on the right), right click them, select properties and make sure "Configuration Properties -> General -> Character Set" is set to "Use Unicode Character set"

4) **Nix**:
   - Obtain sources from git (https://github.com/G-Node/pandora)
   - Create build folder (e.g. nix/build)
   - Run CMake: `> cmake .. -G"Visual Studio 12 Win64"`
   - Build via CMake: `> cmake --build . --config Release`
   - If all went well exectue the tests: `> ctest .`

