import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtMultimedia 5.11
import InterfaceSingleton 1.0

Item {
    id: newEntryScreen

    property string siteName: ""
    property string otpSecret: ""

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
                    text: qsTr("Add a new site with secret key.")
                }

                Rectangle {
                    Layout.fillWidth: true
                }
            }

            Rectangle {
                height: 10
            }

            RowLayout {
                Rectangle {
                    width: 50
                }

                GridLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    columns: 2
                    rows: 3

                    Text {
                        id: siteNameLabel
                        Layout.row: 0
                        Layout.column: 0
                        horizontalAlignment: Text.AlignRight

                        text: qsTr("Site Name : ")
                    }

                    Rectangle {
                        Layout.row: 0
                        Layout.column: 1
                        height: siteNameLabel.height * 2
                        Layout.fillWidth: true

                        border.color: "black"
                        border.width: 1

                        TextInput {
                            id: siteNameInput
                            text: siteName
                            anchors.fill: parent
                            anchors.topMargin: 3
                            anchors.bottomMargin: 3
                            anchors.leftMargin: 3
                            anchors.rightMargin: 3
                            font.pixelSize: height - 2
                            anchors.centerIn: parent
                            clip: true
                        }
                    }

                    Text {
                        id: secretValueLabel
                        Layout.row: 1
                        Layout.column: 0
                        horizontalAlignment: Text.AlignRight

                        text: qsTr("Secret Value : ")
                    }

                    Rectangle {
                        // Put a line around the text entry area.
                        Layout.row: 1
                        Layout.column: 1
                        Layout.fillWidth: true
                        height: secretValueLabel.height * 2

                        border.color: "black"
                        border.width: 1

                        TextInput {
                            id: secretValueInput
                            anchors.fill: parent
                            anchors.topMargin: 3
                            anchors.bottomMargin: 3
                            anchors.leftMargin: 3
                            anchors.rightMargin: 3
                            font.pixelSize: height - 2
                            anchors.centerIn: parent
                            clip: true
                            text: otpSecret
                        }
                    }

                    Text {
                        id: digitCountLabel
                        Layout.row: 2
                        Layout.column: 0
                        horizontalAlignment: Text.AlignRight

                        text: qsTr("Digit Count : ")
                    }

                    ComboBox {
                        id: numberCountComboBox
                        Layout.row: 2
                        Layout.column: 1
                        Layout.fillWidth: true
                        height: digitCountLabel.height + 2

                        model: ["6", "7", "8"]
                    }
                }

                Rectangle {
                    width: 50
                }
            }

            Rectangle {
                height: 5
                Layout.fillWidth: true
            }

            RowLayout {
                Rectangle {
                    Layout.fillWidth: true
                }

                Text {
                    // Hidden error text.
                    id: errorText

                    color: "red"
                    text: qsTr("Provided data is invalid!")
                    visible: false
                }

                Rectangle {
                    Layout.fillWidth: true
                }
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
                    id: getFromCameraButton

                    text: qsTr("Acquire from camera");

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            // We need to push the QRCodeScan screen on to the screen stack.
                            screenStack.push(Qt.resolvedUrl("QRCodeScan.qml"));
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
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
                    id: saveButton

                    text: qsTr("Save")

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("Saving key entries...");
                            var keyType, otpType, numberCount;
                            var errorSet = false;

                            // Convert the keyType index to the currect value.
                            switch (otpTypeInput.currentIndex) {
                            case 0:
                                // TOTP
                                otpType = 0;
                                break;

                            case 1:
                                // HOTP
                                otpType = 1;
                                break;

                            default:
                                // Shouldn't happen, but set to an invalid value to cause the save to fail.
                                otpType = -1;
                                errorText.text = qsTr("Invalid OTP type!");
                                errorSet = true;
                                break;
                            }

                            // Convert the keyType index to the correct value.
                            switch (secretFormatInput.currentIndex) {
                            case 0:
                                // Base 32
                                keyType = 1;
                                break;

                            case 1:
                                // HEX
                                keyType = 0;
                                break;

                            default:
                                // Use an invalid value to cause the save to fail.
                                keyType = -1;
                                errorText.text = qsTr("Invalid key format!");
                                errorSet = true;
                                break;
                            }

                            // Convert the combobox index to the correct number count.
                            switch (numberCountComboBox.currentIndex) {
                            case 0:
                                // 6
                                numberCount = 6;
                                break;

                            case 1:
                                // 7
                                numberCount = 7;
                                break;

                            case 2:
                                // 8
                                numberCount = 8;
                                break;

                            default:
                                // Shouldn't be possible, but use an invalid value to cause the save to fail.
                                numberCount = -1;
                                errorText.text = qsTr("Invalid number of numbers.  Must be 6, 7, or 8.");
                                errorSet = true;
                                break;
                            }

                            if (!InterfaceSingleton.addKeyEntry(siteNameInput.text, secretValueInput.text, keyType, otpType, numberCount)) {
                                if (!errorSet) {
                                    errorText.text = qsTr("Unable to save the key entry.  Be sure that values are provided for all of the settings above.");

                                    // Create a timer to make the error text disappear after a few seconds.
                                    Qt.createQmlObject("import QtQuick 2.0; Timer { interval: 3000; running: true; repeat: false; onTriggered: errorText.visible = false; }", parent, "timer");
                                    console.info("Timer set..");
                                }

                                InterfaceSingleton.logError("Failed to save the key data!  Error : " + errorText.text);

                                // Show the text.
                                errorText.visible = true;
                                return;     // Don't close the window.
                            }

                            // Close the window.
                            screenStack.pop();
                        }
                    }
                }

                Rectangle {
                    width: 10
                }

                Button {
                    id: cancelButton

                    text: qsTr("Cancel")

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

            // Take up all the extra space.
            Rectangle {
                height: 10
                Layout.fillWidth: true
            }
        }
    }
}
