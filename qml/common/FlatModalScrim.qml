import QtQuick 2.15

Rectangle {
    id: root

    property var themeColors: ({})

    anchors.fill: parent
    color: themeColors.clrScrim || "#590F172A"
    opacity: visible ? 1.0 : 0.0
    visible: opacity > 0

    Behavior on opacity {
        NumberAnimation { duration: 180; easing.type: Easing.OutCubic }
    }
}
