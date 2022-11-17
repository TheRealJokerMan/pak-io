//
// Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
// SPDX-License-Identifier: MIT.
//

#pragma once

#include <filesystem>
#include <system_error>
#include <vector>

namespace pak::fios
{
    /// Read the contents of a file into a container.
    /// \param path The full qualified path of the file to read.
    /// \param output A pointer to a container to store the contents of the file.
    /// \return An empty error code on success; a valid error code otherwise.
    auto read_file(std::filesystem::path path, std::vector<std::byte>* output) -> std::error_code;

    /// Write the contents of container to a file.
    /// \param input The content to be written to file.
    /// \param path The full qualified path of the file to write to.
    /// \return An empty error code on success; a valid error code otherwise.
    auto write_file(std::vector<std::byte> const& input, std::filesystem::path path) -> std::error_code;

} // namespace pak::fios
