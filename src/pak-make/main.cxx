//
// Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
// SPDX-License-Identifier: MIT.
//

#include <pak-io/file-system.hxx>

#include <fmt/format.h>
#include <cstdint>
#include <cstdlib>
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace worker
{
    static auto pack(std::filesystem::path specification, std::filesystem::path destination) -> bool
    {
        auto const archive_name = specification.filename().stem().string();
        auto const base_directory = std::filesystem::path(specification).remove_filename();
        std::vector<std::filesystem::path> path_list;

        try
        {
            std::ifstream ifs(specification);
            auto jf = nlohmann::json::parse(ifs);

            for (auto const& elem : jf["files"])
            {
                std::filesystem::path entry = elem;

                if (entry.is_relative())
                {
                    // Entries with a relative path are expected to be relative the specification file.
                    entry = base_directory / entry;
                }

                path_list.emplace_back(entry);
            }
        }
        catch (std::exception const& ex)
        {
            std::cerr << ex.what();
            return false;
        }

        auto error_code = pak::Archive::create(archive_name, destination, base_directory, path_list);
        if (error_code)
        {
            std::cerr << fmt::format("Failed to create archive '{}'", error_code.message()) << std::endl;
            return false;
        }

        return true;
    }

} // namespace worker

auto main(int32_t argc, char* argv[]) -> int32_t
{
    cxxopts::Options options("pak-make", "Create an archive");

    // clang-format off
    options.add_options()
        ("f,file", "Path of .json specification file", cxxopts::value<std::string>())
        ("d,directory", "Destination directory", cxxopts::value<std::string>())
        ("h,help", "Print usage");
    // clang-format on

    cxxopts::ParseResult result;

    try
    {
        result = options.parse(argc, argv);
    }
    catch (cxxopts::exceptions::exception const& ex)
    {
        std::cerr << "pak-make: " << ex.what() << std::endl;
        std::cout << options.help() << std::endl;
        return EXIT_FAILURE;
    }

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        return EXIT_SUCCESS;
    }

    if (!result.count("file"))
    {
        std::cerr << "pak-make: missing input file!" << std::endl;
        std::cout << options.help() << std::endl;
        return EXIT_FAILURE;
    }

    if (!result.count("directory"))
    {
        std::cerr << "pak-make: missing output directory!" << std::endl;
        std::cout << options.help() << std::endl;
        return EXIT_FAILURE;
    }

    return worker::pack(result["file"].as<std::string>(), result["directory"].as<std::string>()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
