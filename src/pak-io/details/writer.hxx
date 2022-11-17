//
// Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
// SPDX-License-Identifier: MIT.
//

#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

namespace pak::writer
{
    /// Represents a buffer and the current position in a buffer.
    class Cursor final
    {
    public:
        /// Constructor.
        Cursor();

        /// Append data into the buffer.
        /// \param data Address of the data.
        /// \param size The size of the data to append, in bytes.
        template <typename T>
        void append(T const* data, uint64_t size)
        {
            auto const ptr{ reinterpret_cast<std::byte const*>(data) };
            append(ptr, size);
        }

        /// Append data into the buffer.
        /// \param data Address of the data.
        /// \param size The size of the data to append, in bytes.
        void append(std::byte const* data, uint64_t size);

        /// Accesor.
        /// \return The data in the buffer.
        auto data() const -> std::vector<std::byte> const&;

        /// Fixup a pointer address in the buffer.
        /// \param offset The offset, from the beginning of the buffer, to write to.
        /// \param value The value to write at the offset.
        void fixup(uint64_t offset, uint64_t value);

        /// Accessor.
        /// \return The current length of the buffer.
        auto offset() const -> uint64_t;

    private:
        std::unique_ptr<std::vector<std::byte>> m_buffer;
    };

    /// An entry in an archive.
    class Entry final
    {
    public:
        /// Constructor.
        /// \param source The fully qualified path of the entry.
        /// \param base A fully qualified path that the entry is relative to.
        Entry(std::filesystem::path source, std::filesystem::path base);

        /// Fixup the entry.
        /// \param cursor The output cursor.
        void fixup(Cursor* cursor);

        /// Load the contents of a file.
        /// \param source The fully qualified path of a file to read.
        /// \return An empty error code on success; a valid error code otherwise.
        auto load_contents(std::filesystem::path source) -> std::error_code;

        /// Write this entry.
        /// \param cursor The output cursor.
        void write(Cursor* cursor);

    private:
        std::string m_path;

        std::vector<std::byte> m_contents;
        uint64_t m_uncompressed_size{};

        uint64_t m_data_offset{};
        uint64_t m_path_offset{};
    };

} // namespace pak::writer
