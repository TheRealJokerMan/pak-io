#
# Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
# SPDX-License-Identifier: MIT.
#

# Set the minimum CMake version.
cmake_minimum_required(VERSION 3.24.2 FATAL_ERROR)

# Set the minimum MacOS version.
set(CMAKE_OSX_DEPLOYMENT_TARGET "12" CACHE STRING "Minimum OS X deployment version")

# Define the project.
project(PakIO VERSION 0.0.1.0 DESCRIPTION "PAK I/O" HOMEPAGE_URL "https://jamiekenyon.io")
set(CMAKE_PACKAGE_VENDOR "Jamie Kenyon")

# Set project-wide defaults.
set(CMAKE_CXX_STANDARD 20)

# Include the dependent packages.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include("options")
include("packages")

# Add the code.
add_subdirectory("src")

# Add the documentation.
if(BUILD_DOCS)
    add_subdirectory("docs")
endif(BUILD_DOCS)

# Add the tests.
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory("tests")
endif(BUILD_TESTING)
