#
# Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
# SPDX-License-Identifier: MIT.
#

if(NOT UNIX)
    return()
endif(NOT UNIX)

# Define the target.
add_library(pak-io-details-platform STATIC
    "fios.cxx"
)

# Set the C++ standard version.
target_compile_features(pak-io-details-platform PRIVATE cxx_std_20)

# Set warnings as errors.
target_compile_options(pak-io-details-platform
    PRIVATE
        -Werror
        -Wall
)

# Set the target's include directories.
target_include_directories(pak-io-details-platform
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../../.."
)
