..
 Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
 SPDX-License-Identifier: MIT.


Overview
=========================================

A small, modern C++, library for reading and writing a bespoke archive file format, based on `PAK <https://docs.fileformat.com/game/pak/>`_. The file has a ".pak" extension and is an uncompressed single file, which comprises several individual files.

There are many forms of "PAK" files. Essentially these are uncompressed archives of files. They are frequently used in video games, such as `Quake <https://quakewiki.org/wiki/.pak>`_. They are attractive because they are extremely simple to load at runtime, without any decompression overhead and *any* file can be store in them.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   api_reference
