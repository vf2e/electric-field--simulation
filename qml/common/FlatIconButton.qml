import QtQuick 2.15
import QtQuick.Controls 2.15
import MagFont 1.0

Button {
    id: root

    property var themeColors: ({})

    implicitWidth: 38
    implicitHeight: 38
    padding: 0

    contentItem: Text {
        text: root.text
        font.family: AppFont.family
        font.pixelSize: AppFont.title
        font.bold: true
        color: hovered ? (themeColors.clrPrimary || "#2563EB") : (themeColors.clrSubText || "#64748B")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        radius: 8
        color: pressed ? "#FEE2E2" : (hovered ? "#F1F5F9" : "transparent")
        border.width: hovered ? 1 : 0
        border.color: themeColors.clrBorder || "#E2E8F0"
        scale: pressed ? 0.94 : (hovered ? 1.02 : 1.0)
        Behavior on scale { NumberAnimation { duration: 120 } }
    }
}
