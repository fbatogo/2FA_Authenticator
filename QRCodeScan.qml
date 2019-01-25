import QtQuick 2.0
import QtMultimedia 5.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

Item {
    property string readText: ""

    Rectangle {
        anchors.fill: parent
        color: "white"

        Camera {
            id: camera
            viewfinder {
                resolution: "160x160"
            }
        }

        ColumnLayout {
            anchors.fill: parent

            Rectangle {
                height: 10
            }

            RowLayout {
                Layout.fillWidth: true

                Rectangle {
                    Layout.fillWidth: true
                }

                Rectangle {
                    border.color: "black"
                    border.width: 1

                    width: 161
                    height: 161

                    // Show the video that we are scanning.
                    VideoOutput {
                        source: camera
                        filters: [ videoFilter ]
                        width: 160
                        height: 160
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                }
            }

            Text {
                id: codeText
                Layout.fillWidth: true
                font.pointSize: 20
                horizontalAlignment: Text.AlignHCenter
            }

            Rectangle {
                Layout.fillHeight: true
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
