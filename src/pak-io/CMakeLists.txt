#
# Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
# SPDX-License-Identifier: MIT.
#

# Define the target.
add_library(pak-io STATIC
    "error-code.cxx"
    "file-system.cxx"
)

# Set the public headers.
set_target_properties(pak-io
    PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        POSITION_INDEPENDENT_CODE ON
        PUBLIC_HEADER "error-code.hxx;file-system.hxx;details/fios.hxx;details/pod.hxx;details/version.hxx.in;details/writer.hxx"
        VISIBILITY_INLINES_HIDDEN ON
)

# Set the C++ standard version.
target_compile_features(pak-io PRIVATE cxx_std_20)

# Set warnings as errors.
target_compile_options(pak-io
    PRIVATE
        $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Werror -Wall>
        $<$<CXX_COMPILER_ID:MSVC>:/WX /W4>
)

# Set the target's include directories.
target_include_directories(pak-io
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

# Set the target's dependencies.
target_link_libraries(pak-io
    PRIVATE
        pak-io-details
)

# Add the private sources.
add_subdirectory("details")

# Installation.
install(TARGETS pak-io
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION "include/pak-io"
)
