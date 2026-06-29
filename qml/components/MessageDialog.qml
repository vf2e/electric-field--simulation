import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import MagFont 1.0
import "../common"

Popup {
    id: root

    property var themeColors: ({})
    property string message: ""
    property string dialogTitle: qsTr("提示")

    modal: false
    dim: false
    focus: true
    closePolicy: Popup.CloseOnEscape
    padding: 0
    width: 420
    implicitHeight: contentLayout.implicitHeight

    background: Item {}

    function show(title, text) {
        dialogTitle = title
        message = text
        open()
    }

    FlatCard {
        anchors.fill: parent
        themeColors: root.themeColors
        useShadow: false
        cardRadius: 12

        ColumnLayout {
            id: contentLayout
            anchors.fill: parent
            anchors.margins: 24
            spacing: 0

            RowLayout {
                Layout.fillWidth: true
                spacing: 12

                Rectangle {
                    width: 48
                    height: 48
                    radius: 24
                    color: themeColors.primarySoft || "#DBEAFE"

                    Text {
                        anchors.centerIn: parent
                        text: "!"
                        color: themeColors.clrPrimary || "#2563EB"
                        font.family: AppFont.family
                        font.pixelSize: AppFont.headline
                        font.bold: true
                    }
                }

                Text {
                    Layout.fillWidth: true
                    text: dialogTitle
                    color: themeColors.clrTitle || "#0F172A"
                    font.family: AppFont.family
                    font.pixelSize: AppFont.title
                    font.bold: true
                    wrapMode: Text.WordWrap
                    maximumLineCount: 2
                    elide: Text.ElideRight
                }
            }

            Text {
                Layout.fillWidth: true
                Layout.topMargin: 12
                Layout.leftMargin: 60
                text: message
                color: themeColors.clrText || "#475569"
                font.family: AppFont.family
                font.pixelSize: AppFont.body
                lineHeight: 1.35
                wrapMode: Text.WordWrap
            }

            FlatDivider {
                Layout.fillWidth: true
                Layout.topMargin: 20
                themeColors: root.themeColors
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 62

                FlatButton {
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("知道了")
                    tone: "primary"
                    themeColors: root.themeColors
                    implicitWidth: 112
                    onClicked: root.close()
                }
            }
        }
    }
}
