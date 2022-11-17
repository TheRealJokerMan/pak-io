#
# Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
# SPDX-License-Identifier: MIT.
#

import os
import pathlib
import unittest

import pypakio


class TestBindings(unittest.TestCase):
    """
    A unittest to test the Python bindings.
    """

    def test_file_system(self):
        """
        Test PAK filesystem.
        """

        # Test that the test has been setup correctly.
        assert 'ARCHIVE_PATH' in os.environ

        # Test mounting an invalid archive.
        error_code = pypakio.FileSystem().mount(pathlib.Path("/does/not/exist"))
        assert error_code

        # Test mounting a valid archive.
        archive_path = pathlib.Path(os.environ.get('ARCHIVE_PATH'))
        error_code = pypakio.FileSystem().mount(archive_path)
        assert not error_code

        # Test reading individual entries.
        entry_count = pypakio.FileSystem().count_entries(archive_path)
        assert entry_count == 4

        # Test reading entry properties.
        entry = pypakio.FileSystem().entry_by_index(archive_path, 0)
        assert entry
        assert entry.path() == 'five-twelve-kibibytes.txt'
        assert entry.size() == 512 * 1024

        # Test reading individual entry data.
        file_data = pypakio.FileData()
        error_code = pypakio.FileSystem().read("one-subdirectory/two-subdirectory/two-mebibytes.txt", file_data)
        assert file_data.data
        assert file_data.size == 2 * 1024 * 1024


if __name__ == '__main__':
    unittest.main()
