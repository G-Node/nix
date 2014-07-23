##########################################
# NIX CMake

cmake_minimum_required (VERSION 2.8.10)
project (nix CXX)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 9)
set(VERSION_PATCH 1)

set(VERSION_ABI   1)


if(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} -std=c++11") ## Optimize
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wunreachable-code")

  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -ggdb3")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")

  if(CMAKE_COMPILER_IS_GNUCXX)
    MESSAGE(STATUS "Activating coverage support.")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")
    SET(CMAKE_SHARED_LINKER_FLAGS="${CMAKE_SHARED_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
  endif()

endif()

if(NOT WIN32)
  if(USE_NO_DEPRECATED_DECLARATIONS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
  endif()
endif()

#########################################
# HDF-5
# FIND_PACKAGE (HDF5 NAMES hdf5)
find_package (HDF5 REQUIRED COMPONENTS C CXX)
include_directories (${HDF5_INCLUDE_DIR})
set (LINK_LIBS ${LINK_LIBS} ${HDF5_LIBRARIES} ${HDF5_CXX_LIBRARIES})


########################################
# Boost
if(WIN32)
  set(Boost_USE_STATIC_LIBS ON)
else()
  set(Boost_USE_STATIC_LIBS OFF)
endif()

set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.49.0 REQUIRED date_time regex program_options)

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

include_directories(include)
file(GLOB_RECURSE nix_SOURCES src/*.cpp)
file(GLOB_RECURSE nix_INCLUDES include/*.hpp)

add_library(nix SHARED ${nix_INCLUDES} ${nix_SOURCES})
target_link_libraries(nix ${LINK_LIBS})
set_target_properties(nix PROPERTIES
		      VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
		      SOVERSION ${VERSION_ABI})

if(WIN32)
  set_target_properties(nix PROPERTIES COMPILE_FLAGS -DNIXEXPORT)
  ADD_CUSTOM_COMMAND(TARGET nix POST_BUILD
	                 COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${HDF5_INCLUDE_DIR}/../bin/hdf5.dll"
	                         "$<TARGET_FILE_DIR:nix>"
	                 COMMENT "Copying HDF5 C DLL")
  ADD_CUSTOM_COMMAND(TARGET nix POST_BUILD
	                 COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${HDF5_INCLUDE_DIR}/../bin/hdf5_cpp.dll"
	                         "$<TARGET_FILE_DIR:nix>"
	                 COMMENT "Copying HDF5 CXX DLL")
endif()


if(NIX_FRAMEWORK)
  set_target_properties(nix PROPERTIES
                        FRAMEWORK FALSE
                        FRAMEWORK_VERSION 1.0
                        PUBLIC_HEADER "${nix_INCLUDES}")
endif()

#IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
#  set(CMAKE_VERBOSE_MAKEFILE TRUE)
#endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/main.cpp")
  add_executable(playground EXCLUDE_FROM_ALL main.cpp)
  target_link_libraries(playground nix)
endif()


######## static analysis
find_program(CLANG_CHECK clang-check)
if(CLANG_CHECK)
  MESSAGE(STATUS "FOUND clang-check: ${CLANG_CHECK}")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #should maybe check for generator

  add_custom_target(check COMMAND ${CLANG_CHECK} -p ${CMAKE_BINARY_DIR} ${nix_SOURCES}
                    DEPENDS nix
		    SOURCES ${nix_SOURCES})
endif()


########################################
# CLI

file(GLOB NixCli_SOURCES "cli/*.cpp")

add_executable(nixcli cli/Cli.cpp ${NixCli_SOURCES})
if(NOT WIN32)
  set_target_properties(nixcli PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations")
endif()
target_link_libraries(nixcli nix)
message(STATUS "CLI executable added")

########################################
# Tests

enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(CppUnit)
include_directories(${CPPUNIT_INCLUDE_DIR})

file(GLOB Tests_SOURCES "test/Test*.cpp")

add_executable(TestRunner test/Runner.cpp ${Tests_SOURCES})
if(NOT WIN32)
  set_target_properties(TestRunner PROPERTIES COMPILE_FLAGS "-Wno-deprecated-declarations")
endif()
target_link_libraries(TestRunner ${CPPUNIT_LIBRARIES} nix)

foreach(test ${Tests_SOURCES})
  get_filename_component(TestFileName ${test} NAME_WE)
  string(REPLACE "Test" "" TestName ${TestFileName})
  message(STATUS " Test added: ${TestName}")
  add_test(${TestName} TestRunner ${TestName})
endforeach(test)

include(CTest)


########################################
# Install

install(TARGETS nix
        LIBRARY DESTINATION lib
        FRAMEWORK DESTINATION "/Library/Frameworks"
        RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)


#pkg-config support
configure_file(${CMAKE_SOURCE_DIR}/nix.pc.in ${CMAKE_BINARY_DIR}/nix.pc)
install(FILES ${CMAKE_BINARY_DIR}/nix.pc DESTINATION lib/pkgconfig)


########################################

MESSAGE(STATUS "READY. ")
MESSAGE(STATUS "===============================")
MESSAGE(STATUS "CFLAGS:  ${CMAKE_CXX_FLAGS}")
MESSAGE(STATUS "BOOST:   ${Boost_LIBRARIES}")
MESSAGE(STATUS "HDF5:    ${HDF5_LIBRARIES}")
MESSAGE(STATUS "CPPUNIT: ${CPPUNIT_LIBRARIES}")
MESSAGE(STATUS "===============================")
