import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import MagFont 1.0
import "../common"
import "./"

CollapsiblePanel {
    id: root

    readonly property var themePalette: App.themePalette(App.themeIndex)
    themeColors: themePalette

    title: qsTr("调试面板")
    shortTitle: qsTr("调试")
    isLeft: true
    expandedWidth: 320

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
        implicitHeight: AppFont.ctlHeight
        onClicked: App.roamEnabled = !App.roamEnabled

        contentItem: Text {
            text: App.roamEnabled ? qsTr("漫游模式 · 开启") : qsTr("漫游模式")
            color: App.roamEnabled ? "#FFFFFF" : (themeColors.clrSubText || "#64748B")
            font.family: AppFont.family
            font.pixelSize: AppFont.labelMedium
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

    FlatSectionLabel {
        width: parent.width
        text: qsTr("显示选项")
        themeColors: root.themeColors
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

    FlatToggle {
        width: parent.width
        text: qsTr("法向平滑")
        checked: App.smoothNormals
        themeColors: root.themeColors
        onToggled: App.smoothNormals = checked
    }

    FlatDivider { width: parent.width; themeColors: root.themeColors }

    FlatSectionLabel {
        width: parent.width
        text: qsTr("仿真参数")
        themeColors: root.themeColors
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

    FlatSectionLabel {
        width: parent.width
        text: qsTr("头模材质")
        themeColors: root.themeColors
    }

    GridLayout {
        width: parent.width
        columns: 2
        columnSpacing: 8
        rowSpacing: 8

        Repeater {
            model: App.headMaterialNames()

            AbstractButton {
                id: materialBtn
                Layout.fillWidth: true
                Layout.preferredHeight: AppFont.ctlHeight
                checkable: true
                checked: App.headMaterialIndex === index

                onClicked: App.headMaterialIndex = index

                contentItem: Text {
                    text: modelData
                    color: materialBtn.checked
                           ? (themeColors.clrPrimary || "#2563EB")
                           : (themeColors.clrText || "#334155")
                    font.family: AppFont.family
                    font.pixelSize: AppFont.label
                    font.bold: materialBtn.checked
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                background: Rectangle {
                    radius: 8
                    color: materialBtn.checked
                           ? (themeColors.primarySoft || "#DBEAFE")
                           : (materialBtn.hovered ? "#F8FAFC" : "#FFFFFF")
                    border.width: materialBtn.checked ? 2 : 1
                    border.color: materialBtn.checked
                                      ? (themeColors.clrPrimary || "#3B82F6")
                                      : (themeColors.clrBorder || "#E2E8F0")
                }
            }
        }
    }

    FlatDivider { width: parent.width; themeColors: root.themeColors }

    FlatSectionLabel {
        width: parent.width
        text: qsTr("线圈拍位置 / 角度")
        themeColors: root.themeColors
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
