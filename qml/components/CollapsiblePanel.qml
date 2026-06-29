import QtQuick 2.15
import MagFont 1.0
import "../common"

Item {
    id: root

    anchors.fill: parent

    readonly property var themePalette: App.themePalette(App.themeIndex)

    Rectangle {
        anchors.fill: parent
        color: themePalette.clrBg || "#F5F8FD"
    }

    default property alias panelContent: bodyColumn.data

    property string title: ""
    property string shortTitle: ""
    property bool collapsed: false
    property bool isLeft: true
    property int expandedWidth: 300
    property var themeColors: ({})

    implicitWidth: collapsed ? 44 : expandedWidth
    implicitHeight: parent ? parent.height : 600

    Behavior on implicitWidth { NumberAnimation { duration: 180; easing.type: Easing.OutCubic } }

    FlatCard {
        anchors.fill: parent
        themeColors: root.themeColors
        useShadow: false
        cardRadius: collapsed ? 12 : 14

        Row {
            anchors.fill: parent
            anchors.margins: collapsed ? 6 : 0
            spacing: 0

            Item {
                visible: collapsed
                width: visible ? 32 : 0
                height: parent.height

                Column {
                    anchors.centerIn: parent
                    spacing: 10

                    FlatIconButton {
                        text: isLeft ? "›" : "‹"
                        themeColors: root.themeColors
                        onClicked: root.collapsed = false
                    }

                    Text {
                        width: 28
                        text: shortTitle
                        rotation: -90
                        transformOrigin: Item.Center
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: themeColors.clrSubText || "#64748B"
                        font.family: AppFont.family
                        font.pixelSize: AppFont.caption
                        font.bold: true
                    }
                }
            }

            Item {
                visible: !collapsed
                width: parent.width - (collapsed ? 32 : 0)
                height: parent.height

                Column {
                    anchors.fill: parent
                    anchors.margins: 14
                    spacing: 12

                    Row {
                        width: parent.width
                        spacing: 8

                        Text {
                            text: title
                            color: themeColors.clrTitle || "#0F172A"
                            font.family: AppFont.family
                            font.pixelSize: AppFont.labelMedium
                            font.bold: true
                            verticalAlignment: Text.AlignVCenter
                            height: AppFont.ctlHeight
                            width: parent.width - collapseBtn.width - 8
                            elide: Text.ElideRight
                        }

                        FlatIconButton {
                            id: collapseBtn
                            text: isLeft ? "‹" : "›"
                            themeColors: root.themeColors
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: root.collapsed = true
                        }
                    }

                    Flickable {
                        width: parent.width
                        height: parent.height - 46
                        contentHeight: bodyColumn.height
                        boundsBehavior: Flickable.StopAtBounds
                        clip: true

                        Column {
                            id: bodyColumn
                            width: parent.width
                            spacing: 12
                        }
                    }
                }
            }
        }
    }
}
