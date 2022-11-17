#
# Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
# SPDX-License-Identifier: MIT.
#

# Include the required modules.
include("FetchContent")

# Doxygen / Sphinx.
if(BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    if(PYTHON_BINDINGS)
        find_program(SPHINX_EXECUTABLE NAMES sphinx-build DOC "Path to sphinx-build executable" REQUIRED)
    endif(PYTHON_BINDINGS)
endif(BUILD_DOCS)

# Format (as <format> is still missing from libc++14).
if(NOT TARGET fmt::fmt)
    FetchContent_Declare(Format GIT_REPOSITORY "https://github.com/fmtlib/fmt.git" GIT_TAG "11.0.2")
    FetchContent_MakeAvailable(Format)
endif(NOT TARGET fmt::fmt)

# JSON support.
if(NOT TARGET nlohmann_json::nlohmann_json)
    FetchContent_Declare(jsonlib GIT_REPOSITORY "https://github.com/nlohmann/json.git" GIT_TAG "v3.11.3")
    FetchContent_MakeAvailable(jsonlib)
endif(NOT TARGET nlohmann_json::nlohmann_json)

# Program Options.
if(NOT TARGET cxxopts::cxxopts)
    set(CXXOPTS_BUILD_EXAMPLES  OFF)
    set(CXXOPTS_BUILD_TESTS     OFF)
    set(CXXOPTS_ENABLE_INSTALL  OFF)
    set(CXXOPTS_ENABLE_WARNINGS OFF)

    FetchContent_Declare(cxxopts GIT_REPOSITORY "https://github.com/jarro2783/cxxopts.git" GIT_TAG "v3.2.1")
    FetchContent_MakeAvailable(cxxopts)
endif(NOT TARGET cxxopts::cxxopts)

# Python bindings.
if(PYTHON_BINDINGS AND NOT TARGET pybind11::embed)
    find_package(Python COMPONENTS Interpreter Development REQUIRED)

    FetchContent_Declare(pybind GIT_REPOSITORY "https://github.com/pybind/pybind11.git" GIT_TAG "v2.13.6")
    FetchContent_MakeAvailable(pybind)
endif(PYTHON_BINDINGS AND NOT TARGET pybind11::embed)

# Unit-testing.
if(NOT TARGET Catch2::Catch2WithMain)
    FetchContent_Declare(Catch GIT_REPOSITORY "https://github.com/catchorg/Catch2.git" GIT_TAG "v3.7.1")
    FetchContent_MakeAvailable(Catch)
    list(APPEND CMAKE_MODULE_PATH "${catch_SOURCE_DIR}/extras")
endif(NOT TARGET Catch2::Catch2WithMain)
