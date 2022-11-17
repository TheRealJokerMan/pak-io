//
// Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
// SPDX-License-Identifier: MIT.
//

#include "details/fios.hxx"
#include "details/pod.hxx"
#include "details/version.hxx"
#include "details/writer.hxx"
#include "file-system.hxx"

#include <cstring>

namespace pak
{
    Entry::Entry(std::string const& path, uint8_t const* data, uint64_t size) : m_data_value(data), m_data_size(size), m_path(path)
    {
    }

    void Entry::contents(uint8_t const** data, uint64_t* size) const
    {
        *data = m_data_value;
        *size = m_data_size;
    }

    auto Entry::operator==(std::string const& path) const -> bool
    {
        return m_path == path;
    }

    auto Archive::create(std::string const& name,
                         std::filesystem::path const& out_dir,
                         std::filesystem::path const& rel_dir,
                         std::vector<std::filesystem::path> const& sources) -> std::error_code
    {
        // Check that the source file exist.
        for (auto const& source : sources)
        {
            if (!std::filesystem::exists(source))
            {
                return make_error_code(ErrorCode::SourceDoesNotExist);
            }
        }

        // Derive the fully qualified path of the destination file.
        auto const destination{ std::filesystem::weakly_canonical(std::filesystem::path(out_dir).append(name).replace_extension(".pak")) };
        if (std::filesystem::exists(destination))
        {
            return make_error_code(ErrorCode::TargetArchiveExists);
        }
        else
        {
            auto directories{ std::filesystem::path(destination).remove_filename() };
            std::filesystem::create_directories(directories);
        }

        // Load the sources.
        std::vector<std::unique_ptr<writer::Entry>> entries;
        for (auto const& path : sources)
        {
            auto entry{ std::make_unique<writer::Entry>(path, rel_dir) };
            auto result{ entry->load_contents(path) };

            if (result)
            {
                return result;
            }
            else
            {
                entries.emplace_back(std::move(entry));
            }
        }

        auto cursor{ std::make_unique<writer::Cursor>() };
        uint64_t const name_offset = offsetof(PAK_FILE_HEADER, name) + cursor->offset();

        // Write the header.
        PAK_FILE_HEADER header;
        header.numEntries = entries.size();
        header.version = make_version(Version::major(), Version::minor(), Version::patch(), Version::tweak());
        cursor->append(&header, sizeof(header));

        // Write the entries.
        for (auto const& entry : entries)
        {
            entry->write(cursor.get());
        }

        // Fixup the header.
        cursor->fixup(name_offset, cursor->offset());
        cursor->append(name.data(), sizeof(name.size() + 1));

        // Fixup the entries.
        for (auto const& entry : entries)
        {
            entry->fixup(cursor.get());
        }

        // Write the file.
        return fios::write_file(cursor->data(), destination);
    }

    auto Archive::extract(std::filesystem::path const& path, std::filesystem::path const& /*directory*/) -> std::error_code
    {
        auto file{ std::make_unique<Archive>() };
        return file->mount(path);
    }

    auto Archive::mount(std::filesystem::path const& path) -> std::error_code
    {
        // Test that the file exists.
        if (!std::filesystem::exists(path))
        {
            return make_error_code(ErrorCode::ArchiveDoesNotExist);
        }

        m_contents = std::make_unique<std::vector<std::byte>>();
        auto result{ fios::read_file(path, m_contents.get()) };
        if (result)
        {
            return result;
        }

        // Test the archive header.
        auto const* header{ reinterpret_cast<PAK_FILE_HEADER const*>(m_contents->data()) };
        if (std::memcmp(header->magic, PAK_FILE_HEADER_MAGIC, sizeof(PAK_FILE_HEADER::magic)))
        {
            return make_error_code(ErrorCode::ArchiveInvalidHeader);
        }

        // Test the archive version.
        if (!match_version(header->version))
        {
            return make_error_code(ErrorCode::ArchiveWrongVersion);
        }

        // Test the archive header name.
        if (!header->name)
        {
            return make_error_code(ErrorCode::ArchiveNameMissing);
        }

        // Fixup and read the name of the archive.
        m_name = reinterpret_cast<char const*>(reinterpret_cast<uint64_t>(header->name) + reinterpret_cast<uint64_t>(m_contents->data()));

        auto const* file_entry{ reinterpret_cast<PAK_FILE_ENTRY const*>(m_contents->data() + sizeof(*header)) };
        for (uint64_t index = 0; index < header->numEntries; ++index, ++file_entry)
        {
            if (!file_entry->path)
            {
                return make_error_code(ErrorCode::EntryPathMissing);
            }

            // clang-format off
            std::string file_path { reinterpret_cast<char const*>(reinterpret_cast<uint64_t>(file_entry->path) + reinterpret_cast<uint64_t>(m_contents->data()))};
            // clang-format on

            if (!file_entry->offset)
            {
                return make_error_code(ErrorCode::EntryDataMissing);
            }

            auto const* data{ reinterpret_cast<uint8_t const*>(file_entry->offset + reinterpret_cast<uint64_t>(m_contents->data())) };

            m_entries.emplace_back(std::make_unique<Entry>(file_path, data, file_entry->uncompressedSize));
        }

        return {};
    }

    auto FileSystem::count_entries(std::filesystem::path& path) const -> uint64_t
    {
        if (m_archives.count(path))
        {
            auto const& archive{ m_archives.at(path) };
            return std::distance(archive->begin(), archive->end());
        }

        return 0;
    }

    auto FileSystem::entry_by_index(std::filesystem::path& path, uint64_t index) const -> Entry const*
    {
        auto const& archive{ m_archives.at(path) };
        return (archive->begin() + index)->get();
    }

    auto FileSystem::instance() -> FileSystem&
    {
        static FileSystem instance;
        return instance;
    }

    auto FileSystem::mount(std::filesystem::path& path) -> std::error_code
    {
        auto archive{ std::make_unique<Archive>() };
        auto result{ archive->mount(path) };

        if (!result)
        {
            m_archives.emplace(path, std::move(archive));
        }

        return result;
    }

    auto FileSystem::read(std::string const& path, FileData* file_data) const -> std::error_code
    {
        if (!file_data)
        {
            return make_error_code(ErrorCode::InvalidValues);
        }

        for (auto const& kv : m_archives)
        {
            for (auto const& entry : *kv.second)
            {
                if (*entry == path)
                {
                    entry->contents(&file_data->data, &file_data->size);
                    return {};
                }
            }
        }

        return make_error_code(ErrorCode::MissingEntry);
    }

    void FileSystem::unmount(std::filesystem::path const& path)
    {
        m_archives.erase(path);
    }

} // namespace pak
