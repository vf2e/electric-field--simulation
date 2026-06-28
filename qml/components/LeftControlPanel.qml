import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../common"
import "./"

CollapsiblePanel {
    id: root

    readonly property var themePalette: App.themePalette(App.themeIndex)
    themeColors: themePalette

    title: qsTr("调试面板")
    shortTitle: qsTr("调试")
    isLeft: true
    expandedWidth: 300

    RowLayout {
        width: parent.width
        spacing: 8

        FlatButton {
            text: qsTr("打开")
            tone: "primary"
            themeColors: root.themeColors
            Layout.fillWidth: true
            onClicked: App.openFileDialog()
        }

        FlatButton {
            text: qsTr("设置")
            tone: "secondary"
            themeColors: root.themeColors
            Layout.fillWidth: true
            onClicked: App.openSettings()
        }

        FlatButton {
            text: qsTr("重置")
            tone: "secondary"
            themeColors: root.themeColors
            Layout.fillWidth: true
            onClicked: App.resetCamera()
        }
    }

    AbstractButton {
        width: parent.width
        implicitHeight: 38
        onClicked: App.roamEnabled = !App.roamEnabled

        contentItem: Text {
            text: App.roamEnabled ? qsTr("漫游模式 · 开启") : qsTr("漫游模式")
            color: App.roamEnabled ? "#FFFFFF" : (themeColors.clrSubText || "#64748B")
            font.pixelSize: 13
            font.bold: App.roamEnabled
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        background: Rectangle {
            radius: 8
            color: App.roamEnabled ? "#8B5CF6" : (parent.hovered ? "#F8FAFC" : "#FFFFFF")
            border.width: 1
            border.color: App.roamEnabled ? "#8B5CF6" : (themeColors.clrBorder || "#D8DEE8")
        }
    }

    FlatDivider { width: parent.width; themeColors: root.themeColors }

    Text {
        text: qsTr("显示选项")
        color: themeColors.clrSubText || "#64748B"
        font.pixelSize: 11
        font.bold: true
    }

    FlatToggle {
        width: parent.width
        text: qsTr("显示帧率")
        checked: App.fpsOverlay
        themeColors: root.themeColors
        onToggled: App.fpsOverlay = checked
    }

    FlatToggle {
        width: parent.width
        text: qsTr("显示线圈拍模型")
        checked: App.coilModelVisible
        themeColors: root.themeColors
        onToggled: App.coilModelVisible = checked
    }

    FlatToggle {
        width: parent.width
        text: qsTr("启用电场仿真")
        checked: App.simulationEnabled
        themeColors: root.themeColors
        onToggled: App.simulationEnabled = checked
    }

    FlatDivider { width: parent.width; themeColors: root.themeColors }

    Text {
        text: qsTr("仿真参数")
        color: themeColors.clrSubText || "#64748B"
        font.pixelSize: 11
        font.bold: true
    }

    FlatSliderRow {
        width: parent.width
        label: qsTr("强度")
        from: 0
        to: 100
        value: App.intensity
        suffix: "%"
        themeColors: root.themeColors
        onValueChanged: App.intensity = value
    }

    FlatSliderRow {
        width: parent.width
        label: qsTr("头皮")
        from: 0
        to: 100
        value: App.headOpacity
        suffix: "%"
        accent: true
        themeColors: root.themeColors
        onValueChanged: App.headOpacity = value
    }

    Text {
        text: qsTr("线圈拍位置 / 角度")
        color: themeColors.clrSubText || "#64748B"
        font.pixelSize: 11
        font.bold: true
    }

    FlatSliderRow {
        width: parent.width
        label: "X"
        from: -120; to: 120; value: App.coilX
        sliderEnabled: App.coilControlsEnabled
        themeColors: root.themeColors
        onValueChanged: App.coilX = value
    }
    FlatSliderRow {
        width: parent.width
        label: "Y"
        from: -120; to: 120; value: App.coilY
        sliderEnabled: App.coilControlsEnabled
        themeColors: root.themeColors
        onValueChanged: App.coilY = value
    }
    FlatSliderRow {
        width: parent.width
        label: "Z"
        from: 20; to: 260; value: App.coilZ
        sliderEnabled: App.coilControlsEnabled
        themeColors: root.themeColors
        onValueChanged: App.coilZ = value
    }
    FlatSliderRow {
        width: parent.width
        label: "Rx"
        from: -90; to: 90; value: App.coilRx
        sliderEnabled: App.coilControlsEnabled
        themeColors: root.themeColors
        onValueChanged: App.coilRx = value
    }
    FlatSliderRow {
        width: parent.width
        label: "Ry"
        from: -90; to: 90; value: App.coilRy
        sliderEnabled: App.coilControlsEnabled
        themeColors: root.themeColors
        onValueChanged: App.coilRy = value
    }
    FlatSliderRow {
        width: parent.width
        label: "Rz"
        from: -90; to: 90; value: App.coilRz
        sliderEnabled: App.coilControlsEnabled
        themeColors: root.themeColors
        onValueChanged: App.coilRz = value
    }
}
