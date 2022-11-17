#
# Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
# SPDX-License-Identifier: MIT.
#

if(NOT WIN32)
    return()
endif(NOT WIN32)

# Define the target.
add_library(pak-io-details-platform STATIC
    "fios.cxx"
)

# Set the C++ standard version.
target_compile_features(pak-io-details-platform PRIVATE cxx_std_20)

# Set the compile definitions.
target_compile_definitions(pak-io-details-platform
    PRIVATE
        _AMD64_
)

# Set warnings as errors.
target_compile_options(pak-io-details-platform
    PRIVATE
        /WX
        /W4
)

# Set the target's include directories.
target_include_directories(pak-io-details-platform
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../../.."
)
