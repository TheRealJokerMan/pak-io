//
// Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
// SPDX-License-Identifier: MIT.
//

#include <pybind11/pybind11.h>
#include <pybind11/stl/filesystem.h>

#include <pak-io/file-system.hxx>

namespace pypakio
{
    namespace py = pybind11;

    static void init_pypakio(py::module& m)
    {
        // clang-format off

        py::class_<pak::Entry>(m, "Entry")
            .def("path", &pak::Entry::path, "Get the path of the entry.")
            .def("size", &pak::Entry::size, "Get the size of the entry's data, in bytes.");

        py::class_<pak::FileData>(m, "FileData")
            .def(py::init<>())
            .def_readwrite("data", &pak::FileData::data)
            .def_readwrite("size", &pak::FileData::size);

        py::class_<pak::FileSystem, std::unique_ptr<pak::FileSystem, py::nodelete>>(m, "FileSystem")
            .def(py::init([]()
                { 
                    return std::unique_ptr<pak::FileSystem, py::nodelete>(&pak::FileSystem::instance()); 
                }))
            .def("count_entries", &pak::FileSystem::count_entries, "Count the number of entries in an archive.", py::arg("path"))
            .def("entry_by_index", &pak::FileSystem::entry_by_index, "Get an entry by indexe.", py::arg("path"), py::arg("index"))
            .def("mount", &pak::FileSystem::mount, "Mount an archive for reading, adding its entries to the index.", py::arg("path"))
            .def("read", &pak::FileSystem::read, " Read the contents of a file.", py::arg("path"), py::arg("file_data"))
            .def("unmount", &pak::FileSystem::unmount, "Unmount an already mounted archive.", py::arg("path"));

        // clang-format on
    }

    static void init_system_error(py::module& m)
    {
        // clang-format off
        py::class_<std::error_code>(m, "error_code")
            .def(py::init<>())
            .def("value", &std::error_code::value)
            .def("message", &std::error_code::message)
            .def("__bool__", [](const std::error_code& v) { return static_cast<bool>(v); })
            .def("__repr__", [](const std::error_code& v) { return v.message(); });
        // clang-format on
    }

    PYBIND11_MODULE(pypakio, m)
    {
        m.doc() = "PAK I/O Library";

        init_pypakio(m);
        init_system_error(m);
    }
} // namespace pypakio
