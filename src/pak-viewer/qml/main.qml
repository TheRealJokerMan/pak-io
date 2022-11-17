//
// Copyright (c) 2022 Jamie Kenyon. All Rights Reserved.
// SPDX-License-Identifier: MIT.
//

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Window
import Qt.labs.qmlmodels

import PakComponents

ApplicationWindow {
    width: Screen.desktopAvailableHeight * 0.25
    height: Screen.desktopAvailableHeight * 0.5
    title: "PAK Viewer"
    visible: true

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {
                text: qsTr("&Open...")

                onTriggered: {
                    fileDialog.open();
                }
            }

            MenuSeparator { }

            Action {
                text: qsTr("&Quit")
                shortcut: StandardKey.Quit

                onTriggered: {
                    Qt.quit();
                }
            }
        }
    }

    ListView {
        id: particleList
        anchors.fill: parent
        anchors.margins: 10
        model: pakModel
        headerPositioning: ListView.OverlayHeader
        header: Rectangle {
            height: 40
            anchors.left: parent.left
            anchors.right: parent.right
            z: 2
            Row {
                anchors.fill: parent
                Text {
                    width: parent.width * 0.5
                    text: qsTr("Path")
                }
                Text {
                    width: parent.width * 0.5
                    text: qsTr("Size")
                }
            }
        }

        delegate: Item {
            height: 40
            anchors.left: parent.left
            anchors.right: parent.right

            Row {
                anchors.fill: parent
                Text {
                    width: parent.width * 0.5
                    text: model.path
                }
                Text {
                    width: parent.width * 0.5
                    text: model.size
                }
            }
            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                height: 1
                visible: model.index !== 0
                color: Material.color(Material.Grey)
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose a PAK file"
        nameFilters: ["PAK Files (*.pak)"]
        visible: false

        onAccepted: {
            pakModel.updateFromPath(fileDialog.selectedFile)
            fileDialog.close()
        }
    }

    PakModel {
        id: pakModel
    }
}
