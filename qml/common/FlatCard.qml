import QtQuick 2.15

Rectangle {
    id: root

    property alias contentItem: contentArea.data
    property bool useShadow: false
    property int cardRadius: 12
    property var themeColors: ({})

    color: themeColors.clrCard || "#FFFFFF"
    radius: cardRadius
    border.width: 1
    border.color: themeColors.clrBorder || "#E2E8F0"

    default property alias children: contentArea.data

    Item {
        id: contentArea
        anchors.fill: parent
    }
}
