#
# Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
# SPDX-License-Identifier: MIT.
#

import os
import sys

from PySide6.QtGui import QGuiApplication
from PySide6.QtQml import QQmlApplicationEngine

import pakmodel


def main():
    """
    Program entry point.
    """

    app = QGuiApplication(sys.argv)
    engine = QQmlApplicationEngine()
    
    engine.quit.connect(app.quit)

    try:
        engine.load(os.path.join(os.path.dirname(__file__), 'qml', 'main.qml'))
        if not engine.rootObjects():
            sys.exit(-1)
    except Exception as ex:
        print(f'An exception occured loading the QML: {ex}')
    
    sys.exit(app.exec())


if __name__ == '__main__':
    main()
