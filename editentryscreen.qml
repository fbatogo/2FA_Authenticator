import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import InterfaceSingleton 1.0

Item {
    id: editEntryScreen

    Component.onCompleted: {
        // XXX Populate the list of keys we can edit.
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
                text: qsTr("Please select the OTP to edit : ");
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

                        Text {
                            id: toEditText
                            text: identifier
                        }

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
                }

                ListModel {
                    id: toEditModel
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
