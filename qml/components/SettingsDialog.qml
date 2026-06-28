import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../common"

Popup {
    id: root

    property var themeColors: ({})

    modal: false
    dim: false
    focus: true
    closePolicy: Popup.CloseOnEscape
    padding: 0
    width: shell.width
    height: shell.height

    background: Item {}

    property int selectedTheme: App.themeIndex

    function cancelAndClose() {
        close()
    }

    function applyAndClose() {
        if (selectedTheme !== App.themeIndex) {
            App.themeIndex = selectedTheme
        }
        close()
    }

    onAboutToShow: selectedTheme = App.themeIndex

    Item {
        id: shell
        width: 560
        height: dialogBody.height + 8

        // 轻阴影（不用 QtGraphicalEffects）
        Rectangle {
            anchors.fill: parent
            anchors.topMargin: 5
            radius: 16
            color: themeColors.shadowColor || "#18000000"
        }

        Rectangle {
            id: dialogBody
            width: parent.width
            height: contentLayout.implicitHeight
            radius: 16
            color: themeColors.clrCard || "#FFFFFF"
            border.width: 1
            border.color: themeColors.clrBorder || "#E2E8F0"

            ColumnLayout {
                id: contentLayout
                anchors.fill: parent
                spacing: 0

                RowLayout {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 72
                    Layout.leftMargin: 24
                    Layout.rightMargin: 16
                    spacing: 14

                    Rectangle {
                        Layout.preferredWidth: 44
                        Layout.preferredHeight: 44
                        radius: 12
                        color: themeColors.primarySoft || "#DBEAFE"
                        border.width: 1
                        border.color: "#93C5FD"

                        Text {
                            anchors.centerIn: parent
                            text: "Aa"
                            color: themeColors.clrPrimary || "#2563EB"
                            font.pixelSize: 18
                            font.bold: true
                        }
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4

                        Text {
                            text: qsTr("外观设置")
                            color: themeColors.clrTitle || "#0F172A"
                            font.pixelSize: 20
                            font.bold: true
                        }

                        Text {
                            text: qsTr("选择界面主题，点击「完成」后应用")
                            color: themeColors.clrSubText || "#64748B"
                            font.pixelSize: 13
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                        }
                    }

                    FlatIconButton {
                        text: "✕"
                        themeColors: root.themeColors
                        onClicked: root.cancelAndClose()
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 1
                    color: themeColors.clrBorder || "#E2E8F0"
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.margins: 24
                    spacing: 14

                    FlatSectionLabel {
                        text: qsTr("主题方案")
                        themeColors: root.themeColors
                    }

                    GridLayout {
                        Layout.fillWidth: true
                        columns: 2
                        columnSpacing: 14
                        rowSpacing: 14

                        Repeater {
                            model: App.themeNames()

                            AbstractButton {
                                id: themeBtn
                                Layout.fillWidth: true
                                Layout.preferredHeight: 108
                                checkable: true
                                checked: selectedTheme === index

                                readonly property var themePalette: App.themePalette(index)

                                onClicked: selectedTheme = index

                                background: Rectangle {
                                    radius: 12
                                    color: themeBtn.checked
                                           ? (themeColors.primarySoft || "#DBEAFE")
                                           : (themeBtn.hovered ? "#F8FAFC" : (themeColors.clrCard || "#FFFFFF"))
                                    border.width: themeBtn.checked ? 2 : 1
                                    border.color: themeBtn.checked
                                                  ? (themeColors.clrPrimary || "#3B82F6")
                                                  : (themeBtn.hovered ? "#93C5FD" : (themeColors.clrBorder || "#E2E8F0"))
                                    Behavior on color { ColorAnimation { duration: 140 } }
                                    Behavior on border.color { ColorAnimation { duration: 140 } }
                                }

                                contentItem: Item {
                                    anchors.fill: parent
                                    anchors.margins: 14

                                    Row {
                                        anchors.top: parent.top
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        spacing: 6

                                        Rectangle {
                                            width: 52
                                            height: 28
                                            radius: 6
                                            color: themeBtn.themePalette.clrBg || "#F5F8FD"
                                            border.width: 1
                                            border.color: "#20000000"
                                        }

                                        Rectangle {
                                            width: 52
                                            height: 28
                                            radius: 6
                                            color: themeBtn.themePalette.clrCard || "#FFFFFF"
                                            border.width: 1
                                            border.color: themeColors.clrBorder || "#E2E8F0"
                                        }

                                        Rectangle {
                                            width: 52
                                            height: 28
                                            radius: 6
                                            color: themeBtn.themePalette.clrPrimary || "#3B82F6"
                                        }
                                    }

                                    Text {
                                        anchors.bottom: parent.bottom
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        text: modelData
                                        color: themeBtn.checked
                                               ? (themeColors.clrPrimary || "#1D4ED8")
                                               : (themeColors.clrText || "#334155")
                                        font.pixelSize: 14
                                        font.bold: themeBtn.checked
                                    }

                                    Rectangle {
                                        visible: themeBtn.checked
                                        anchors.top: parent.top
                                        anchors.right: parent.right
                                        width: 22
                                        height: 22
                                        radius: 11
                                        color: themeColors.clrPrimary || "#3B82F6"

                                        Text {
                                            anchors.centerIn: parent
                                            text: "✓"
                                            color: "#FFFFFF"
                                            font.pixelSize: 12
                                            font.bold: true
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 1
                    color: themeColors.clrBorder || "#E2E8F0"
                }

                RowLayout {
                    Layout.fillWidth: true
                    Layout.margins: 20
                    Layout.preferredHeight: 40
                    spacing: 12

                    Item { Layout.fillWidth: true }

                    FlatButton {
                        text: qsTr("取消")
                        tone: "secondary"
                        themeColors: root.themeColors
                        implicitWidth: 96
                        onClicked: root.cancelAndClose()
                    }

                    FlatButton {
                        text: qsTr("完成")
                        tone: "primary"
                        themeColors: root.themeColors
                        implicitWidth: 108
                        onClicked: root.applyAndClose()
                    }
                }
            }
        }
    }
}
