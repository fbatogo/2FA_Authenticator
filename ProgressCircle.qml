// Taken from http://www.bytebau.com/progress-circle-with-qml-and-javascript/
// ByteBau (Jörn Buchholz) @bytebau.com

import QtQuick 2.0
import QtQml 2.2

Item {
    id: root

    width: size
    height: size

    property int size: 200               // The size of the circle in pixel
    property real arcBegin: 0            // start arc angle in degree
    property real arcEnd: 90            // end arc angle in degree
    property real arcOffset: 0           // rotation
    property bool showBackground: false  // a full circle as a background of the arc
    property real lineWidth: 20          // width of the line
    property string colorCircle: "#0000ff" //"#CC3333"

    property alias beginAnimation: animationArcBegin.enabled
    property alias endAnimation: animationArcEnd.enabled

    property int animationDuration: 200

    onArcBeginChanged: canvas.requestPaint()
    onArcEndChanged: canvas.requestPaint()

    Behavior on arcBegin {
        id: animationArcBegin
        enabled: true
        NumberAnimation {
            duration: parent.animationDuration
            easing.type: Easing.InOutCubic
        }
    }

    Behavior on arcEnd {
        id: animationArcEnd
        enabled: true
        NumberAnimation {
            duration: root.animationDuration
            easing.type: Easing.InOutCubic
        }
    }

    Canvas {
        id: canvas
        anchors.fill: parent
        rotation: -90 + parent.arcOffset

        onPaint: {
            var ctx = getContext("2d")
            var x = width / 2
            var y = height / 2
            var start = Math.PI * (parent.arcBegin / 180)
            var end = Math.PI * (parent.arcEnd / 180)

            ctx.reset()

            ctx.beginPath()
            ctx.fillStyle = root.colorCircle
            ctx.moveTo(x, y)
            ctx.arc(x, y, width / 2, start, end, false)
            ctx.lineTo(x, y)
            ctx.fill();
        }
    }
}
