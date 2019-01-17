import QtQuick 2.0
import QtMultimedia 5.9
import QtQuick.Layouts 1.3
import QRCode 1.0

Item {
    property string readText: ""

    Rectangle {
        anchors.fill: parent
        color: "black"

        Camera {
            id: camera
            viewfinder.resolution: "320x320"
        }

        QRCodeVideoFilter {
            id: videoFilter
            onDetected: {
                // XXX When this is triggered, we need to close this view and return the data to the parent screen.
                readText = code
            }
        }

        ColumnLayout {
            anchors.fill: parent

            // Show the video that we are scanning.
            VideoOutput {
                source: camera
                filters: [ videoFilter ]
            }

            Text {
                id: codeText
                Layout.fillWidth: true
                font.pointSize: 20
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
}
