//
// Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
// SPDX-License-Identifier: MIT.
//

#pragma once

#include <system_error>

namespace pak
{
    /// The possible errors relating to the reading or writing of an archive.
    enum class ErrorCode : int32_t
    {
        /// When creating an archive, the target archive already exists.
        TargetArchiveExists,

        /// When creating an archive, a source entry does not exist.
        SourceDoesNotExist,

        /// When reading an archive, the specified file does not exist.
        ArchiveDoesNotExist,

        /// When reading an archive, the archive header is invalid.
        ArchiveInvalidHeader,

        /// When reading an archive, the archive version does not match this version of the API.
        ArchiveWrongVersion,

        /// When reading an archive, the archive name is not present.
        ArchiveNameMissing,

        /// When reading an entry, the entry path is not present.
        EntryPathMissing,

        /// When reading an entry, the entry data is not present.
        EntryDataMissing,

        /// When requesting an entry, the entry cannot be found.
        MissingEntry,

        /// Invalid values have been supplied.
        InvalidValues
    };

    /// Create an error code.
    /// \param code The pak-io related error code.
    /// \return A valid error code.
    auto make_error_code(ErrorCode code) -> std::error_code;

} // namespace pak
