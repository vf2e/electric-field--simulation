import QtQuick 2.15
import QtQuick.Layouts 1.15
import MagFont 1.0

Rectangle {
    id: root

    readonly property var themePalette: App.themePalette(App.themeIndex)

    color: themePalette.clrCard || "#FFFFFF"

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
                width: 36
                height: 36
                radius: 8
                color: themePalette.clrPrimary || "#3B82F6"

                Text {
                    anchors.centerIn: parent
                    text: "E"
                    color: "#FFFFFF"
                    font.family: AppFont.family
                    font.pixelSize: AppFont.title
                    font.bold: true
                }
            }

            Column {
                spacing: 2
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    text: qsTr("EField Viewer")
                    color: themePalette.clrTitle || "#0F172A"
                    font.family: AppFont.family
                    font.pixelSize: AppFont.labelMedium
                    font.bold: true
                }

                Text {
                    text: qsTr("八字拍 · YRD-B9076 电场仿真")
                    color: themePalette.clrSubText || "#64748B"
                    font.family: AppFont.family
                    font.pixelSize: AppFont.caption
                }
            }
        }

        Item { Layout.fillWidth: true }

        Rectangle {
            Layout.preferredWidth: statusChip.implicitWidth + 24
            Layout.preferredHeight: AppFont.ctlHeight
            radius: 8
            color: themePalette.primarySoft || "#EFF6FF"
            border.width: 1
            border.color: "#93C5FD"

            Text {
                id: statusChip
                anchors.centerIn: parent
                text: App.simulationEnabled ? qsTr("仿真运行中") : qsTr("仿真已暂停")
                color: themePalette.clrPrimary || "#1D4ED8"
                font.family: AppFont.family
                font.pixelSize: AppFont.label
                font.bold: true
            }
        }
    }
}
