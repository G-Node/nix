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



include_directories(include)
file(GLOB pandora_SOURCES src/*.cpp)
file(GLOB pandora_INCLUDES include/*.hpp)

add_library(pandora SHARED ${pandora_INCLUDES} ${pandora_SOURCES})
target_link_libraries(pandora ${LINK_LIBS})

#IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
#  set(CMAKE_VERBOSE_MAKEFILE TRUE)
#endif()

MESSAGE(STATUS "READY. ")
MESSAGE(STATUS "===============================")
MESSAGE(STATUS "H5: ${HDF5_LIBRARIES}")
MESSAGE(STATUS "===============================")
