import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

Item {
    Component.onCompleted: {
        // Change the icon on the toolbar to be the back button.
        menuButton.source = "resources/back.svg";
    }

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
