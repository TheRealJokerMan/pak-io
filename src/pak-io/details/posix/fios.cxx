//
// Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
// SPDX-License-Identifier: MIT.
//

#include "pak-io/details/fios.hxx"

#include <fcntl.h>
#include <unistd.h>

namespace pak::fios
{
    auto read_file(std::filesystem::path path, std::vector<std::byte>* output) -> std::error_code
    {
        auto fd{ open(path.string().c_str(), O_RDONLY) };

        auto result{ fd >= 0 };
        if (result)
        {
            auto const size{ std::filesystem::file_size(path) };
            if (size > 0)
            {
                output->resize(size);
                result = read(fd, output->data(), size) > 0;
            }

            close(fd);
        }
        else
        {
            return std::make_error_code(static_cast<std::errc>(errno));
        }

        return {};
    }

    auto write_file(std::vector<std::byte> const& input, std::filesystem::path path) -> std::error_code
    {
        auto fd{ open(path.string().c_str(), O_CREAT | O_WRONLY, 0644) };

        auto result{ fd >= 0 };
        if (result)
        {
            auto const size{ input.size() };
            if (size)
            {
                result = size == static_cast<uint64_t>(write(fd, input.data(), size));
            }

            close(fd);
        }
        else
        {
            return std::make_error_code(static_cast<std::errc>(errno));
        }

        return {};
    }

} // namespace pak::fios
