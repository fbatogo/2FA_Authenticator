import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

Item {
    Rectangle {
        anchors.fill: parent
        color: "white"

        ColumnLayout {
            anchors.fill: parent

            Rectangle {
                width: parent.width
                height: 10
            }

            RowLayout {
                Rectangle {
                    width: 20
                }

                Text {
                    text: qsTr("Settings")
                }

                Rectangle {
                    Layout.fillWidth: true
                }
            }

            Rectangle {
                Layout.fillHeight: true
            }

            RowLayout {
                Rectangle {
                    Layout.fillWidth: true
                }

                Button {
                    text: qsTr("Save")
                }

                Rectangle {
                    width: 10
                }

                Button {
                    text: qsTr("Cancel")

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            // Remove ourselves.
                            screenStack.pop()
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                }
            }

            Rectangle {
                height: 10
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                // Remove ourselves.
                console.log("Pop!");
                screenStack.pop()
            }
        }
    }
}
