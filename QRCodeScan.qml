import QtQuick 2.0
import QtMultimedia 5.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import QRFilter 1.0

Item {
    property string readText: ""

    Component.onCompleted: {
        camera.start();
    }

    Camera {
        id: camera
    }

    QRFilter {
        id: videoFilter
    }

    Rectangle {
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
                height: 10
                Layout.fillWidth: true
            }

            RowLayout {
                Rectangle {
                    Layout.fillWidth: true
                }

                Button {
                    id: closeButton
                    text: qsTr("Close")

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            camera.stop();

                            screenStack.pop();
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                }
            }

            Rectangle {
                height: 10
                width: parent.width
            }
        }
    }
}
