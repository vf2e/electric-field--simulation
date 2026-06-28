import QtQuick 2.15

Rectangle {
    height: 1
    property var themeColors: ({})
    color: themeColors.clrBorder || "#E2E8F0"
}
