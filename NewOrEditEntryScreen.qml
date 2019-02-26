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

    Component.onCompleted: NewOrEdit.init();

    onVisibleChanged: NewOrEdit.onVisibleChanged(visible);

    Rectangle {
        anchors.fill: parent
        color: "white"

        ColumnLayout {
            anchors.fill: parent

/*            VerticalPadding {
                size: 2
            } */

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

                    Rectangle {
                        Layout.row: 0
                        Layout.column: 1
                        Layout.columnSpan: 2
                        height: siteNameLabel.height + 8
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
                            anchors.centerIn: parent
                            clip: true

                            onEditingFinished: NewOrEdit.checkEnableSave();
                        }
                    }

                    // Secret value.
                    Text {
                        id: secretValueLabel
                        Layout.row: 1
                        Layout.column: 0
                        Layout.alignment: Qt.AlignRight

                        text: qsTr("Secret : ")
                    }

                    Rectangle {
                        // Put a line around the text entry area.
                        id: secretValueInputBox
                        Layout.row: 1
                        Layout.column: 1
                        Layout.columnSpan: 2
                        Layout.fillWidth: true
                        height: secretValueLabel.height + 8
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
                            anchors.centerIn: parent
                            clip: true
                            text: ""

                            onEditingFinished: NewOrEdit.checkEnableSave();
                        }
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

                    TextMetrics {
                        id: threeDots
                        text: "..."
                    }

                    Button {
                        id: otpTypeButton
                        Layout.row: 2
                        Layout.column: 2
                        Layout.maximumWidth: threeDots.width + 8
                        Layout.maximumHeight: threeDots.height + 8
                        height: threeDots.height + 8
                        width: threeDots.width + 8
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

                    Button {
                        id: secretTypeButton
                        Layout.row: 3
                        Layout.column: 2
                        Layout.maximumWidth: threeDots.width + 8
                        Layout.maximumHeight: threeDots.height + 8
                        height: threeDots.height + 8
                        width: threeDots.width + 8
                        text: "..."
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

                    Button {
                        id: digitCountButton
                        Layout.row: 4
                        Layout.column: 2
                        Layout.maximumWidth: threeDots.width + 8
                        Layout.maximumHeight: threeDots.height + 8
                        height: threeDots.height + 8
                        width: threeDots.width + 8
                        text: "..."
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
                    }

/*                    Button {
                        id: algorithmButton
                        Layout.row: 5
                        Layout.column: 2
                        Layout.maximumWidth: threeDots.width + 8
                        Layout.maximumHeight: threeDots.height + 8
                        height: threeDots.height + 8
                        width: threeDots.width + 8
                        text: "..."
                    }*/

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

                    Button {
                        id: periodButton
                        Layout.row: 6
                        Layout.column: 2
                        Layout.maximumWidth: threeDots.width + 8
                        Layout.maximumHeight: threeDots.height + 8
                        height: threeDots.height + 8
                        width: threeDots.width + 8
                        text: "..."
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

                    Button {
                        id: offsetButton
                        Layout.row: 7
                        Layout.column: 2
                        Layout.maximumWidth: threeDots.width + 8
                        Layout.maximumHeight: threeDots.height + 8
                        height: threeDots.height + 8
                        width: threeDots.width + 8
                        text: "..."
                    }
                }

                HorizontalPadding {
                    size: 5
                }
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
