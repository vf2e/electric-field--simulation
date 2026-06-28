import QtQuick 2.15
import "../components"

Item {
    id: root

    readonly property var themeColors: App.themePalette(App.themeIndex)

    SettingsDialog {
        id: settingsDialog
        themeColors: root.themeColors
        anchors.centerIn: parent
        onVisibleChanged: root.updateModalState()
    }

    MessageDialog {
        id: messageDialog
        themeColors: root.themeColors
        anchors.centerIn: parent
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
