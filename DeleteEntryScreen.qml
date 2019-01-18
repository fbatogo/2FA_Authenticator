import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import InterfaceSingleton 1.0

Item {
    Component.onCompleted: {
        console.log("DeleteEntryScreen is ready!");

        populateToDeleteList();
    }

    function populateToDeleteList() {
        var keys = InterfaceSingleton.keyEntries();

        // Make sure the list is empty.
        toDeleteModel.clear();

        for (var i = 0; i < keys.count(); i++) {
            toDeleteModel.append({ identifier: keys.at(i).mIdentifier, itemChecked: false });
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
                text: qsTr("Please select the OTPs to delete : ");
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
                    id: itemsToDelete

                    anchors.fill: parent
                    model: toDeleteModel
                    delegate: itemsToDeleteDelegate
                    clip: true
                }

                Component {
                    id: itemsToDeleteDelegate

                    CheckBox {
                        id: checkbox
                        tristate: false
                        checked: itemChecked
                        text: identifier
                        onToggled: itemChecked = checkbox.checked
                    }
                }

                ListModel {
                    id: toDeleteModel
                }
            }

            Rectangle {
                width: 10
            }
        }

        RowLayout {
            Layout.fillWidth: true

            Rectangle {
                Layout.fillWidth: true
            }

            Button {
                id: deleteButton
                text: qsTr("Delete Selected");

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        var deleted = false;            // Have we deleted something, or not?
                        var error = false;

                        for (var i = 0; i < toDeleteModel.count; i++) {
                            if (toDeleteModel.get(i).itemChecked) {
                                if (!InterfaceSingleton.deleteKey(toDeleteModel.get(i).identifier)) {
                                    console.log("Unable to delete key with the identifier : " + toDeleteModel.get(i).identifier);
                                    error = true;
                                } else {
                                    deleted = true;
                                }
                            }
                        }

                        // If there was an error deleting one of the keys, we want to stay on this screen, but update the list.
                        if (error) {
                            populateToDeleteList();
                            return;
                        }

                        // If we deleted something, we want to close this window.
                        if (deleted) {
                            screenStack.pop();
                            return;
                        }
                    }
                }
            }

            Rectangle {
                width: 10
            }

            Button {
                id: cancelButton
                text: qsTr("Cancel");

                MouseArea {
                    anchors.fill: parent

                    onClicked: screenStack.pop();
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
