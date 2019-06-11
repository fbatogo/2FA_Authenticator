// Pie graphic drawing based on code/sample at http://www.bytebau.com/progress-circle-with-qml-and-javascript/
// ByteBau (Jörn Buchholz) @bytebau.com

import QtQuick 2.0
import QtQml 2.2

Item {
    id: root

    width: size
    height: size

    property string name: ""             // The name of this progress circle.
    property int size: 200               // The size of the circle in pixels
    property int maxTime: 30             // The length of time that will show the circle at 100%
    property int currentTime: 30         // The current time in the countdown.
    property string circleColor: "#0000ff"
    property int countDown: 0

    function startIfNotRunning() {
        if (!circleAnimation.running) {
            circleAnimation.restart();
        }
    }


    onCurrentTimeChanged: {
        if (circleAnimation.running) {
            circleAnimation.stop();
        }

        // Draw the current view before starting the countdown.
        canvas.requestPaint();

        // Set the new to/from/duration values.
        if (currentTime == 0) {
            circleAnimation.from = 360;
            circleAnimation.duration = 30000;
        } else {
            circleAnimation.from = ((currentTime / maxTime) * 360);
            circleAnimation.duration = (currentTime * 1000);
        }
        circleAnimation.to = 0;


        circleAnimation.restart();
    }

    onCountDownChanged: {

        //console.log("[" + Math.floor(Date.now() / 1000) + "] '" + name + "' count down = " + countDown);
        canvas.requestPaint();

        if (countDown <= 0) {
            // Make sure the animation stops when we are done.
            circleAnimation.stop();
        }
    }

    NumberAnimation {
        id: circleAnimation
        target: root
        property: "countDown"
        easing.type: Easing.Linear
        to: 0
        running: false
    }

    Canvas {
        id: canvas
        anchors.fill: parent
        rotation: -90

        onPaint: {
            var ctx = getContext("2d")
            var x = width / 2
            var y = height / 2
            var start = Math.PI * 0
            var end = Math.PI * (countDown / 180)

            ctx.reset()

            ctx.beginPath()
            ctx.fillStyle = root.circleColor
            ctx.moveTo(x, y)
            ctx.arc(x, y, width / 2, start, end, false)
            ctx.lineTo(x, y)
            ctx.fill();
        }
    }
}
