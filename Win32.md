How to compile nix on Windows
=============================

The following guide shows build options for both, 32- and 64-bit. They are marked with :three::two: and :six::four: in the text.

Dependencies
------------

1. **Required build tools**
  - ***Visual Studio 2013 Express***
    - Download link: http://www.visualstudio.com/en-us/downloads/download-visual-studio-vs.aspx
    - The required edition is "Visual Studio Express 2013 for Windows Desktop" (the download is free, but registration is needed)
    - From now on exectue commands (marked with "`>`") from the "Developer Command Prompt for VS2013" found as explained here: http://msdn.microsoft.com/en-us/library/ms229859.aspx
    - Expressions inside brackets "`< >`" are to be replaced (together with brackets) by a value: "`<A>`" => "`15`" if A was 15.
    <br><br>Extra stuff<br>
    - If you want a "Developer Command Prompt" inside the "Visual Studio->Tools" menu (oldschool way) do the following:
      - Open "Tools->External Tools" and click "Add"
      - Make :three::two:`Title` = `Command Prompt 32`, `Command` = `C:\Windows\System32\cmd.exe`, `Arguments` = `/K "<VSPATH>\Common7\Tools\vsvars32.bat"` or :six::four:`Title` = `Command Prompt 64`, `Command` = `C:\Windows\SysWOW64\cmd.exe`, `Arguments` = `/K "<VSPATH>\Common7\Tools\vsvars64.bat"`

  - ***CMake***
    - Obtain from http://www.cmake.org and install the binary for windows
    - If "cmake" not added to PATH (command not found when typed in cmd window): reboot. If still missing follow these steps: https://www.java.com/en/download/help/path.xml

2. **HDF5**
  - Obtain sources (>= 1.8.13) from: http://www.hdfgroup.org/HDF5/release/obtainsrc.html
  - Create a build sub-folder (e.g. `build`) in the HDF5 folder
  - From within the build folder execute:<br>
  :three::two:
    ```> cmake -C"../config/cmake/cacheinit.cmake" -G"Visual Studio 12" -DHDF5_ENABLE_SZIP_SUPPORT:BOOL=OFF -DHDF5_ENABLE_Z_LIB_SUPPORT:BOOL=OFF -DHDF5_BUILD_FORTRAN:BOOL=OFF -DBUILD_SHARED_LIBS:BOOL=OFF ..```<br>
  :six::four:
    ```> cmake -C"../config/cmake/cacheinit.cmake" -G"Visual Studio 12 Win64" -DHDF5_ENABLE_SZIP_SUPPORT:BOOL=OFF -DHDF5_ENABLE_Z_LIB_SUPPORT:BOOL=OFF -DHDF5_BUILD_FORTRAN:BOOL=OFF -DBUILD_SHARED_LIBS:BOOL=OFF ..```
  - From within the hdf5\build folder execute:
    Build HDF5 via: `> cmake --build . --config Release`
  - Install Nullsoft installer version 2.x from: http://nsis.sourceforge.net - newer versions 3.x will _not_ work!
  - From within the hdf5 folder execute:
    Create a installer for HDF5 via: `> cpack -C Release CPackConfig.cmake`
  - Install HDF5 via the installer which now should have been created in the hdf5 folder named e.g. :three::two:`HDF5-1.8.13-win32.exe` or :six::four:`HDF5-1.8.13-win64.exe`. Install inside `C:\opt\hdf5`. (Or any folder you like, but then you are on your own regarding paths & environment vars)
  <br><br>Troubleshooting - If you get errors try this<br>
  - Find a file named "release" in the hdf5 "/bin" folder and rename it to "_release"

3. **Boost**:
  - Obtain sources from: http://www.boost.org/users/download/
  - Boostrap with: `> bootstrap`
  - Build needed libraries via (and install to PREFIX, change "address-model=64" to ="32" for 32-bit build):
    ```
     > .\b2 --toolset=msvc-12.0 threading=multi architecture=x86 address-model=64
     --prefix=<PREFIX> --libdir=<PREFIX>\lib --include-dir=<PREFIX>\include
     -j4 --build-type=complete --with-date_time --with-regex --with-filesystem 
     --with-program_options --with-system install
    ```
  - Set `BOOST_ROOT` to PREFIX (`> set BOOST_ROOT=<PREFIX>` and `> setx BOOST_ROOT <PREFIX> /m`) and add BOOST root & include dirs to PATH, e.g.: "C:\Users\B\opt\boost", "C:\Users\B\opt\boost\include\boost-1_55\boost"

4. **CppUnit**:
  - Obtain sources: http://sourceforge.net/apps/mediawiki/cppunit/
  - Use 7-zip to extract ".tar.gz" file: http://www.7-zip.org/
  - Open src/CppUnitLibraries.dsw in VS2013 (and agree to conversion, ignore error on creation of backup file)
  - Set build configratuion to Release via: BUILD->Configuration Manager
  - Mark all projects in solution (on the right), right click them, select properties and make sure "Configuration Properties -> C/C++ -> Code Generation -> Runtime Library" is set to "Multi-threaded DLL (/MD)"
  - If you are building 64-bit: use Configuration Manager and set Platform to "x64" for all entries
  - If you are building 64-bit: right click each item in the solution and select "Properties"->"Configuration Properties"->"Librarian" or "Linker"->"General"->"Output File" and change it by adding the prefix "$(SolutionDir)$(Platform)". E.g. "Release\cppunit.lib" should change to "$(SolutionDir)$(Platform)\Release\cppunit.lib". Do this for all items in solution.
  - Now try to build the solution at least once: ignore the errors you get, we just need a part to build which should work!
  - Add cppunit include dir to PATH, e.g.: "C:\Users\B\opt\cppunit\include"
  <br><br>The following point can most likely be ignored unless you can't get the necessary parts of cppunit to build:<br>
  - If you get errors about "Multibyte Character Set" being used, try the following measures: 
    - Download and install "Multibyte MFC Library for Visual Studio 2013" from: https://www.microsoft.com/en-us/download/details.aspx?id=40770
    - If still getting errors (even after reboot): Mark all projects in solution (on the right), right click them, select properties and make sure "Configuration Properties -> General -> Character Set" is set to "Use Unicode Character set"

5. **Nix**:
  - Obtain sources from git (https://github.com/G-Node/nix)
  - Create build folder (e.g. nix/build)
  - Run CMake: `> cmake .. -G"Visual Studio 12 Win64"`
  - Open "nix.sln" with Visual Studio, go to "Configuration Manager" and set config to "Release" and build the solution. Or build via CMake: `> cmake --build . --config Release`
  - If all went well exectue the tests: `> ctest .` and `Release\TestRunner.exe`

6. **Troubleshooting**:

  - If you get "'cl' is not recognized as internal or external command..." make sure VC++ bin dir is added to PATH
  - If you get include file(s) not found errors look for "vcvars*.bat" file, e.g. "vcvars32.bat" and execute it once
  - If you have problems compiling boost for BOOST_ROOT not being found or "access denied" errors, create a batch file with

   ```
     set BOOST_ROOT=C:\Users\B\opt\boost\
     C:\Users\B\opt\boost\b2 --toolset=msvc-12.0 threading=multi architecture=ia64 address-model=64 --prefix=C:\Users\B\opt\boost --libdir=<C:\Users\B\opt\boost\lib --include-dir=C:\Users\B\opt\boost\include -j4 --build-type=complete --with-date_time --with-regex install
   ```

  and execute it with "runas" command.

