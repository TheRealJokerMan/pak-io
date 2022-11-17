#
# Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
# SPDX-License-Identifier: MIT.
#

# Define the target.
add_executable(pak-make
    "main.cxx"
)

# Set the C++ standard version.
target_compile_features(pak-make PRIVATE cxx_std_20)

# Set warnings as errors.
target_compile_options(pak-make
    PRIVATE
        $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Werror -Wall>
        $<$<CXX_COMPILER_ID:MSVC>:/WX /W4>
)

# Set the target's dependencies.
target_link_libraries(pak-make
    PRIVATE
        pak-io

        cxxopts::cxxopts
        fmt::fmt
        nlohmann_json::nlohmann_json
)

# Installation.
install(TARGETS pak-make DESTINATION "bin")
