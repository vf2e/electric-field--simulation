import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import EfieldViewer 1.0

import "components"
import "common"

ApplicationWindow {
    id: windowRoot

    visible: true
    width: 1440
    height: 900
    minimumWidth: 1024
    minimumHeight: 680
    title: qsTr("EField Viewer · 八字拍电场仿真")
    color: themePalette.clrBg || "#F5F8FD"

    readonly property var themePalette: App.themePalette(App.themeIndex)

    SettingsDialog {
        id: settingsDialog
        themeColors: windowRoot.themePalette
    }

    MessageDialog {
        id: messageDialog
        themeColors: windowRoot.themePalette
    }

    Connections {
        target: App
        function onErrorOccurred(message) {
            messageDialog.show(qsTr("错误"), message)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Top header bar
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 56
            color: themePalette.clrCard || "#FFFFFF"
            border.width: 0
            border.color: themePalette.clrBorder || "#E2E8F0"

            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: themePalette.clrBorder || "#E2E8F0"
            }

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                spacing: 16

                Row {
                    spacing: 10
                    Layout.alignment: Qt.AlignVCenter

                    Rectangle {
                        width: 32
                        height: 32
                        radius: 8
                        color: themePalette.clrPrimary || "#3B82F6"

                        Text {
                            anchors.centerIn: parent
                            text: "E"
                            color: "#FFFFFF"
                            font.pixelSize: 16
                            font.bold: true
                        }
                    }

                    Column {
                        spacing: 2
                        anchors.verticalCenter: parent.verticalCenter

                        Text {
                            text: qsTr("EField Viewer")
                            color: themePalette.clrTitle || "#0F172A"
                            font.pixelSize: 15
                            font.bold: true
                        }

                        Text {
                            text: qsTr("八字拍 · YRD-B9076 电场仿真")
                            color: themePalette.clrSubText || "#64748B"
                            font.pixelSize: 11
                        }
                    }
                }

                Item { Layout.fillWidth: true }

                Rectangle {
                    Layout.preferredWidth: statusChip.implicitWidth + 24
                    Layout.preferredHeight: 34
                    radius: 8
                    color: themePalette.primarySoft || "#EFF6FF"
                    border.width: 1
                    border.color: "#93C5FD"

                    Text {
                        id: statusChip
                        anchors.centerIn: parent
                        text: App.simulationEnabled ? qsTr("仿真运行中") : qsTr("仿真已暂停")
                        color: themePalette.clrPrimary || "#1D4ED8"
                        font.pixelSize: 12
                        font.bold: true
                    }
                }
            }
        }

        // Main workspace
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 12

                LeftControlPanel {
                    id: leftPanel
                    Layout.preferredWidth: collapsed ? 44 : 300
                    Layout.fillHeight: true
                    themeColors: windowRoot.themePalette
                    settingsDialog: settingsDialog
                }

                // 3D viewport frame
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: 14
                    color: "transparent"
                    border.width: 1
                    border.color: themePalette.clrBorder || "#E2E8F0"
                    clip: true

                    VtkHost {
                        id: vtkViewport
                        objectName: "vtkViewport"
                        anchors.fill: parent
                        anchors.margins: 1
                        z: 1
                    }

                    // Viewport hint overlay (QML layer above native VTK)
                    Text {
                        z: 2
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.margins: 14
                        text: qsTr("左键旋转 · 中键平移 · 滚轮缩放 · Home 重置")
                        color: Qt.rgba(0.39, 0.45, 0.55, 0.75)
                        font.pixelSize: 11
                    }
                }

                ColorBarPanel {
                    id: rightPanel
                    Layout.preferredWidth: collapsed ? 44 : 168
                    Layout.fillHeight: true
                    themeColors: windowRoot.themePalette
                }
            }
        }

        // Status bar
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 32
            color: themePalette.clrCard || "#FCFCFD"

            Rectangle {
                anchors.top: parent.top
                width: parent.width
                height: 1
                color: themePalette.clrBorder || "#E8ECF2"
            }

            Text {
                anchors.fill: parent
                anchors.leftMargin: 16
                anchors.rightMargin: 16
                verticalAlignment: Text.AlignVCenter
                text: App.statusMessage.length ? App.statusMessage : qsTr("正在加载仿真场景...")
                color: themePalette.clrSubText || "#64748B"
                font.pixelSize: 12
                elide: Text.ElideRight
            }
        }
    }

    Shortcut {
        sequence: StandardKey.Open
        onActivated: App.openFileDialog()
    }

    Shortcut {
        sequence: "Home"
        onActivated: App.resetCamera()
    }
}
