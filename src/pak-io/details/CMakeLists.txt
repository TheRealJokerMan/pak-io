#
# Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
# SPDX-License-Identifier: MIT.
#

# Configure source files.
configure_file("version.hxx.in" "generated/details/version.hxx")

# Define the target.
add_library(pak-io-details STATIC
    "writer.cxx"
)

# Set the C++ standard version.
target_compile_features(pak-io-details PRIVATE cxx_std_20)

# Set warnings as errors.
target_compile_options(pak-io-details
    PRIVATE
        $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Werror -Wall>
        $<$<CXX_COMPILER_ID:MSVC>:/WX /W4>
)

# Set the target's include directories.
target_include_directories(pak-io-details
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
        "${CMAKE_CURRENT_BINARY_DIR}/generated"
)

# Set the target's dependencies.
target_link_libraries(pak-io-details
    PRIVATE
        pak-io-details-platform
)

# Add the platform-specific sources.
add_subdirectory("posix")
add_subdirectory("win32")
