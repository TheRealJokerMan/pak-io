//
// Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
// SPDX-License-Identifier: MIT.
//

#include "fios.hxx"
#include "pod.hxx"
#include "writer.hxx"

#include <cstring>

namespace pak::writer
{
    Cursor::Cursor()
    {
        m_buffer = std::make_unique<std::vector<std::byte>>();
    }

    auto Cursor::data() const -> std::vector<std::byte> const&
    {
        return *m_buffer.get();
    }

    void Cursor::fixup(uint64_t offset, uint64_t value)
    {
        uint64_t* data{ reinterpret_cast<uint64_t*>(m_buffer->data() + offset) };
        std::memcpy(data, &value, sizeof(value));
    }

    auto Cursor::offset() const -> uint64_t
    {
        return m_buffer->size();
    }

    void Cursor::append(std::byte const* data, uint64_t size)
    {
        m_buffer->insert(m_buffer->end(), data, data + size);
    }

    Entry::Entry(std::filesystem::path source, std::filesystem::path base)
    {
        m_uncompressed_size = std::filesystem::file_size(source);
        m_path = std::filesystem::relative(source, base).string();
        std::replace(m_path.begin(), m_path.end(), '\\', '/');
    }

    void Entry::fixup(Cursor* cursor)
    {
        cursor->fixup(m_path_offset, cursor->offset());
        cursor->append(m_path.data(), m_path.size() + 1);

        cursor->fixup(m_data_offset, cursor->offset());
        cursor->append(m_contents.data(), m_contents.size());
    }

    auto Entry::load_contents(std::filesystem::path src) -> std::error_code
    {
        return fios::read_file(src, &m_contents);
    }

    void Entry::write(Cursor* cursor)
    {
        m_data_offset = offsetof(PAK_FILE_ENTRY, offset) + cursor->offset();
        m_path_offset = offsetof(PAK_FILE_ENTRY, path) + cursor->offset();

        PAK_FILE_ENTRY entry;
        entry.uncompressedSize = m_uncompressed_size;

        cursor->append(&entry, sizeof(entry));
    }

} // namespace pak::writer
