//
// Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
// SPDX-License-Identifier: MIT.
//

#include <cstdint>

#define PAK_FILE_HEADER_MAGIC "PAKFILE"

/// The pak file header.
struct PAK_FILE_HEADER
{
    /// The file indentifier.
    uint8_t magic[8] = { PAK_FILE_HEADER_MAGIC };

    /// Specifies the name of the pak file.
    char const* name = nullptr;

    /// Specifies the version of the library that was used to create the file.
    uint32_t version = 0;

    /// Padding.
    uint32_t padding = 0;

    /// Specifies the number of file entries in the pak file.
    uint64_t numEntries = 0;
};

// Ensure the size of the struct is known and that there is no invisible padding.
static_assert(sizeof(PAK_FILE_HEADER) == 32);

/// The header of each entry in a pak file.
struct PAK_FILE_ENTRY
{
    /// Specifies the path of the file.
    char const* path = nullptr;

    /// Specifies the uncompressed size of the file entry.
    uint64_t uncompressedSize = 0;

    /// Specifies the offset of file entry's data, relative to the beginning of the file.
    uint64_t offset = 0;

    /// 16-byte alignment.
    uint64_t padding = 0;
};

// Ensure the size of the struct is known and that there is no invisible padding.
static_assert(sizeof(PAK_FILE_ENTRY) == 32);
