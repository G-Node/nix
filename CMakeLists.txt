##########################################
# Pandora CMake

cmake_minimum_required (VERSION 2.6)
project (pandora CXX)

set(CMAKE_CXX_FLAGS "-std=c++11") ## Optimize
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -ggdb3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")


#########################################
# HDF-5
# FIND_PACKAGE (HDF5 NAMES hdf5)
find_package (HDF5 REQUIRED COMPONENTS C CXX)
include_directories (${HDF5_INCLUDE_DIR})
set (LINK_LIBS ${LINK_LIBS} ${HDF5_LIBRARIES} ${HDF5_CXX_LIBRARIES})


########################################
# Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.45.0 COMPONENTS date_time)

include_directories(${Boost_INCLUDE_DIRS})
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


include_directories(include)
file(GLOB pandora_SOURCES src/*.cpp)
file(GLOB pandora_INCLUDES include/*.hpp)

add_library(pandora SHARED ${pandora_INCLUDES} ${pandora_SOURCES})
target_link_libraries(pandora ${LINK_LIBS})

#IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
#  set(CMAKE_VERBOSE_MAKEFILE TRUE)
#endif()


########################################
# Tests

enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(CppUnit)
include_directories(${CPPUNIT_INCLUDE_DIRS})

file(GLOB Tests_SOURCES "test/Test*.cpp")

add_executable(TestRunner test/Runner.cpp ${Tests_SOURCES})
target_link_libraries(TestRunner ${CPPUNIT_LIBRARIES} pandora)

foreach(test ${Tests_SOURCES})
        get_filename_component(TestName ${test} NAME_WE)
	message(STATUS " Test added: ${TestName}")
        add_test(${TestName} TestRunner ${TestName})
endforeach(test)


########################################

MESSAGE(STATUS "READY. ")
MESSAGE(STATUS "===============================")
MESSAGE(STATUS "BOOST:   ${Boost_LIBRARIES}")
MESSAGE(STATUS "HDF5:    ${HDF5_LIBRARIES}")
MESSAGE(STATUS "CPPUNIT: ${CPPUNIT_LIBRARIES}")
MESSAGE(STATUS "===============================")
