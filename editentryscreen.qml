import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import InterfaceSingleton 1.0

Item {
    id: editEntryScreen

    Component.onCompleted: {
        // Change the icon on the toolbar to be the back button.
        menuButton.source = "resources/back.svg";

        // XXX Populate the list of keys we can edit.
        populateToEditList();
    }

    function populateToEditList() {
        var keys = InterfaceSingleton.keyEntries();

        // Make sure the list is empty.
        toEditModel.clear();

        for (var i = 0; i < keys.count(); i++) {
            toEditModel.append({ identifier: keys.at(i).mIdentifier });
        }
    }

    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            height: 10
            color: "transparent"
        }

        RowLayout {
            Rectangle {
                width: 10
                color: "transparent"
            }

            Text {
                text: qsTr("Please select the OTP entry to edit : ");
            }

            Rectangle {
                Layout.fillWidth: true
                color: "transparent"
            }
        }

        RowLayout {
            Rectangle {
                color: "transparent"
                width: 5
            }

            Rectangle {
                height: 1
                Layout.fillWidth: true
                color: "transparent"
                border.color: "grey"
                border.width: 1
            }

            Rectangle {
                color: "transparent"
                width: 5
            }
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Rectangle {
                width: 10
                color: "transparent"
            }

            ListView {
                id: itemToEdit

                Layout.fillWidth: true
                Layout.fillHeight: true

                model: toEditModel
                delegate: itemsToEditDelegate
                clip: true
            }

            Component {
                id: itemsToEditDelegate

                Text {
                    Layout.fillWidth: true
                    id: toEditText
                    text: identifier
                    font.pixelSize: 16

                    MouseArea {
                        anchors.fill: parent
                        z:1

                        onClicked: {
                            var error = false;
                            console.log("Item clicked! -- " + identifier);

                            // XXX Push the edit screen on the stack, and pass the identifier value that was clicked.
                        }
                    }
                }
            }

            ListModel {
                id: toEditModel
            }

            Rectangle {
                width: 10
                color: "transparent"
            }
        }

        RowLayout {
            Rectangle {
                color: "transparent"
                width: 5
            }

            Rectangle {
                height: 1
                Layout.fillWidth: true
                color: "transparent"
                border.color: "grey"
                border.width: 1
            }

            Rectangle {
                color: "transparent"
                width: 5
            }
        }

        Rectangle {
            height: 5
            color: "transparent"
        }
    }
}
