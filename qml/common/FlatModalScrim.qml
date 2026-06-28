import QtQuick 2.15

Rectangle {
    id: root

    property var themeColors: ({})
    property bool active: false

    anchors.fill: parent
    color: themeColors.clrScrim || "#590F172A"
    opacity: active ? 1.0 : 0.0
    visible: opacity > 0.001

    Behavior on opacity {
        NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
    }
}
