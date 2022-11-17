# .pak I/O

[![CI](https://github.com/TheRealJokerMan/pak-io/actions/workflows/CI.yml/badge.svg)](https://github.com/TheRealJokerMan/pak-io/actions/workflows/CI.yml)

A small, modern C++, library for reading and writing a bespoke archive file format, based on [PAK](https://docs.fileformat.com/game/pak/). The file has a ".pak" extension and is an uncompressed single file, which comprises several individual files.

There are many forms of "PAK" files. Essentially these are uncompressed archives of files. They are frequently used in video games, such as [Quake](https://quakewiki.org/wiki/.pak). They are attractive because they are extremely simple to load at runtime, without any decompression overhead and *any* file can be store in them.

## API

The API is straight forward and all of the API calls return an `std::error_code`.

First, the header file needs to be included: `#include <pak-io/file-system.hxx>`

Then:

<pre>
auto error_code = pak::FileSystem::instance().mount("path/to/archive.pak");
if (!error_code)
{
    std::byte const* data = nullptr;
    uint64_t size = 0;
    error_code = pak::FileSystem::instance().read("file.txt", &data, &size))
    if (!error_code)
    {
        // data and size now have a pointer to the data and the size of that data, in bytes.
    }
    else
    {
        std::cout << "An error occured tried to read from the pak file. [" << error_code.message() < "]" << std::endl;
    }
}
else
{
    std::cout << "An error occured tried to mount the pak file. [" << error_code.message() < "]" << std::endl;
}
</pre>

NB: Several .pak files can be mounted, but there is no guarantee of which file you get if there are naming collisions.

This library also provides Python bindings.

## Tools

There are three tools here:

1. pak-make: This tool will generate a .pak file from a "specification file". It takes two parameters:
    1. `--file` A fully qualified path to a "specification file".
    2. `--directory` A fully qualified path to a directory to store the generated file.

    The "specification file" is an extremely simple JSON file with a format of

    <pre>
    {
      "files" : [
      ]
    }</pre>

    where each entry in `files` is a string either of a fully-qualified path or a path relative to the JSON file.

2. pak-unmake: This tool will extract a .pak file. It takes two parameters:
    1. `--file` A fully qualified path to an existing .pak file.
    2. `--directory` A fully qualified path to a directory to store the extracted files.

3. pak-viewer: This GUI will show the contents of a particular .pak file.
