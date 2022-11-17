#
# Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
# SPDX-License-Identifier: MIT.
#

include("CTest")

# Test the tools themselves.
add_test(NAME make-tool   COMMAND $<TARGET_FILE:pak-make>   "--file" "${CMAKE_CURRENT_SOURCE_DIR}/data/sample.json" "--directory" "${CMAKE_BINARY_DIR}/test_data")
add_test(NAME unmake-tool COMMAND $<TARGET_FILE:pak-unmake> "--file" "${CMAKE_BINARY_DIR}/test_data/sample.pak"     "--directory" "${CMAKE_BINARY_DIR}/test_data/extracted")
set_tests_properties(unmake-tool PROPERTIES DEPENDS make-tool)

# Define the target.
add_executable(pak-io-tests
    "file-system.cxx"
)

# Set the C++ standard version.
target_compile_features(pak-io-tests PRIVATE cxx_std_20)

# Set warnings as errors.
target_compile_options(pak-io-tests
    PRIVATE
        $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Werror -Wall>
        $<$<CXX_COMPILER_ID:MSVC>:/WX /W4>
)

# Set the target's dependencies.
target_link_libraries(pak-io-tests
    PRIVATE
        Catch2::Catch2WithMain
        pak-io
)

# Discover the tests.
add_test(NAME test-pak-io COMMAND $<TARGET_FILE:pak-io-tests>)
set_tests_properties(test-pak-io
    PROPERTIES
        ENVIRONMENT "ARCHIVE_PATH=${CMAKE_BINARY_DIR}/test_data/sample.pak"
    DEPENDS
        make-tool
)

# Add the Python bindings tests.
if(PYTHON_BINDINGS)
    add_test(NAME test-python-bindings COMMAND ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/bindings.py")
    set_tests_properties(test-python-bindings
        PROPERTIES
            ENVIRONMENT "ARCHIVE_PATH=${CMAKE_BINARY_DIR}/test_data/sample.pak;PYTHONPATH=$<TARGET_FILE_DIR:pypakio>"
        DEPENDS
            make-tool
    )
endif(PYTHON_BINDINGS)

# Add the GUI tools tests.
if(BUILD_GUI_TOOLS)
    add_test(NAME test-qml COMMAND "qmllint" "${CMAKE_SOURCE_DIR}/src/pak-viewer/qml/main.qml")
endif(BUILD_GUI_TOOLS)
