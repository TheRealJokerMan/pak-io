#
# Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
# SPDX-License-Identifier: MIT.
#

import pathlib

from PySide6 import QtCore
from PySide6.QtCore import Qt, QUrl
from PySide6.QtQml import QmlElement

import pypakio

QML_IMPORT_NAME = "PakComponents"
QML_IMPORT_MAJOR_VERSION = 1

@QmlElement
class PakModel(QtCore.QAbstractListModel):
    """
    A custom model to enable QML to display the contents of an archive.
    """

    PathRole = QtCore.Qt.UserRole + 1000
    SizeRole = QtCore.Qt.UserRole + 1001
    path = None

    def __init__(self, *args, **kwargs):
        super(PakModel, self).__init__(*args, **kwargs)

    def data(self, index, role=Qt.DisplayRole):
        if self.path:
            entry = pypakio.FileSystem().entry_by_index(self.path, index.row())

            if role == PakModel.PathRole:
                return entry.size()
            else:
                return entry.size()

    def roleNames(self):
        roles = dict()
        roles[PakModel.PathRole] = b"path"
        roles[PakModel.SizeRole] = b"size"
        return roles
    
    def rowCount(self, parent=QtCore.QModelIndex()):
        if self.path:
            return pypakio.FileSystem().count_entries(self.path)
        else:
            return 0

    @QtCore.Slot(str)
    def updateFromPath(self, path):
        self.beginResetModel()

        self.path = pathlib.Path(QUrl(path).toLocalFile())        
        pypakio.FileSystem().mount(self.path)

        self.endResetModel()
