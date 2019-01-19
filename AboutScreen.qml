import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import InterfaceSingleton 1.0

Item {

    Component.onCompleted: {
        // Load the license text file, and stick it in the license text area.
        var rawFile = new XMLHttpRequest();
        var resultData = null;

        console.log("Loading the licenses text...");
        rawFile.open("GET", "resources/licenses.txt", false);

        // Set up a callback to be called when the data comes in.
        rawFile.onreadystatechange = function()
        {
            if (rawFile.readyState === 4) {
                if ((rawFile.status === 200) || (rawFile.status === 0)) {
                    // Pass the data to the callback function provided in the original call.
                    resultData = rawFile.responseText;
                }
            }
        }

        // Initiate sending the request to get the data.
        rawFile.send(null);

        licenseText.text = resultData;
        console.log("License text : " + resultData);
    }

    Rectangle {
        anchors.fill: parent
        color: "white"

        ColumnLayout {
            anchors.fill: parent

            Rectangle {
                width: parent.width
                height: 50
            }

            RowLayout {
                Rectangle {
                    Layout.fillWidth: true
                }

                Text {
                    width: parent.width
                    horizontalAlignment: Text.Center
                    text: "2FA Authenticator Version " + InterfaceSingleton.version()
                }

                Rectangle {
                    Layout.fillWidth: true
                }
            }

            Rectangle {
                width: parent.width
                height: 20
            }

            RowLayout {
                Rectangle {
                    Layout.fillWidth: true
                }

                Text {
                    width: parent.width
                    horizontalAlignment: Text.Center
                    text: "(c) Copyright 2019, Chris Hessing"
                }

                Rectangle {
                    Layout.fillWidth: true
                }
            }

            Rectangle {
                width: parent.width
                height: 10
            }

            RowLayout {
                Rectangle {
                    width: 10
                }

                Text {
                    width: parent.width
                    text: qsTr("Licenses")
                }

                Rectangle {
                    Layout.fillWidth: true
                }
            }

            RowLayout {
                Rectangle {
                    width: 10
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    border.color: "black"
                    border.width: 1

                    Flickable {
                        anchors.fill: parent

                        flickableDirection: Flickable.VerticalFlick

                        TextArea.flickable: TextArea {
                            id: licenseText
                            readOnly: true
                            textFormat: TextEdit.RichText
                            text: "License text goes here...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n...\n"
                            wrapMode: TextArea.Wrap
                        }

                        ScrollBar.vertical: ScrollBar { }
                    }
                }

                Rectangle {
                    width: 10
                }
            }

            RowLayout {
                width: parent.width

                Rectangle {
                    Layout.fillWidth: true
                }

                Button {
                    text: "Ok"
                    highlighted: true

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            // Remove ourselves.
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
                width: parent.width
                height: 10
            }
        }
    }
}
