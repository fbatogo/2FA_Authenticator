import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtMultimedia 5.11
import InterfaceSingleton 1.0
import QRCodeSingleton 1.0

import "resources/javascript/NewOrEditEntryScreen.js" as NewOrEdit
import "resources/widgets/"

Item {
    id: newEntryScreen

    property bool editing: false
    property string identifier: ""

    // XXX Only enable the save button when we have data in all of the required fields!

    Component.onCompleted: NewOrEdit.init();

    onVisibleChanged: NewOrEdit.onVisibleChanged(visible);

    Rectangle {
        anchors.fill: parent
        color: "white"

        ColumnLayout {
            anchors.fill: parent

            VerticalPadding {
                size: 10
            }

            SubHeader {
                headerText: {
                    if (!editing) {
                        return qsTr("Add a new site with secret key.");
                    }

                    return qsTr("Edit information for a site with a secret key.");
                }
            }

            VerticalPadding {
                size: 5
            }

            RowLayout {
                HorizontalPadding {
                    size: 20
                }

                GridLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    columns: 2
                    rows: 5

                    // Key type value setting
                    Text {
                        id: otpTypeLabel
                        Layout.row: 0
                        Layout.column: 0
                        horizontalAlignment: Text.AlignRight

                        text: qsTr("OTP Type : ")
                    }

                    ComboBox {
                        id: otpTypeComboBox
                        Layout.row: 0
                        Layout.column: 1
                        Layout.fillWidth: true
                        //height: digitCountLabel.height + 2
                        height: {
                            console.log("Height : " + hiddenNumbers.height);
                            return hiddenNumbers.height;
                        }

                        model: otpTypeModel         // XXX When HOTP is selected, we need to show a row to allow inputting the current counter value.
                    }

                    ListModel {
                        id: otpTypeModel

                        ListElement {
                            otpType: "TOTP"
                        }

                        ListElement {
                            otpType: "HOTP"
                        }
                    }


                    // Site name/label value
                    Text {
                        id: siteNameLabel
                        Layout.row: 1
                        Layout.column: 0
                        horizontalAlignment: Text.AlignRight

                        text: qsTr("Site Name : ")
                    }

                    Rectangle {
                        Layout.row: 1
                        Layout.column: 1
                        height: siteNameLabel.height * 2
                        Layout.fillWidth: true
                        color: "transparent"

                        border.color: "black"
                        border.width: 1

                        TextInput {
                            id: siteNameInput
                            text: ""
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

                    // Secret value format
                    Text {
                        id: secretValueTypeLabel
                        Layout.row: 2
                        Layout.column: 0
                        horizontalAlignment: Text.AlignRight

                        text: qsTr("Secret Format : ")
                    }

                    ComboBox {
                        id: secretValueTypeComboBox
                        Layout.row: 2
                        Layout.column: 1
                        Layout.fillWidth: true
                        //height: digitCountLabel.height + 2
                        height: {
                            console.log("Height : " + hiddenNumbers.height);
                            return hiddenNumbers.height;
                        }

                        model: secretValueTypeModel         // XXX When HOTP is selected, we need to show a row to allow inputting the current counter value.
                    }

                    ListModel {
                        id: secretValueTypeModel

                        ListElement {
                            otpType: "Base32"
                        }

                        ListElement {
                            otpType: "HEX"
                        }
                    }


                    // Secret value.
                    Text {
                        id: secretValueLabel
                        Layout.row: 3
                        Layout.column: 0
                        horizontalAlignment: Text.AlignRight

                        text: qsTr("Secret Value : ")
                    }

                    Rectangle {
                        // Put a line around the text entry area.
                        Layout.row: 3
                        Layout.column: 1
                        Layout.fillWidth: true
                        height: secretValueLabel.height * 2
                        color: "transparent"

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
                            text: ""
                        }
                    }

                    // Digit count
                    Text {
                        id: digitCountLabel
                        Layout.row: 4
                        Layout.column: 0
                        horizontalAlignment: Text.AlignRight

                        text: qsTr("Digit Count : ")
                    }

                    TextMetrics {
                        id: hiddenNumbers
                        text: "8"
                    }

                    ComboBox {
                        id: numberCountComboBox
                        Layout.row: 4
                        Layout.column: 1
                        Layout.fillWidth: true
                        //height: digitCountLabel.height + 2
                        height: {
                            console.log("Height : " + hiddenNumbers.height);
                            return hiddenNumbers.height;
                        }

                        textRole: "digits"

                        model: digitsModel
                    }

                    ListModel {
                        id: digitsModel

                        ListElement {
                            digits: "6"
                        }

                        ListElement {
                            digits: "7"
                        }

                        ListElement {
                            digits: "8"
                        }
                    }

                    // XXX Add an "advanced" screen to allow setting the offset and time step?
                    // XXX Add an option to select the hashing algorithm.
                }

                HorizontalPadding {
                    size: 20
                }
            }

            VerticalPadding {
                size: 5
            }

            RowLayout {
                Rectangle {
                    Layout.fillWidth: true
                    color: "transparent"
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
                    color: "transparent"
                }
            }

            FillEmptySpace {
            }

            PaddedRowLine {

            }

            RowLayout {
                Rectangle {
                    Layout.fillWidth: true
                    color: "transparent"
                }

                Button {
                    id: saveButton

                    text: qsTr("Save")  // XXX Add code to check if we should be able to save, and to enable the save button when it is valid, and disable it when it isn't.

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("Saving key entries...");
                            var keyType, otpType, numberCount;
                            var errorSet = false;

                            // Convert the keyType index to the currect value.
                            switch (otpTypeComboBox.currentIndex) {
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
                            switch (secretValueTypeComboBox.currentIndex) {
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

                            if (editing) {
                                if (!InterfaceSingleton.updateKeyEntry(siteNameInput.text, secretValueInput.text, keyType, otpType, numberCount)) {
                                    if (!errorSet) {
                                        errorText.text = qsTr("Unable to update the key entry.  Please be sure that values a provided for all of the settings above.");

                                        // Create a timer to make the error text disappear after a few seconds.
                                        Qt.createQmlObject("import QtQuick 2.0; Timer { interval: 3000; running: true; repeat: false; onTriggered: errorText.visible = false; }", parent, "timer");
                                        console.info("Timer set..");
                                    }

                                    InterfaceSingleton.logError("Failed to update the key data!  Error : " + errorText.text);

                                    // Show the text.
                                    errorText.visible = true;
                                    return;     // Don't close the window.
                                }

                                // XXX update the database, rather than adding a key entry.
                            } else {
                                if (!InterfaceSingleton.addKeyEntry(siteNameInput.text, secretValueInput.text, keyType, otpType, numberCount)) {
                                    if (!errorSet) {
                                        errorText.text = qsTr("Unable to save the key entry.  Please be sure that values are provided for all of the settings above.");

                                        // Create a timer to make the error text disappear after a few seconds.
                                        Qt.createQmlObject("import QtQuick 2.0; Timer { interval: 3000; running: true; repeat: false; onTriggered: errorText.visible = false; }", parent, "timer");
                                        console.info("Timer set..");
                                    }

                                    InterfaceSingleton.logError("Failed to save the key data!  Error : " + errorText.text);

                                    // Show the text.
                                    errorText.visible = true;
                                    return;     // Don't close the window.
                                }
                            }

                            // Close the window.
                            screenStack.pop();
                        }
                    }
                }

                Rectangle {
                    width: 10
                    color: "transparent"
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
                    color: "transparent"
                }
            }

            // Take up all the extra space.
            Rectangle {
                height: 10
                Layout.fillWidth: true
                color: "transparent"
            }
        }
    }
}
