import QtQuick 2.15

Text {
    property var themeColors: ({})

    color: themeColors.clrSubText || "#64748B"
    font.pixelSize: 11
    font.bold: true
    font.letterSpacing: 0.4
}
