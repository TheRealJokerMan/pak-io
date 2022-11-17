#
# Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
# SPDX-License-Identifier: MIT.
#

# Add the library source.
add_subdirectory("pak-io")

# Add the command-line tools.
if(BUILD_CLI_TOOLS)
    add_subdirectory("pak-make")
    add_subdirectory("pak-unmake")
endif(BUILD_CLI_TOOLS)

# Add the GUI tools.
if(BUILD_GUI_TOOLS)
    add_subdirectory("pak-viewer")
endif(BUILD_GUI_TOOLS)

# Add th Python bindings.
if(PYTHON_BINDINGS)
    add_subdirectory("python")
endif(PYTHON_BINDINGS)
