import QtQuick 2.15
import MagFont 1.0

Rectangle {
    readonly property var themePalette: App.themePalette(App.themeIndex)

    color: themePalette.clrCard || "#FCFCFD"

    Rectangle {
        anchors.top: parent.top
        width: parent.width
        height: 1
        color: themePalette.clrBorder || "#E8ECF2"
    }

    Text {
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        verticalAlignment: Text.AlignVCenter
        text: App.statusMessage.length ? App.statusMessage : qsTr("正在加载仿真场景...")
        color: themePalette.clrSubText || "#64748B"
        font.family: AppFont.family
        font.pixelSize: AppFont.label
        elide: Text.ElideRight
    }
}
