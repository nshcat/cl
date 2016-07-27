#######################################################################################
## libcl CMake Project File
##
## This simple project file makes it easy and clean to include libcl in CMake projects
## by either including the repo as git submodule or cloning it into the project tree.
## The resulting folder can be directly used as subproject and defines variables that
## define both include directories and libraries to link to.
##
## Usage:
## -------
##
## In main project file, before any subprojects that depend on libcl:
##    add_subdirectory(cl)
##
## In each subproject that uses libcl:
##    include_directories(${LIBCL_INCLUDE_DIRS})
##    target_link_libraries(target_name ${LIBCL_LIBRARIES})
##
##

# Version requirement and project info
cmake_minimum_required(VERSION 3.1.3)
project(cl)

# C++ Standard
## set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# Include pegtl library
add_subdirectory(pegtl)
include_directories(${PEGTL_INCLUDE_DIRS})

# Include ut library
add_subdirectory(ut)
include_directories(${LIBUT_INCLUDE_DIRS})

# Source files
file(GLOB SOURCE_FILES src/*.cxx)

# Define target as static library
add_library(cl ${SOURCE_FILES})

# Require support for at least C++14.
set_property(TARGET cl PROPERTY CXX_STANDARD 14)
set_property(TARGET cl PROPERTY CXX_STANDARD_REQUIRED ON)

# Set user variables
set(LIBCL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE INTERNAL "")
set(LIBCL_LIBRARIES cl CACHE INTERNAL "")

# Include own headers to allow own source files to easily include header files
include_directories(${LIBCL_INCLUDE_DIRS}/cl)
