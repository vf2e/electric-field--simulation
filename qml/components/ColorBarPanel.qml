import QtQuick 2.15
import QtQuick.Layouts 1.15
import "../common"
import "./"

CollapsiblePanel {
    id: root

    readonly property var themePalette: App.themePalette(App.themeIndex)
    themeColors: themePalette

    title: qsTr("色标")
    shortTitle: qsTr("色标")
    isLeft: false
    expandedWidth: 184

    FlatSectionLabel {
        width: parent.width
        text: qsTr("电场强度")
        themeColors: root.themeColors
    }

    FlatCard {
        width: parent.width
        themeColors: root.themeColors
        useShadow: false
        cardRadius: 10
        implicitHeight: colorBarSection.implicitHeight + 20

        ColumnLayout {
            id: colorBarSection
            anchors.fill: parent
            anchors.margins: 10
            spacing: 8

            Text {
                Layout.fillWidth: true
                text: qsTr("V/m")
                color: themeColors.clrSubText || "#64748B"
                font.pixelSize: 11
                horizontalAlignment: Text.AlignHCenter
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.preferredHeight: Math.max(220, root.height * 0.42)
                spacing: 10

                Rectangle {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 36
                    radius: 6
                    clip: true
                    color: "transparent"
                    border.width: 1
                    border.color: themeColors.clrBorder || "#E2E8F0"

                    ColorBar {
                        anchors.fill: parent
                        lutMin: App.lutMin
                        lutMax: App.lutMax
                        themeColors: root.themeColors
                    }
                }

                ColumnLayout {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 52
                    spacing: 4

                    Text {
                        text: App.lutMax
                        color: themeColors.clrText || "#334155"
                        font.pixelSize: 13
                        font.bold: true
                    }

                    Text {
                        text: qsTr("高")
                        color: themeColors.clrSubText || "#94A3B8"
                        font.pixelSize: 10
                    }

                    Item { Layout.fillHeight: true }

                    Text {
                        text: qsTr("低")
                        color: themeColors.clrSubText || "#94A3B8"
                        font.pixelSize: 10
                    }

                    Text {
                        text: App.lutMin
                        color: themeColors.clrText || "#334155"
                        font.pixelSize: 13
                        font.bold: true
                    }
                }
            }
        }
    }

    FlatDivider {
        width: parent.width
        themeColors: root.themeColors
    }

    FlatSectionLabel {
        width: parent.width
        text: qsTr("色标范围")
        themeColors: root.themeColors
    }

    ColumnLayout {
        width: parent.width
        spacing: 10

        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            Text {
                text: qsTr("上限")
                color: themeColors.clrSubText || "#64748B"
                font.pixelSize: 12
                Layout.preferredWidth: 28
            }

            FlatTextField {
                Layout.fillWidth: true
                text: String(App.lutMax)
                inputMethodHints: Qt.ImhDigitsOnly
                themeColors: root.themeColors
                onEditingFinished: {
                    var v = parseInt(text)
                    if (!isNaN(v)) App.lutMax = v
                    else text = String(App.lutMax)
                }
            }

            Text {
                text: qsTr("V/m")
                color: themeColors.clrSubText || "#94A3B8"
                font.pixelSize: 11
                Layout.preferredWidth: 24
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            Text {
                text: qsTr("下限")
                color: themeColors.clrSubText || "#64748B"
                font.pixelSize: 12
                Layout.preferredWidth: 28
            }

            FlatTextField {
                Layout.fillWidth: true
                text: String(App.lutMin)
                inputMethodHints: Qt.ImhDigitsOnly
                themeColors: root.themeColors
                onEditingFinished: {
                    var v = parseInt(text)
                    if (!isNaN(v)) App.lutMin = v
                    else text = String(App.lutMin)
                }
            }

            Text {
                text: qsTr("V/m")
                color: themeColors.clrSubText || "#94A3B8"
                font.pixelSize: 11
                Layout.preferredWidth: 24
            }
        }
    }
}
