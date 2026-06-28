import QtQuick 2.15
import QtQuick.Controls 2.15

TextField {
    id: root

    property var themeColors: ({})
    property bool hasError: false
    property int ctlHeight: 38

    implicitHeight: ctlHeight
    font.pixelSize: 14
    color: themeColors.clrText || "#334155"
    selectionColor: themeColors.clrPrimary || "#3B82F6"
    selectedTextColor: "#FFFFFF"
    horizontalAlignment: Text.AlignLeft
    verticalAlignment: Text.AlignVCenter
    leftPadding: 12
    rightPadding: 12
    selectByMouse: true

    background: Rectangle {
        radius: 8
        color: hasError ? "#FEF2F2" : (themeColors.clrFieldBg || "#F8FAFC")
        border.width: 1
        border.color: activeFocus
            ? (hasError ? "#EF4444" : "#93C5FD")
            : (hasError ? "#FCA5A5" : (themeColors.clrFieldBorder || "#CBD5E1"))
    }
}
