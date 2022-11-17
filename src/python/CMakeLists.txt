#
# Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
# SPDX-License-Identifier: MIT.
#

# Define the target.
pybind11_add_module(pypakio
    "py-pak-io.cxx"
)

# Set the C++ standard version.
target_compile_features(pypakio PRIVATE cxx_std_20)

# Set warnings as errors.
target_compile_options(pypakio
    PRIVATE
        $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Werror -Wall>
        $<$<CXX_COMPILER_ID:MSVC>:/WX /W4 /Wv:18>
)

# Set the target's dependencies.
target_link_libraries(pypakio
    PRIVATE
        pak-io
)

# Installation.
install(TARGETS pypakio
    LIBRARY DESTINATION "bin"
    RUNTIME DESTINATION "bin"
)
