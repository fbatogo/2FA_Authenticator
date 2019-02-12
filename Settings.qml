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

            // List the configuration options that we allow.
            CheckBox {
                id: showIssuer
                tristate: false
                checked: false      // XXX For now.
                text: qsTr("Show Issuer")
            }

            CheckBox {
                id: showHotpCounter
                tristate: false
                checked: false
                text: qsTr("Show HOTP Counter Value")
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

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            // XXX Save the configuration settings.

                            // Remove our widget from the stack.
                            console.log("Pop!");
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
    }
}
