import QtQuick 2.15
import QtQuick.Controls 2.15
import MagFont 1.0

AbstractButton {
    id: root

    property var themeColors: ({})

    checkable: true
    implicitHeight: AppFont.ctlHeight
    implicitWidth: parent ? parent.width : 280
    padding: 10
    leftPadding: 12
    rightPadding: 12

    contentItem: Text {
        text: root.text
        color: root.checked ? (themeColors.clrPrimary || "#1D4ED8") : (themeColors.clrSubText || "#64748B")
        font.family: AppFont.family
        font.pixelSize: AppFont.labelMedium
        font.bold: root.checked
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        radius: 8
        color: root.checked ? (themeColors.primarySoft || "#EFF6FF") : (parent.hovered ? "#F8FAFC" : "#FFFFFF")
        border.width: 1
        border.color: root.checked ? "#93C5FD" : (themeColors.clrBorder || "#D8DEE8")
        Behavior on color { ColorAnimation { duration: 120 } }
    }
}
