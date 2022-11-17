//
// Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
// SPDX-License-Identifier: MIT.
//

#include <pak-io/file-system.hxx>

#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <string>

namespace ext
{
    static auto get_env_var(std::string const& key) -> std::string
    {
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
        char const* val = getenv(key.c_str());
#ifdef _MSC_VER
#pragma warning(pop)
#endif
        return val ? std::string(val) : std::string();
    }
} // namespace ext

SCENARIO("archive file reading")
{
    GIVEN("A valid archive ")
    {
        auto archive_path = std::filesystem::path(ext::get_env_var("ARCHIVE_PATH"));
        REQUIRE(!archive_path.empty());

        REQUIRE(!pak::FileSystem::instance().mount(archive_path));

        WHEN("Invalid values are supplied.")
        {
            REQUIRE(pak::FileSystem::instance().read("", nullptr));
        }

        WHEN("An entry is unknown.")
        {
            pak::FileData file_data;
            REQUIRE(pak::FileSystem::instance().read("does/not/exist.txt", &file_data));
        }

        WHEN("An entry is known.")
        {
            pak::FileData file_data;

            REQUIRE(!pak::FileSystem::instance().read("one-subdirectory/two-subdirectory/two-mebibytes.txt", &file_data));

            THEN("The entry is correct.")
            {
                REQUIRE(file_data.data);
                REQUIRE(file_data.size == 2 * 1024 * 1024);
            }
        }
    }
}
