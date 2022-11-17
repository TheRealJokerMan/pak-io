//
// Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
// SPDX-License-Identifier: MIT.
//

#pragma once

#include <pak-io/error-code.hxx>

#include <filesystem>
#include <unordered_map>
#include <vector>

namespace ext
{
    /// Extension to provide a specialisation of std::hash<fs::path>.
    struct PathHash
    {
        /// Generates a hash for a path.
        /// \return A hash.
        auto operator()(const std::filesystem::path& p) const noexcept
        {
            return std::filesystem::hash_value(p);
        }
    };
} // namespace ext

namespace pak
{
    /// An entry in an archive.
    class Entry final
    {
    public:
        /// Constructor.
        /// \param path The path of the entry.
        /// \param data A pointer to beginning of this entry's data.
        /// \param size The length of this entry's data.
        Entry(std::string const& path, uint8_t const* data, uint64_t size);

        /// Obtain details of the contents of this entry.
        /// \param data Receives the contents of the file.
        /// \param size Receives the size of the file's contents, in bytes.
        void contents(uint8_t const** data, uint64_t* size) const;

        /// Accessor.
        /// \return The path of the entry.
        auto path() const -> std::string
        {
            return m_path;
        }

        /// Accessor.
        /// \return The size of the entry's data, in bytes.
        auto size() const -> uint64_t
        {
            return m_data_size;
        }

        /// Equality operator.
        /// \param path A path of a file.
        /// \return true if \a path is the same path as this entry; false otherwise.
        auto operator==(std::string const& path) const -> bool;

    private:
        uint8_t const* m_data_value;
        uint64_t m_data_size;
        std::string m_path;
    };

    /// File provides the ability to read existing archives and create new archives.
    class Archive final
    {
    public:
        /// Accessor.
        /// \return An iterator pointing to the first entry in the archive.
        auto begin() const
        {
            return m_entries.begin();
        }

        /// Create a new archive.
        /// \param name The name of the archive.
        /// \param output_directory Fully qualified path of the directory to create the archive in.
        /// \param relative_directory Paths to sources in the archive, will be relative to this directory.
        /// \param sources An array of fully qualified paths of files to add to the new archive.
        /// \return An empty error code on success; a valid error code otherwise.
        static auto create(std::string const& name,
                           std::filesystem::path const& output_directory,
                           std::filesystem::path const& relative_directory,
                           std::vector<std::filesystem::path> const& sources) -> std::error_code;

        /// Accessor.
        /// \return An iterator pointing to the past-the-end entry in the archive.
        auto end() const
        {
            return m_entries.end();
        }

        /// Extract an existing archive,
        /// \param path The fully qualified path of a file to read.
        /// \param directory Fully qualified path of the directory to create the archive in.
        /// \return An empty error code on success; a valid error code otherwise.
        static auto extract(std::filesystem::path const& path, std::filesystem::path const& directory) -> std::error_code;

        /// Mount an archive for reading, adding its entries to the index.
        /// \param path Fully qualified path of an archive to mount.
        /// \return true on success; false otherwise.
        auto mount(std::filesystem::path const& path) -> std::error_code;

    private:
        std::unique_ptr<std::vector<std::byte>> m_contents;
        std::string m_name;
        std::vector<std::unique_ptr<Entry>> m_entries;
    };

    /// Holds a reference to the data and the size of the data in an entry.
    struct FileData final
    {
        /// Pointer to the beginning of the data.
        uint8_t const* data = nullptr;

        /// The length of the data.
        uint64_t size = 0;
    };

    /// A file-system like interface to reading entries from archives.
    class FileSystem final
    {
    public:
        /// Count the number of entries in an archive.
        /// \param path Fully qualified path of an archive to mount.
        /// \return The number of entries in an archive.
        auto count_entries(std::filesystem::path& path) const -> uint64_t;

        /// Get an entry by index.
        /// \param path Fully qualified path of an archive to mount.
        /// \param index The index of the entry.
        /// \return The entry.
        auto entry_by_index(std::filesystem::path& path, uint64_t index) const -> Entry const*;

        /// Singleton instance accessor.
        /// \return The singleton instance.
        static auto instance() -> FileSystem&;

        /// Mount an archive for reading, adding its entries to the index.
        /// \param path Fully qualified path of an archive to mount.
        /// \return true on success; false otherwise.
        auto mount(std::filesystem::path& path) -> std::error_code;

        /// Read the contents of a file.
        /// \param path The path of the file to read.
        /// \param file_data Receives the contents of the file.
        /// \return An empty error code on success; a valid error code otherwise.
        auto read(std::string const& path, FileData* file_data) const -> std::error_code;

        /// Unmount an already mounted archive.
        /// \param path Fully qualified path of an archive to mount.
        void unmount(std::filesystem::path const& path);

    private:
        std::unordered_map<std::filesystem::path, std::unique_ptr<Archive>, ext::PathHash> m_archives;
    };

} // namespace pak
