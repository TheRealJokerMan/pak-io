#
# Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
# SPDX-License-Identifier: MIT.
#

# Define the target.
add_executable(pak-unmake
    "main.cxx"
)

# Set the C++ standard version.
target_compile_features(pak-unmake PRIVATE cxx_std_20)

# Set warnings as errors.
target_compile_options(pak-unmake
    PRIVATE
        $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Werror -Wall>
        $<$<CXX_COMPILER_ID:MSVC>:/WX /W4>
)

# Set the target's dependencies.
target_link_libraries(pak-unmake
    PRIVATE
        pak-io

        cxxopts::cxxopts
        fmt::fmt
)

# Installation.
install(TARGETS pak-unmake DESTINATION "bin")
