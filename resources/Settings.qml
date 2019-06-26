import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import Rollin.SettingsHandler 1.0
import Rollin.Logger 1.0 as Logger

import "widgets/"

Item {
    Component.onCompleted: {
        // Change the icon on the toolbar to be the back button.
        menuButton.source = "/resources/images/back.svg";
    }

    Rectangle {
        anchors.fill: parent
        color: "white"

        ColumnLayout {
            anchors.fill: parent

            VerticalPadding {
                size: 10
            }

            SubHeader {
                headerText: qsTr("Settings");
            }

            VerticalPadding {
                size: 10
            }

            RowLayout {

                VerticalPadding {
                    size: 10
                }

                ColumnLayout {
                    // List the configuration options that we allow.
                    CheckBox {
                        id: showIssuer
                        tristate: false
                        checked: SettingsHandler.showIssuer()
                        text: qsTr("Show Issuer")

                        onToggled: SettingsHandler.setShowIssuer(checked);
                    }

                    CheckBox {
                        id: showHotpCounter
                        tristate: false
                        checked: SettingsHandler.showHotpCounterValue();
                        text: qsTr("Show HOTP Counter Value")

                        onToggled: SettingsHandler.setShowHotpCounterValue(checked);
                    }

                    CheckBox {
                        id: showHashAlgorithm
                        tristate: false
                        checked: SettingsHandler.showHashAlgorithm();
                        text: qsTr("Show hash algorithm");

                        onToggled: SettingsHandler.setShowHashAlgorithm(checked);
                    }

                    CheckBox {
                        id: logToFile
                        tristate: false
                        checked: SettingsHandler.logToFile();
                        text: qsTr("Log to a file");

                        onToggled: SettingsHandler.setLogToFile(checked);
                    }

                    RowLayout {
                        Label {
                            text: qsTr("Database location : ");
                        }

                        TextInput {
                            id: databaseLocation
                            text: SettingsHandler.databaseLocation();
                        }

                        Button {
                            id: selectDatabaseLocation
                            text: qsTr("Select");

                            onClicked: {
                                // Set the path to the current folder.
                                databaseLocationFileDialog.folder = "file:///" + SettingsHandler.databaseLocation();

                                // Open the dialog.
                                databaseLocationFileDialog.open();
                            }
                        }

                        Button {
                            id: resetDatabaseLocation
                            text: qsTr("Reset");
                        }
                    }
                }
            }

            FillEmptySpace {
            }
        }
    }

    // XXX Change this screen to use the pencil to select a new path, similar to other
    // items in the NewOrEditEntryScreen.
    FileDialog {
        id: databaseLocationFileDialog
        visible: false
        modality: Qt.WindowModal
        title: qsTr("Select a folder");
        selectMultiple: false
        selectFolder: true
        nameFilters: [ "All files (*)" ]
        selectedNameFilter: "All files (*)"
        sidebarVisible: true
        onAccepted: {
            console.log("Accepted: " + fileUrls[0]);

            // Update the settings with the path, if it has changed.
            if (fileUrls !== SettingsHandler.databaseLocation()) {
                // XXX Close the database file, move it to the new location, reopen it, and update the location in our settings.

            } else {
                Logger.logDebug("Not updating the database location, because it wasn't changed!");
            }
        }
        onRejected: { console.log("Rejected") }
    }

}
