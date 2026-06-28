import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../common"

Popup {
    id: root

    property var themeColors: ({})

    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    padding: 0
    width: 520
    implicitHeight: contentLayout.implicitHeight

    background: Item {}

    onAboutToShow: {
        originalTheme = App.themeIndex
        selectedTheme = App.themeIndex
    }

    property int originalTheme: App.themeIndex
    property int selectedTheme: App.themeIndex

    FlatCard {
        anchors.fill: parent
        themeColors: root.themeColors
        useShadow: false
        cardRadius: 12

        ColumnLayout {
            id: contentLayout
            anchors.fill: parent
            anchors.margins: 0
            spacing: 0

            // 标题栏
            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: 56
                Layout.leftMargin: 24
                Layout.rightMargin: 16
                spacing: 12

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 2

                    Text {
                        text: qsTr("外观设置")
                        color: themeColors.clrTitle || "#0F172A"
                        font.pixelSize: 18
                        font.bold: true
                    }

                    Text {
                        text: qsTr("选择界面主题，实时预览三维视口背景")
                        color: themeColors.clrSubText || "#64748B"
                        font.pixelSize: 13
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }
                }

                FlatIconButton {
                    text: "✕"
                    themeColors: root.themeColors
                    onClicked: root.close()
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
                spacing: 16

                FlatSectionLabel {
                    text: qsTr("主题")
                    themeColors: root.themeColors
                }

                GridLayout {
                    Layout.fillWidth: true
                    columns: 2
                    columnSpacing: 12
                    rowSpacing: 12

                    Repeater {
                        model: App.themeNames()

                        AbstractButton {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 96
                            checkable: true
                            checked: selectedTheme === index

                            onClicked: {
                                selectedTheme = index
                                App.themeIndex = index
                            }

                            background: Rectangle {
                                radius: 10
                                color: checked ? (themeColors.primarySoft || "#DBEAFE")
                                               : (parent.hovered ? "#F8FAFC" : "#FFFFFF")
                                border.width: checked ? 2 : 1
                                border.color: checked ? (themeColors.clrPrimary || "#3B82F6")
                                                      : (parent.hovered ? "#93C5FD" : (themeColors.clrBorder || "#E2E8F0"))
                                Behavior on color { ColorAnimation { duration: 120 } }
                                Behavior on border.color { ColorAnimation { duration: 120 } }
                            }

                            contentItem: Column {
                                anchors.centerIn: parent
                                spacing: 10

                                Rectangle {
                                    id: themeSwatch
                                    width: 128
                                    height: 32
                                    radius: 6
                                    anchors.horizontalCenter: parent.horizontalCenter

                                    property color swatchStart: App.themeAccent(index)
                                    property color swatchEnd: {
                                        var palette = App.themePalette(index)
                                        return palette.clrPrimaryHover || "#2563EB"
                                    }

                                    gradient: Gradient {
                                        orientation: Gradient.Horizontal
                                        GradientStop { position: 0.0; color: themeSwatch.swatchStart }
                                        GradientStop { position: 1.0; color: themeSwatch.swatchEnd }
                                    }

                                    Rectangle {
                                        anchors.fill: parent
                                        radius: 6
                                        color: "transparent"
                                        border.width: 1
                                        border.color: "#20000000"
                                    }
                                }

                                Text {
                                    text: modelData
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    color: parent.parent.checked ? (themeColors.clrPrimary || "#1D4ED8") : (themeColors.clrText || "#334155")
                                    font.pixelSize: 13
                                    font.bold: parent.parent.checked
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
                Layout.preferredHeight: 38
                spacing: 12

                Item { Layout.fillWidth: true }

                FlatButton {
                    text: qsTr("取消")
                    tone: "secondary"
                    themeColors: root.themeColors
                    implicitWidth: 92
                    onClicked: {
                        App.themeIndex = originalTheme
                        root.close()
                    }
                }

                FlatButton {
                    text: qsTr("完成")
                    tone: "primary"
                    themeColors: root.themeColors
                    implicitWidth: 104
                    onClicked: root.close()
                }
            }
        }
    }
}
