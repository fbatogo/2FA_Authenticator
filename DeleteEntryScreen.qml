import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import Rollin.InterfaceSingleton 1.0

import "resources/javascript/EntryScreenTools.js" as EntryScreenTools


Item {
    Component.onCompleted: {
        // Change the icon on the toolbar to be the back button.
        menuButton.source = "resources/back.svg";

        EntryScreenTools.populateKeyItemModel(toDeleteModel);
    }

    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            height: 10
            color: "transparent"
        }

        RowLayout {
            Rectangle {
                width: 15
                color: "transparent"
            }

            Text {
                text: qsTr("Please select the OTPs to delete : ");
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
                width: 5
                color: "transparent"
            }

            ListView {
                id: itemsToDelete

                Layout.fillWidth: true
                Layout.fillHeight: true

                model: toDeleteModel
                delegate: itemsToDeleteDelegate
                clip: true
            }

            Component {
                id: itemsToDeleteDelegate

                CheckBox {
                    id: checkbox
                    tristate: false
                    checked: {
                        if ((itemChecked === null) || (itemChecked === "undefined")) {
                            // Default to false.
                            return false;
                        }

                        return itemChecked;
                    }
                    text: identifier
                    onToggled: itemChecked = checkbox.checked
                }
            }

            ListModel {
                id: toDeleteModel
            }

            Rectangle {
                width: 5
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
            Layout.fillWidth: true

            Rectangle {
                Layout.fillWidth: true
                color: "transparent"
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
                            EntryScreenTools.populateKeyItemModel(toDeleteModel);
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
                Layout.fillWidth: true
                color: "transparent"
            }
        }

        Rectangle {
            height: 10
            color: "transparent"
        }
    }
}
