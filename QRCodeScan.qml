import QtQuick 2.4
import QtMultimedia 5.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import QRFilter 1.0
import Rollin.QRCodeSingleton 1.0
import Rollin.Logger 1.0

Item {
    property string readText: ""
    property bool popped: false

    Component.onCompleted: {
        // Change the icon on the toolbar to be the back button.
        menuButton.source = "/resources/back.svg";

        Log.logDebug("Device Name : " + camera.displayName);
    }

    Camera {
        id: camera
    }

    QRFilter {
        id: videoFilter

        onSignalFinished: {
            if (popped) {
                // Do nothing.  (It isn't unusual for the scanner thread to have processed multiple frames before this signal
                // is emitted and processed.  So, if we have already popped once, we don't want to do it again or we will end
                // up back at the main screen.))
                return;
            }

            // If the code isn't valid, display a message, and return.
            if (!Rollin.QRCodeSingleton.isOtpCode()) {
                Log.logError("Not a valid OTP QR code!");

                // Set the error text, and show it.
                errorText.text = qsTr("The QR code scanned wasn't a valid OTP QR code!");

                // Figure out if we need to "blink" the error, or show it for the first time.
                if (errorText.visible) {
                    // Need to "blink" it.  So, stop the main timer, hide the text, and start the "blink" timer.
                    showErrorTimer.stop();
                    errorText.visible = false;
                    blinkTimer.start();
                } else {
                    // We need to enable the text, and start the timer.
                    errorText.visible = true;
                    showErrorTimer.start();
                }

                // We are done processing.
                Rollin.QRCodeSingleton.setCodeProcessing(false);

                // XXX Spin up a timer to hide this text after some period of time, and to keep from
                // trying to process the same thing again.

                return;
            }

            // It is a valid code.  If we are showing an error, hide it, and stop any timers that might be running.
            errorText.visible = false;

            if (blinkTimer.running) {
                blinkTimer.stop();
            }

            if (showErrorTimer.running) {
                showErrorTimer.stop();
            }

            Log.logDebug("Code found!");
            Log.logDebug("Code label : " + Rollin.QRCodeSingleton.label());
            Log.logDebug("Code type : " + Rollin.QRCodeSingleton.type());
            Log.logDebug("Code secret : " + Rollin.QRCodeSingleton.parameterByKey("secret"));

            popped = true;
            screenStack.pop();
        }
    }

    Timer {
        id: showErrorTimer
        repeat: false
        interval: 5000          // Show the error text for 5 seconds.

        onTriggered: {
            // Hide the error text.
            errorText.visible = false;
        }
    }

    /**
     * This time is used to make the text blink off for half a second, and then back on when someone scans another
     * invalid code, after we are already showing an error.
     */
    Timer {
        id: blinkTimer
        repeat: false
        interval: 500           // Hide the error text for 1/2 a second.

        onTriggered: {
            // Show the error text again.
            errorText.visible = true;

            // Start the timer to remove the text after 5 seconds.
            if (showErrorTimer.running) {
                // Restart the timer.
                showErrorTimer.restart();
            } else {
                // Just start it.
                showErrorTimer.start();
            }
        }
    }

    Rectangle {
        id: topRectangle
        anchors.fill: parent
        color: "white"

        ColumnLayout {
            anchors.fill: parent

            Rectangle {
                height: 10
            }

            RowLayout {
                id: videoRow
                Layout.fillWidth: true
                Layout.fillHeight: true

                Rectangle {
                    width: 9
                }

                Rectangle {
                    id: videoContainer
                    border.color: "black"
                    border.width: 1

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    // Show the video that we are scanning.
                    VideoOutput {
                        source: camera
                        filters: [ videoFilter ]
                        x: 1
                        y: 1
                        width: videoContainer.width - 2
                        height: videoContainer.height - 2
                    }
                }

                Rectangle {
                    width: 9
                }
            }

            Rectangle {
                height: 5
                Layout.fillWidth: true
            }

            TextMetrics {
                id: errorTextSize
                text: errorText.text
                font.pixelSize: errorText.font.pixelSize
            }

            Rectangle {
                Layout.fillWidth: true
                height: errorTextSize.height

                Text {
                    id: errorText
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    text: "This is where an error goes!";
                    color: "red"
                    font.pixelSize: 10
                    visible: false
                }
            }

            Rectangle {
                height: 10
                Layout.fillWidth: true
            }
        }
    }
}
