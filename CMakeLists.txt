##########################################
# NIX CMake

cmake_minimum_required (VERSION 2.8.10...3.23.1)

# Defaults
SET(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Install dir prefix")
SET(LIB_INSTALL_DIR "lib" CACHE PATH "Library install directory")
SET(INCLUDE_INSTALL_DIR "include" CACHE PATH "Include install directory")

project (nix C CXX)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 5)
set(VERSION_PATCH 0)

set(VERSION_ABI   1)

set(VERSION_FULL ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

option(BUILD_STATIC "Build static version of the library" OFF)
option(BUILD_COVERAGE "Build with coverage information" OFF)

set(HAVE_COVERAGE OFF)

if(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} -std=c++11") ## Optimize
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wunreachable-code")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-local-typedef") # boost + clang

  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -ggdb3")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -DNDEBUG")

  if(CMAKE_COMPILER_IS_GNUCXX AND BUILD_COVERAGE)
    MESSAGE(STATUS "Activating coverage support.")
    set(HAVE_COVERAGE ON)

    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
      message(WARNING "Coverage requested but built type not 'Debug'")
    endif()

    add_compile_options(--coverage)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
  endif()

endif()

if(NOT WIN32)
  if(USE_NO_DEPRECATED_DECLARATIONS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
  endif()
endif()


set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

#########################################
# HDF-5
set(HDF5_USE_STATIC_LIBRARIES ${BUILD_STATIC})
find_package (HDF5 REQUIRED COMPONENTS C)
include_directories (${HDF5_INCLUDE_DIRS})
set (LINK_LIBS ${LINK_LIBS} ${HDF5_LIBRARIES})
add_definitions(-DH5_USE_110_API=1)

########################################
# Boost
if(WIN32)
  # On windows we always use the static version of boost
  set(Boost_USE_STATIC_LIBS ON)
  set(Boost_USE_STATIC_RUNTIME OFF)
else()
  set(Boost_USE_STATIC_LIBS ${BUILD_STATIC})
endif()

set(Boost_USE_MULTITHREADED ON)

find_package(Boost 1.49.0 REQUIRED date_time regex program_options system filesystem)

include_directories(${Boost_INCLUDE_DIR})
set (LINK_LIBS ${LINK_LIBS} ${Boost_LIBRARIES})

########################################
# Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)


########################################
# NIX

### AUTO-GENERATED VERSION HEADER
configure_file(
  "${PROJECT_SOURCE_DIR}/version.h.in"
  "${PROJECT_BINARY_DIR}/include/nix/nixversion.hpp")
include_directories("${PROJECT_BINARY_DIR}/include")

### scan files
include_directories(BEFORE include)
file(GLOB_RECURSE nix_SOURCES src/*.cpp)
file(GLOB_RECURSE nix_INCLUDES include/*.hpp)
list(APPEND nix_INCLUDES "${PROJECT_BINARY_DIR}/include/nix/nixversion.hpp")

### BACKENDS
set(backends "hdf5")

# This is for tests
include_directories(${CMAKE_SOURCE_DIR}/backend)

foreach(backend ${backends})
  file(GLOB_RECURSE backend_SOURCES "backend/${backend}/*.cpp")
  file(GLOB_RECURSE backend_INCLUDES "backend/${backend}/*.hpp")

  list(APPEND nix_SOURCES ${backend_SOURCES})
  list(APPEND nix_INCLUDES ${backend_INCLUDES})
endforeach()


### LIBRARY

if(BUILD_STATIC)
  set(NIX_LIBTYPE STATIC)
  add_definitions(-DNIX_STATIC=1)
else()
  set(NIX_LIBTYPE SHARED)
endif()

add_library(nixio ${NIX_LIBTYPE} ${nix_INCLUDES} ${nix_SOURCES})
target_link_libraries(nixio ${LINK_LIBS})
set_target_properties(nixio PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
set_target_properties(nixio PROPERTIES COMPILE_FLAGS "-fPIC")
set_target_properties(nixio PROPERTIES
                      VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
                      SOVERSION ${VERSION_ABI})

if(WIN32)
  set_target_properties(nixio PROPERTIES COMPILE_FLAGS -DNIXEXPORT)
endif()


if(NIX_FRAMEWORK)
  set_target_properties(nixio PROPERTIES
                        FRAMEWORK FALSE
                        FRAMEWORK_VERSION 1.0
                        PUBLIC_HEADER "${nix_INCLUDES}")
endif()


if(EXISTS "${CMAKE_SOURCE_DIR}/main.cpp")
  add_executable(playground EXCLUDE_FROM_ALL main.cpp)
  target_link_libraries(playground nixio)
endif()


########################################
# static analysis
find_program(CLANG_CHECK clang-check)
if(CLANG_CHECK)
  MESSAGE(STATUS "FOUND clang-check: ${CLANG_CHECK}")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #should maybe check for generator

  add_custom_target(check COMMAND ${CLANG_CHECK} -p ${CMAKE_BINARY_DIR} ${nix_SOURCES}
                    DEPENDS nixio
                    SOURCES ${nix_SOURCES})
endif()


########################################
# Tests

include(CTest)
enable_testing()

if(BUILD_TESTING)

  find_package(CppUnit)
  include_directories(${CPPUNIT_INCLUDE_DIR})
  include_directories(${CMAKE_SOURCE_DIR}/test)

  file(GLOB Tests_SOURCES "test/*Test*.[ch]pp")
  file(GLOB Real_Test_HEADERS "test/Test*.hpp")

  foreach(thebackend ${backends})
    set(thebackend_TEST_HEADERS)
    file(GLOB thebackend_TEST_HEADERS "test/${thebackend}/Test*.hpp")
    list(APPEND Real_Test_HEADERS ${thebackend_TEST_HEADERS})

    set(thebackend_TEST_SOURCES)
    file(GLOB thebackend_TEST_SOURCES "test/${thebackend}/*Test*.[ch]pp")
    list(APPEND Tests_SOURCES ${thebackend_TEST_SOURCES})
  endforeach()

  foreach(test ${Real_Test_HEADERS})
      get_filename_component(TestFileName ${test} NAME_WE)
      string(REPLACE "Test" "" TestName ${TestFileName})
      message(STATUS " Test added: ${TestName}")
      add_test(${TestName} TestRunner ${TestName})
  endforeach(test)

  add_executable(TestRunner test/Runner.cpp ${Tests_SOURCES})
  if(NOT WIN32)
    set_target_properties(TestRunner PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations")
  endif()
  target_link_libraries(TestRunner ${CPPUNIT_LIBRARIES} nixio)

endif()

add_executable(nix-bench EXCLUDE_FROM_ALL test/Benchmark.cpp)
target_link_libraries(nix-bench nixio)
if(NOT WIN32)
  set_target_properties(nix-bench PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations")
endif()


########################################
# Install

install(TARGETS nixio
        LIBRARY DESTINATION ${LIB_INSTALL_DIR}
        ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
        FRAMEWORK DESTINATION "/Library/Frameworks")
install(DIRECTORY include/ DESTINATION ${INCLUDE_INSTALL_DIR}/nixio-1.0)
install(DIRECTORY ${CMAKE_BINARY_DIR}/include/ DESTINATION ${INCLUDE_INSTALL_DIR}/nixio-1.0)

#pkg-config support
configure_file(${CMAKE_SOURCE_DIR}/nixio.pc.in ${CMAKE_BINARY_DIR}/nixio-1.0.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/nixio-1.0.pc DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)


set(GIT_REV_SHORT "unknown")

find_package(Git)
if(GIT_FOUND)
  message("git found: ${GIT_EXECUTABLE}")
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
    WORKING_DIRECTORY "${local_dir}"
    OUTPUT_VARIABLE GIT_REV_SHORT
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

string(TIMESTAMP TS_RPM "%a %b %d %Y")
string(TIMESTAMP TS_GIT "%Y%m%d")
set(RPM_RELTAG "${TS_GIT}git${GIT_REV_SHORT}")

configure_file(${CMAKE_SOURCE_DIR}/nixio.spec.in ${CMAKE_BINARY_DIR}/nixio.spec)

########################################
# CPack packaging
set(CPACK_PACKAGE_NAME "nixio")
set(CPACK_PACKAGE_VENDOR "g-node.org")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "nix - neuroscience information exchange")
set(CPACK_PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "nixio")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

set(CPACK_SOURCE_IGNORE_FILES
  "~$"
  "\\\\.gitignore$"
  "\\\\.gitmodules$"
  "^${PROJECT_SOURCE_DIR}/.git/"
  "^${PROJECT_SOURCE_DIR}/build/"
  )

set(CPACK_SOURCE_PACKAGE_FILE_NAME "nixio-${CPACK_PACKAGE_VERSION}"
    CACHE INTERNAL "tarball basename")
set(CPACK_SOURCE_GENERATOR ${CPACK_GENERATOR})

if(WIN32)
  install(TARGETS nixio
          ARCHIVE
          DESTINATION lib
          COMPONENT libraries)
  file(GLOB NIX_LICENSE "LICENSE*")
  install(FILES ${NIX_LICENSE}
          DESTINATION ./
          COMPONENT libraries)

  if(NOT BUILD_STATIC)
    file(GLOB HDF5_DLLS "${HDF5_DIR}/../../bin/*.dll")
    install(FILES ${HDF5_DLLS}
            DESTINATION bin
            COMPONENT libraries)
  endif()

  file(GLOB HDF5_COPYING "${HDF5_DIR}/../../COPYING")
  install(FILES ${HDF5_COPYING}
          DESTINATION ./
          COMPONENT libraries)

  set(CPACK_GENERATOR NSIS)
  set(CPACK_NSIS_MODIFY_PATH ON)
else()  # not WIN32
  if (NOT CPACK_GENERATOR)
    set(CPACK_GENERATOR TGZ)
  endif ()
endif()

INCLUDE(CPack)

########################################
get_directory_property(incdirs INCLUDE_DIRECTORIES)

MESSAGE(STATUS "READY. ")
MESSAGE(STATUS "===============================")
MESSAGE(STATUS "STATIC:   ${BUILD_STATIC}")
MESSAGE(STATUS "COVERAGE: ${HAVE_COVERAGE}")
MESSAGE(STATUS "===============================")
MESSAGE(STATUS "INCDIRS: ${incdirs}")
MESSAGE(STATUS "CFLAGS:  ${CMAKE_CXX_FLAGS}")
MESSAGE(STATUS "BOOST:   ${Boost_LIBRARIES}")
MESSAGE(STATUS "HDF5:    ${HDF5_LIBRARIES}")
MESSAGE(STATUS "CPPUNIT: ${CPPUNIT_LIBRARIES}")
MESSAGE(STATUS "===============================")
MESSAGE(STATUS "BACKENDS: ${backends}")
MESSAGE(STATUS "===============================")
MESSAGE(STATUS "INSTALL:")
MESSAGE(STATUS " PREFIX: ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS " INCLUDE DIR: ${INCLUDE_INSTALL_DIR}")
MESSAGE(STATUS " LIBRARY DIR: ${LIB_INSTALL_DIR}")
MESSAGE(STATUS "===============================")
