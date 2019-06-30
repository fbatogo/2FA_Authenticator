import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtMultimedia 5.11
import Rollin.QRCodeSingleton 1.0

import "javascript/NewOrEditEntryScreen.js" as NewOrEdit
import "javascript/utils.js" as Utils
import "widgets/"

Item {
    id: newEntryScreen

    property bool editing: false
    property string identifier: ""

    Component.onCompleted: NewOrEdit.init();

    onActiveFocusChanged: {
        NewOrEdit.onActiveFocusChanged(activeFocus);

        if (activeFocus) {
            // The lines below will cause the widget to lose active focus, but we
            // will end up with focus where we want it instead.
            siteNameInput.focus = true;
            siteNameInput.forceActiveFocus();
        }
    }

    ToastManager {
        id: toast
    }

    // Create the "Select Algorithm" screen that will be shown if the user clicks the ... button next to the algorithm text.
    Component {
        id: selectAlgorithm

        SelectAlgorithm {
            id: alg

            onSelectedChanged: algorithmValue.text = Utils.hashAlgIntToString(selected);
        }
    }

    // Create the "Select OTP type" screen that will be shown if the user clicks the ... button next to the OTP type text.
    Component {
        id: selectOtpType

        OtpTypeSelect {
            id: otpTypeSelect

            onSelectedChanged: otpTypeText.text = Utils.otpTypeIntToString(selected);
        }
    }

    // Create the "Select secret encoding type" screen that will be shown if the user clicks the ... button next to the secret encoding text.
    Component {
        id: selectSecretFormatScreen

        SelectSecretFormat {
            id: secretFormatSelect

            onSelectedChanged: secretValueText.text = Utils.secretTypeIntToString(selected);
        }
    }

    // Create the "Select number of digits" screen that will be shown if the user clicks the ... button next to the number of digits text.
    Component {
        id: selectNumberOfDigitsScreen

        SelectDigitLength {
            id: digitLengthSelect

            onSelectedChanged: digitCountValue.text = (selected + 6);
        }
    }

    // Create the "set the time step" screen that will be shown if the user clicks the ... button next to the time step entry
    Component {
        id: selectTimeStepScreen

        OtpTimeInput {
            id: timeStepSelect

            promptText: qsTr("Select the period that an OTP is valid for : ");

            onSelectedChanged: periodValue.text = selected;
        }
    }

    // Create the "set the timestamp offset" screen that will be shown if the user clicks the ... button next to the time offset entry.
    Component {
        id: selectTimeOffsetScreen

        OtpTimeInput {
            id: timeOffsetSelect

            promptText: qsTr("Select time offset that the OTP should use : ");

            onSelectedChanged: offsetValue.text = selected;
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "white"

        ColumnLayout {
            anchors.fill: parent

            VerticalPadding {
                size: 2
            }

            SubHeader {
                headerText: {
                    if (!editing) {
                        return qsTr("Add a new site with secret key.");
                    }

                    return qsTr("Edit information for a site with a secret key.");
                }
            }

            RowLayout {
                HorizontalPadding {
                    size: 5
                }

                GridLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    columns: 3
                    rows: 7

                    // Site name/label value
                    Text {
                        id: siteNameLabel
                        Layout.row: 0
                        Layout.column: 0
                        Layout.alignment: Qt.AlignRight

                        text: qsTr("Site Name : ")
                    }

                    BoxedTextInput {
                        id: siteNameInput
                        Layout.row: 0
                        Layout.column: 1
                        Layout.columnSpan: 2
                        totalHeight: siteNameLabel.height + 8
                        Layout.fillWidth: true
                        focus: true

                        boxText: ""

                        KeyNavigation.tab: secretValueInput

                        onEditingComplete: NewOrEdit.checkEnableSave();
                    }

                    // Secret value.
                    Text {
                        id: secretValueLabel
                        Layout.row: 1
                        Layout.column: 0
                        Layout.alignment: Qt.AlignRight

                        text: qsTr("Secret : ")
                    }

                    BoxedTextInput {
                        id: secretValueInput
                        Layout.row: 1
                        Layout.column: 1
                        Layout.columnSpan: 2
                        totalHeight: siteNameLabel.height + 8
                        Layout.fillWidth: true

                        boxText: ""

                        KeyNavigation.tab: otpTypeButton

                        onEditingComplete: NewOrEdit.checkEnableSave();
                    }

                    // OTP Type row
                    Text {
                        id: otpTypeLabel
                        Layout.row: 2
                        Layout.column: 0
                        Layout.alignment: Qt.AlignRight

                        text: qsTr("Type : ")
                    }

                    Text {
                        Layout.row: 2
                        Layout.column: 1
                        Layout.fillWidth: true
                        id: otpTypeText
                        text: "Invalid"
                    }

                    EditItemButton {
                        id: otpTypeButton
                        row: 2
                        column: 2
                        showScreen: selectOtpType
                        selected: Utils.otpTypeToInt(otpTypeText.text)

                        KeyNavigation.tab: secretFormatButton
                    }

                    TextMetrics {
                        id: threeDots
                        text: "..."
                    }

                    // Secret value format
                    Text {
                        id: secretValueTypeLabel
                        Layout.row: 3
                        Layout.column: 0
                        Layout.alignment: Qt.AlignRight

                        text: qsTr("Secret Format : ")
                    }

                    Text {
                        id:secretValueText
                        Layout.row: 3
                        Layout.column: 1
                        Layout.fillWidth: true
                        text: "Invalid"
                    }

                    EditItemButton {
                        id: secretFormatButton
                        row: 3
                        column: 2
                        showScreen: selectSecretFormatScreen
                        selected: Utils.secretTypeToInt(secretValueText.text)

                        KeyNavigation.tab: digitCountButton
                    }

                    // Digit count
                    Text {
                        id: digitCountLabel
                        Layout.row: 4
                        Layout.column: 0
                        Layout.alignment: Qt.AlignRight

                        text: qsTr("Digits : ")
                    }

                    Text {
                        id: digitCountValue
                        Layout.row: 4
                        Layout.column: 1
                        text: "Invalid"
                        Layout.fillWidth: true
                    }

                    EditItemButton {
                        id: digitCountButton
                        row: 4
                        column: 2
                        showScreen: selectNumberOfDigitsScreen
                        selected: digitCountValue.text - 6

                        KeyNavigation.tab: algorithmButton
                    }

                    // Algorithm
                    Text {
                        id: algorithmLabel
                        Layout.row: 5
                        Layout.column: 0
                        Layout.alignment: Qt.AlignRight

                        text: qsTr("Algorithm : ")
                    }

                    Text {
                        id: algorithmValue
                        Layout.row: 5
                        Layout.column: 1
                        text: "Invalid"
                        Layout.fillWidth: true
                    }

                    EditItemButton {
                        id: algorithmButton
                        row: 5
                        column: 2
                        showScreen: selectAlgorithm
                        selected: Utils.hashAlgToInt(algorithmValue.text)

                        KeyNavigation.tab: periodButton
                    }

                    // Period
                    Text {
                        id: periodLabel
                        Layout.row: 6
                        Layout.column: 0
                        Layout.alignment: Qt.AlignRight

                        text: qsTr("Period : ")
                    }

                    Text {
                        id: periodValue
                        Layout.row: 6
                        Layout.column: 1
                        text: "Invalid"
                        Layout.fillWidth: true
                    }

                    EditItemButton {
                        id: periodButton
                        row: 6
                        column: 2
                        showScreen: selectTimeStepScreen
                        selected: periodValue.text

                        KeyNavigation.tab: offsetButton
                    }

                    // Offset
                    Text {
                        id: offsetLabel
                        Layout.row: 7
                        Layout.column: 0
                        Layout.alignment: Qt.AlignRight

                        text: qsTr("Offset : ")
                    }

                    Text {
                        id: offsetValue
                        Layout.row: 7
                        Layout.column: 1
                        text: "Invalid"
                        Layout.fillWidth: true
                    }

                    EditItemButton {
                        id: offsetButton
                        row: 7
                        column: 2
                        showScreen: selectTimeOffsetScreen
                        selected: offsetValue.text

                        KeyNavigation.tab: siteNameInput
                    }
                }

                HorizontalPadding {
                    size: 5
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

                    text: qsTr("Save")

                    MouseArea {
                        anchors.fill: parent
                        onClicked: NewOrEdit.saveConfiguration();
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

            VerticalPadding {
                size: 5
            }
        }
    }
}
