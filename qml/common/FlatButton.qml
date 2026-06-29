import QtQuick 2.15
import QtQuick.Controls 2.15
import MagFont 1.0

Button {
    id: root

    property string tone: "secondary"   // primary | secondary | danger
    property int ctlHeight: AppFont.ctlHeight
    property var themeColors: ({})

    implicitHeight: ctlHeight
    implicitWidth: Math.max(96, contentItem.implicitWidth + 28)
    padding: 0
    leftPadding: 14
    rightPadding: 14
    font.family: AppFont.family
    font.pixelSize: AppFont.body
    font.bold: tone === "primary" || tone === "danger"

    contentItem: Text {
        text: root.text
        font: root.font
        color: root.enabled ? textColor : mutedText
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    readonly property color textColor: {
        if (tone === "primary") return "#FFFFFF"
        if (tone === "danger") return "#EF4444"
        return themeColors.clrText || "#334155"
    }
    readonly property color mutedText: "#94A3B8"
    readonly property color fillColor: {
        if (!enabled) return "#F1F5F9"
        if (tone === "primary") return hovered ? (themeColors.clrPrimaryHover || "#2563EB") : (themeColors.clrPrimary || "#3B82F6")
        if (tone === "danger") return hovered ? "#FFF1F2" : "#FFFFFF"
        return hovered ? "#F8FAFC" : "#FFFFFF"
    }
    readonly property color borderColor: {
        if (tone === "primary") return "transparent"
        if (tone === "danger") return "#FCA5A5"
        return hovered ? "#93C5FD" : (themeColors.clrBorder || "#E2E8F0")
    }

    background: Rectangle {
        radius: 8
        color: root.fillColor
        border.width: tone === "primary" ? 0 : 1
        border.color: root.borderColor
        Behavior on color { ColorAnimation { duration: 120 } }
    }
}
