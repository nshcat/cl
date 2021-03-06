#######################################################################################
## libcl CMake Project File
##
## This simple project file makes it easy and clean to include libcl in CMake projects
## by either including the repo as git submodule or cloning it into the project tree.
## The resulting folder can be directly used as subproject.
##
## Usage:
## -------
##
## In main project file, before any subprojects that depend on libcl:
##    add_subdirectory(cl)
##
## In each subproject that uses libcl:
##    target_link_libraries(target_name ${LIBCL_LIBRARIES})
##
##

# Version requirement and project info
cmake_minimum_required(VERSION 3.1.3)
project(cl)

# Include pegtl library
add_subdirectory(PEGTL)

# Include ut library
add_subdirectory(ut)

# Source files
file(GLOB SOURCE_FILES src/*.cxx)

# Define target as static library
add_library(cl ${SOURCE_FILES})

# Link to libut
target_link_libraries(cl ${LIBUT_LIBRARIES} ${PEGTL_LIBRARIES})

# Add top include directory as public dependency.
# This automatically adds them to parent projects when linking to libcl.
target_include_directories(cl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
	
# Add cl subfolder as private dependency.
# It will thus not be available to parent projects.
target_include_directories(cl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/cl)

# Require support for at least C++14.
set_property(TARGET cl PROPERTY CXX_STANDARD 14)
set_property(TARGET cl PROPERTY CXX_STANDARD_REQUIRED ON)

# Set user variables
set(LIBCL_LIBRARIES cl CACHE INTERNAL "")