import QtQuick 2.15
import MagFont 1.0

Text {
    property var themeColors: ({})

    color: themeColors.clrSubText || "#64748B"
    font.family: AppFont.family
    font.pixelSize: AppFont.caption
    font.bold: true
    font.letterSpacing: 0.4
}
