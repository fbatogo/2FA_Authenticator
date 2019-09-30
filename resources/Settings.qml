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
                Layout.fillWidth: true
                size: 10
            }

            SubHeader {
                Layout.fillWidth: true
                headerText: qsTr("Settings");
            }

            VerticalPadding {
                Layout.fillWidth: true
                size: 10
            }

            RowLayout {
                Layout.fillWidth: true

                VerticalPadding {
                    size: 10
                }

                ColumnLayout {
                    Layout.fillWidth: true

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

                    // Button for changing the location of the database.
                    RowLayout {
                        Layout.fillWidth: true

                        Rectangle {
                            Layout.fillWidth: true
                        }

                        Button {
                            id: resetDatabaseLocation
                            text: qsTr("Change Database Location");

                            onClicked: {
                                // Set the path to the current folder.
                                databaseLocationFileDialog.folder = "file:///" + SettingsHandler.databaseLocation();

                                // Open the dialog.
                                databaseLocationFileDialog.open();
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
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
                if (!SettingsHandler.setDatabaseLocation(fileUrls[0])) {
                    Logger.logError("Failed to change the database location!");
                    // XXX Show an error dialog.
                }
            } else {
                Logger.logDebug("Not updating the database location, because it wasn't changed!");
            }
        }
        onRejected: { console.log("Rejected") }
    }

}
