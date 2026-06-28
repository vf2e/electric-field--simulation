import QtQuick 2.15
import "../components"
import "../common"

Item {
    id: root

    readonly property var themeColors: App.themePalette(App.themeIndex)
    readonly property bool dialogOpen: settingsDialog.visible || messageDialog.visible

    FlatModalScrim {
        id: scrim
        z: 0
        themeColors: root.themeColors
        active: root.dialogOpen
    }

    MouseArea {
        z: 0
        anchors.fill: parent
        enabled: root.dialogOpen
        onClicked: {
            if (settingsDialog.visible) {
                settingsDialog.cancelAndClose()
            }
            if (messageDialog.visible) {
                messageDialog.close()
            }
        }
    }

    SettingsDialog {
        id: settingsDialog
        z: 1
        parent: root
        anchors.centerIn: parent
        themeColors: root.themeColors
        onVisibleChanged: root.updateModalState()
    }

    MessageDialog {
        id: messageDialog
        z: 1
        parent: root
        anchors.centerIn: parent
        themeColors: root.themeColors
        onVisibleChanged: root.updateModalState()
    }

    function updateModalState() {
        App.notifyModalVisible(settingsDialog.visible || messageDialog.visible)
    }

    function openSettingsDialog() {
        settingsDialog.open()
    }

    function openMessageDialog(title, message) {
        messageDialog.show(title, message)
    }
}
