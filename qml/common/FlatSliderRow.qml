import QtQuick 2.15
import QtQuick.Controls 2.15

Column {
    id: root

    property string label: ""
    property int from: 0
    property int to: 100
    property int value: 50
    property string suffix: ""
    property bool accent: false
    property bool sliderEnabled: true
    property var themeColors: ({})

    spacing: 6
    width: parent ? parent.width : implicitWidth
    opacity: sliderEnabled ? 1.0 : 0.55

    Row {
        width: parent.width
        spacing: 8

        Text {
            text: label
            width: 28
            color: themeColors.clrSubText || "#64748B"
            font.pixelSize: 12
            verticalAlignment: Text.AlignVCenter
            height: 20
        }

        Slider {
            id: slider
            from: root.from
            to: root.to
            value: root.value
            enabled: root.sliderEnabled
            width: parent.width - 28 - 8 - valueLabel.implicitWidth - 8
            height: 24

            onMoved: root.value = Math.round(value)
            onValueChanged: if (!pressed) root.value = Math.round(value)

            background: Rectangle {
                x: slider.leftPadding
                y: slider.topPadding + slider.availableHeight / 2 - height / 2
                implicitWidth: 200
                width: slider.availableWidth
                height: 5
                radius: 3
                color: themeColors.clrBorder || "#E8ECF2"

                Rectangle {
                    width: slider.visualPosition * parent.width
                    height: parent.height
                    radius: 3
                    color: accent ? "#38BDF8" : (themeColors.clrPrimary || "#3B82F6")
                }
            }

            handle: Rectangle {
                x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
                y: slider.topPadding + slider.availableHeight / 2 - height / 2
                implicitWidth: 16
                implicitHeight: 16
                radius: 8
                color: "#FFFFFF"
                border.width: 2
                border.color: accent ? "#38BDF8" : (themeColors.clrPrimary || "#3B82F6")
            }
        }

        Text {
            id: valueLabel
            text: suffix.length ? (value + suffix) : String(value)
            color: themeColors.clrText || "#334155"
            font.pixelSize: 12
            font.bold: true
            width: 44
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            height: 20
        }
    }
}
