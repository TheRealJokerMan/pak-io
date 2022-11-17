//
// Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
// SPDX-License-Identifier: MIT.
//

#include "pak-io/details/fios.hxx"

#include <errhandlingapi.h>
#include <fileapi.h>
#include <handleapi.h>

namespace pak::fios
{
    auto make_error_code(unsigned long code) -> std::error_code
    {
        return std::error_code(static_cast<int32_t>(code), std::system_category());
    }

    auto read_file(std::filesystem::path path, std::vector<std::byte>* output) -> std::error_code
    {
        auto handle{ CreateFileA(path.string().c_str(), // name of the file to read.
                                 GENERIC_READ,          // open for reading.
                                 FILE_SHARE_READ,       // share for reading.
                                 nullptr,               // default security.
                                 OPEN_EXISTING,         // open an existing file.
                                 FILE_ATTRIBUTE_NORMAL, // normal file.
                                 nullptr) };

        auto result{ handle != INVALID_HANDLE_VALUE };

        if (result)
        {
            if (auto const size{ static_cast<unsigned long>(std::filesystem::file_size(path)) }; size > 0)
            {
                output->resize(size);

                DWORD bytes_read{ 0 };
                result = ReadFile(handle, output->data(), size, &bytes_read, nullptr) ? true : false;
                if (!result)
                {
                    return make_error_code(GetLastError());
                }
            }

            CloseHandle(handle);
        }
        else
        {
            return make_error_code(GetLastError());
        }

        return {};
    }

    auto write_file(std::vector<std::byte> const& input, std::filesystem::path path) -> std::error_code
    {
        auto handle{ CreateFileA(path.string().c_str(), // name of the file to write.
                                 GENERIC_WRITE,         // open for writing.
                                 0,                     // no sharing.
                                 nullptr,               // default security.
                                 CREATE_NEW,            // create new file only.
                                 FILE_ATTRIBUTE_NORMAL, // normal file.
                                 nullptr) };

        auto result{ handle != INVALID_HANDLE_VALUE };
        if (result)
        {
            if (auto const size{ static_cast<unsigned long>(input.size()) }; size > 0)
            {
                DWORD bytes_written;
                result = WriteFile(handle, input.data(), size, &bytes_written, nullptr) ? true : false;
                if (result)
                {
                    result = bytes_written == size;
                }
                else
                {
                    return make_error_code(GetLastError());
                }
            }

            CloseHandle(handle);
        }
        else
        {
            return make_error_code(GetLastError());
        }

        return {};
    }

} // namespace pak::fios
