#
# Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
# SPDX-License-Identifier: MIT.
#

# Installation.
install(
    DIRECTORY
        ${CMAKE_CURRENT_SOURCE_DIR}
    DESTINATION
        "bin"
    PATTERN "CMakeLists.txt" EXCLUDE
)
