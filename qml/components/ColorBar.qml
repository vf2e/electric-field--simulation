import QtQuick 2.15

Item {
    id: root

    property int lutMin: 40
    property int lutMax: 150
    property var themeColors: ({})

    implicitWidth: 36
    implicitHeight: 220

    readonly property var colorStops: [
        { pos: 0.00, color: "#949494" },
        { pos: 0.16, color: "#5C8FD6" },
        { pos: 0.34, color: "#0088FF" },
        { pos: 0.52, color: "#00E5FF" },
        { pos: 0.70, color: "#B8FF40" },
        { pos: 0.86, color: "#FFFF00" },
        { pos: 1.00, color: "#FF2200" }
    ]

    function colorAt(t) {
        t = Math.max(0, Math.min(1, t))
        for (var i = 1; i < colorStops.length; ++i) {
            if (t <= colorStops[i].pos) {
                var t0 = colorStops[i - 1].pos
                var t1 = colorStops[i].pos
                var local = t1 > t0 ? (t - t0) / (t1 - t0) : 0
                return Qt.rgba(
                    (parseInt(colorStops[i - 1].color.substr(1, 2), 16) * (1 - local)
                     + parseInt(colorStops[i].color.substr(1, 2), 16) * local) / 255,
                    (parseInt(colorStops[i - 1].color.substr(3, 2), 16) * (1 - local)
                     + parseInt(colorStops[i].color.substr(3, 2), 16) * local) / 255,
                    (parseInt(colorStops[i - 1].color.substr(5, 2), 16) * (1 - local)
                     + parseInt(colorStops[i].color.substr(5, 2), 16) * local) / 255,
                    1.0)
            }
        }
        return colorStops[colorStops.length - 1].color
    }

    Canvas {
        id: canvas
        anchors.fill: parent
        anchors.margins: 2
        antialiasing: true

        onPaint: {
            var ctx = getContext("2d")
            var h = height
            var w = width
            ctx.clearRect(0, 0, w, h)
            for (var y = 0; y < h; ++y) {
                var t = 1.0 - y / Math.max(1, h - 1)
                var value = lutMin + t * (lutMax - lutMin)
                var norm = lutMax > lutMin ? (value - lutMin) / (lutMax - lutMin) : 1
                ctx.fillStyle = colorAt(norm)
                ctx.fillRect(0, y, w, 1)
            }
        }

        onHeightChanged: requestPaint()
        onWidthChanged: requestPaint()

        Connections {
            target: root
            function onLutMinChanged() { canvas.requestPaint() }
            function onLutMaxChanged() { canvas.requestPaint() }
        }
    }
}
