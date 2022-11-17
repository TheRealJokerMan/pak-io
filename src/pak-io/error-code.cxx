//
// Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
// SPDX-License-Identifier: MIT.
//

#include "error-code.hxx"

namespace pak
{
    /// Custom error category for error codes.
    class PakErrorCategory final : public std::error_category
    {
    public:
        /// <inheritdoc />
        auto name() const noexcept -> char const* override
        {
            return "pak-io::category";
        }

        /// <inheritdoc />
        auto message(int32_t value) const -> std::string override
        {
            auto const error_code{ static_cast<ErrorCode>(value) };

            switch (error_code)
            {
            case ErrorCode::TargetArchiveExists:
                return "The target archive already exists.";

            case ErrorCode::SourceDoesNotExist:
                return "The source file to add to the new archive, does not exist.";

            case ErrorCode::ArchiveDoesNotExist:
                return "The archive file specified does not exist.";

            case ErrorCode::ArchiveInvalidHeader:
                return "The archive file specified has an invalid header.";

            case ErrorCode::ArchiveWrongVersion:
                return "The version of the archive file was not made with this API version.";

            case ErrorCode::ArchiveNameMissing:
                return "The archive name is not present.";

            case ErrorCode::EntryPathMissing:
                return "The entry path is not present.";

            case ErrorCode::EntryDataMissing:
                return "The entry data is not present.";

            case ErrorCode::MissingEntry:
                return "The requested entry could not be found.";

            case ErrorCode::InvalidValues:
                return "Invalid values have been supplied.";
            };

            return {};
        }
    };

    static auto category() -> std::error_category const&
    {
        static PakErrorCategory instance;
        return instance;
    }

    auto make_error_code(ErrorCode code) -> std::error_code
    {
        return std::error_code(static_cast<int32_t>(code), category());
    }

} // namespace pak
