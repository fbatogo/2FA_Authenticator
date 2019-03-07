import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import Rollin.SettingsHandler 1.0

import "resources/widgets/"

Item {
    Component.onCompleted: {
        // Change the icon on the toolbar to be the back button.
        menuButton.source = "resources/back.svg";
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
                }
            }

            FillEmptySpace {
            }
        }
    }
}
