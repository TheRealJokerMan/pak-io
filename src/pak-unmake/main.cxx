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

auto main(int32_t argc, char* argv[]) -> int32_t
{
    cxxopts::Options options("pak-unmake", "Expand an archive");

    // clang-format off
    options.add_options()
        ("f,file", "Path of the archive file", cxxopts::value<std::string>())
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
        std::cerr << "pak-unmake: " << ex.what() << std::endl;
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
        std::cerr << "pak-unmake: missing input file!" << std::endl;
        std::cout << options.help() << std::endl;
        return EXIT_FAILURE;
    }

    if (!result.count("directory"))
    {
        std::cerr << "pak-unmake: missing output directory!" << std::endl;
        std::cout << options.help() << std::endl;
        return EXIT_FAILURE;
    }

    auto error_code = pak::Archive::extract(result["file"].as<std::string>(), result["directory"].as<std::string>());
    if (error_code)
    {
        std::cerr << fmt::format("Failed to create archive '{}'", error_code.message()) << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        return EXIT_SUCCESS;
    }
}
