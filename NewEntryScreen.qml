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
                    rows: 4

                    Text {
                        Layout.row: 0
                        Layout.column: 0
                        horizontalAlignment: Text.AlignRight

                        text: "Site Name : "
                    }

                    Rectangle {
                        Layout.row: 0
                        Layout.column: 1
                        height: otpTypeInput.height
                        Layout.fillWidth: true

                        border.color: "black"
                        border.width: 1

                        TextInput {
                            id: siteNameInput
                            anchors.fill: parent
                            anchors.topMargin: 3
                            anchors.bottomMargin: 3
                            anchors.leftMargin: 3
                            anchors.rightMargin: 3
                            font.pixelSize: height - 6
                            anchors.centerIn: parent
                            clip: true
                        }
                    }

                    Text {
                        Layout.row: 1
                        Layout.column: 0
                        horizontalAlignment: Text.AlignRight

                        text: qsTr("Secret Value : ")
                    }

                    Rectangle {
                        // Put a line around the text entry area.
                        //height: secretValueInput.height + 5
                        Layout.row: 1
                        Layout.column: 1
                        Layout.fillWidth: true
                        height: otpTypeInput.height

                        border.color: "black"
                        border.width: 1

                        TextInput {
                            id: secretValueInput
                            anchors.fill: parent
                            anchors.topMargin: 3
                            anchors.bottomMargin: 3
                            anchors.leftMargin: 3
                            anchors.rightMargin: 3
                            font.pixelSize: height - 6
                            anchors.centerIn: parent
                            clip: true
                        }
                    }

                    Text {
                        Layout.row: 2
                        Layout.column: 0
                        horizontalAlignment: Text.AlignRight

                        text: qsTr("OTP Type : ")
                    }

                    ComboBox {
                        id: otpTypeInput
                        Layout.row: 2
                        Layout.column: 1
                        Layout.fillWidth: true

                        model: ["TOTP", "HOTP"]
                    }

                    Text {
                        Layout.row: 3
                        Layout.column: 0
                        horizontalAlignment: Text.AlignRight

                        text: qsTr("Secret Format : ")
                    }

                    ComboBox {
                        id: scretFormatInput
                        Layout.row: 3
                        Layout.column: 1
                        Layout.fillWidth: true

                        model: ["Base32", "HEX"]
                    }
                }

                Rectangle {
                    width: 50
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

                    text: qsTr("Acquire from camera")
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
