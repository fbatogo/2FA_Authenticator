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
        }

        RowLayout {
            Rectangle {
                width: 15
            }

            Text {
                text: qsTr("Please select the OTP entry to edit : ");
            }

            Rectangle {
                Layout.fillWidth: true
            }
        }

        Rectangle {
            height: 2
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Rectangle {
                width: 10
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true

                border.color: "black"
                border.width: 1

                ListView {
                    id: itemToEdit

                    anchors.fill: parent
                    model: toEditModel
                    delegate: itemsToEditDelegate
                    clip: true
                }

                Component {
                    id: itemsToEditDelegate

                    Rectangle {
                        anchors.fill: parent
                        anchors.leftMargin: 3
                        anchors.rightMargin: 3

                        Text {
                            anchors.fill: parent
                            id: toEditText
                            text: identifier
                            font.pixelSize: 10

                            MouseArea {
                                anchors.fill: parent
                                z:1

                                onClicked: {
                                    var error = false;
                                    console.log("Item clicked!");
        /*
                                    for (var i = 0; i < toDeleteModel.count; i++) {
                                        if (toDeleteModel.get(i).itemChecked) {
                                            if (!InterfaceSingleton.deleteKey(toDeleteModel.get(i).identifier)) {
                                                console.log("Unable to delete key with the identifier : " + toDeleteModel.get(i).identifier);
                                                error = true;
                                            } else {
                                                // XXX
                                            }
                                        }
                                    } */
                                }
                            }
                        }
                    }
                }

                ListModel {
                    id: toEditModel
                }
            }

            Rectangle {
                width: 10
            }
        }

        Rectangle {
            height: 10
        }
    }
}
