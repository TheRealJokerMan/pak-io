#
# Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
# SPDX-License-Identifier: MIT.
#

# Configure files.
string(TIMESTAMP PROJECT_CURRENT_YEAR "%Y")
configure_file("conf.py.in" "conf.py")

# Set Doxygen defaults.
set(DOXYGEN_GENERATE_HTML NO)
set(DOXYGEN_GENERATE_LATEX NO)
set(DOXYGEN_GENERATE_XML YES)
set(DOXYGEN_JAVADOC_AUTOBRIEF TRUE)
set(SPHINX_INDEX_FILE "${CMAKE_CURRENT_BINARY_DIR}/html/index.html")

# State which files require Doxygen processing.
get_target_property(PAKIO_SOURCE_DIR pak-io SOURCE_DIR)
set(DOXYGEN_SOURCES
    "${PAKIO_SOURCE_DIR}/error-code.hxx"
    "${PAKIO_SOURCE_DIR}/file-system.hxx"
)

# Define the Doxygen target.
doxygen_add_docs(doxygen ${DOXYGEN_SOURCES} USE_STAMP_FILE ALL COMMENT "Generating Doxygen documentation.")

if(PYTHON_BINDINGS)
    # The required RST files.
    set(RST_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/index.rst"
    )

    # Define the Sphinx target.
    add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
        COMMAND
            ${CMAKE_COMMAND} -E copy_if_different ${RST_FILES} ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND
            ${SPHINX_EXECUTABLE} -b html -c ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS
            ${RST_FILES}
            doxygen
        MAIN_DEPENDENCY
            "conf.py.in"
        COMMENT
            "Generating Sphinx documentation"
    )

    # Custom command to enable Sphinx build to be built as part of the build.
    add_custom_target(sphinx ALL DEPENDS ${SPHINX_INDEX_FILE})
endif(PYTHON_BINDINGS)
