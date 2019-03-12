import QtQuick 2.0
import Rollin.InterfaceSingleton 1.0
import QtQuick.Layouts 1.3
import UiClipboard 1.0
import Rollin.SettingsHandler 1.0

import "resources/javascript/SecretScreen.js" as SecretScreenImpl;
import "resources/javascript/utils.js" as Utils
import "resources/widgets/"

Component {
    Item {
        Component.onCompleted: {
            SecretScreenImpl.populateListModel(otpListModel);

            // If we don't have anything in our list model, show the StartHereScreen.
            if (otpListModel.count <= 0) {
                console.log("No entries exist in the database.  Showing the 'StartHereScreen'.");
                screenStack.push(showStartHereScreen);
            }

            // Start the timer.
            updateTimer.start();
        }

        Connections {
            target: window
            onUpdateOtpData: SecretScreenImpl.updateScreen(otpListModel);
        }

        UiClipboard {
            id: clipboard
        }

        onVisibleChanged: {
            if (visible) {
                SecretScreenImpl.updateScreen(otpListModel);
            }
        }

        Timer {
            id: updateTimer

            // Tick once a second, and update all of the timer graphics, along with
            // updating the codes when the timers hit 0.
            interval: 1000
            repeat: true

            onTriggered: SecretScreenImpl.updateEntryState(otpListModel);
        }

        ListView {
            id: otpList

            width: parent.width
            height: parent.height
            model: otpListModel
            delegate: otpListDelegate
            clip: true
            spacing: 0
        }

        Component {
            id: otpListDelegate

            ColumnLayout {
                anchors.left: parent.left
                anchors.right: parent.right

                // Show the normal column widget.
                RowLayout {
                    id: entryRow
                    visible: !showError

                    Layout.fillWidth: true
                    height: keyColumn.height

                    Column {
                        id: keyColumn
                        visible: !showError
                        Layout.fillWidth: true
                        height: identifierText.height + identifierText.anchors.topMargin + otpNumberLabel.height + otpNumberLabel.anchors.topMargin

                        Text {
                            id: identifierText
                            width: parent.width

                            visible: !showError

                            // The name of the site the key is for.
                            x: 5
                            y: 5
                            text: identifier
                            font.bold: true
                            font.pointSize: 14
                        }

                        // Show the issue, if enabled.
                        Text {
                            id: issuer
                            width: parent.width
                            x: 25

                            visible: ((!showError) && (SettingsHandler.showIssuer()))

                            text: {
                                if (issuerText === "") {
                                    return qsTr("Issuer : <Not Provided>");
                                }

                                return qsTr("Issuer : %1").arg(issuerText);
                            }
                        }

                        // Show the HOTP counter, if enabled.
                        Text {
                            id: hotpCounterLabel
                            width: parent.width
                            x: 25

                            visible: ((!showError) && (otpType === 1) && (SettingsHandler.showHotpCounterValue()))

                            text: {
                                if (hotpCounter < 0) {
                                    return qsTr("Invalid HOTP Counter value!");
                                }

                                return qsTr("HOTP Counter Value : %1").arg(hotpCounter);
                            }
                        }

                        // Show the hash method used, if enabled.
                        Text {
                            id: hashUsedLabel
                            width: parent.width
                            x: 25

                            visible: ((!showError) && (SettingsHandler.showHashAlgorithm()))

                            text: qsTr("Hash Type Used : %1").arg(Utils.hashAlgIntToString(algorithm));
                        }

                        Text {
                            id: otpNumberLabel
                            width: parent.width

                            visible: !showError

                            text: otpCode
                            x: 25
                            color: "blue"
                            font.pointSize: 32
                            font.bold: true
                        }
                    }

                    // Show the clock icon.
                    Rectangle {
                        id: clockFrame
                        visible: (otpType !== 1)

                        width: keyColumn.height
                        height: keyColumn.height

                        ProgressCircle {
                            id: timer
                            x: 5
                            y: 5
                            size: clockFrame.width - 10
                            arcBegin: 0
                            arcEnd: (360 - circleShown)
                        }
                    }

                    // Show a refresh button for HOTP values.
                    Rectangle {
                        id: hotpRefreshButton

                        visible: (otpType === 1)

                        width: keyColumn.height
                        height: keyColumn.height

                        Image {
                            x: 5
                            y: 5
                            source: "resources/refresh.svg"
                            sourceSize.height: keyColumn.height - 20
                            sourceSize.width: keyColumn.height - 20
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                console.log("Getting new value.");

                                // Increment the counter value.
                                InterfaceSingleton.incrementHotpCounter(identifier);

                                // Force update the display.
                                SecretScreenImpl.populateListModel(otpListModel);
                            }
                        }
                    }

                    // Show a copy button to copy the current value.
                    Rectangle {
                        id: copyButton

                        width: keyColumn.height
                        height: keyColumn.height

                        Image {
                            x: 5
                            y: 5
                            source: "resources/copy.svg"
                            sourceSize.height: keyColumn.height - 10
                            sourceSize.width: keyColumn.height - 10
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                console.log("Copied to the clipboard.");
                                clipboard.setText(otpNumberLabel.text.replace(/\s+/g, ''));
                            }
                        }
                    }

                    HorizontalPadding {
                        size: 10
                    }
                }

                // Error version of the widget.
                RowLayout {
                    id: errorEntryRow
                    visible: showError

                    Layout.fillWidth: true
                    height: keyColumn.height

                    Column {
                        id: errorContainerFrame
                        Layout.fillWidth: true
                        y: 5
                        height: errorIdentifierText.height + errorLabel.height + 5 + 5
                        visible: showError

                        Text {
                            id: errorIdentifierText
                            width: parent.width
                            y: 5
                            x: 5
                            visible: showError

                            // The name of the site the key is for.
                            text: identifier
                            font.bold: true
                            font.pointSize: 14
                        }


                        Text {
                            id: errorLabel
                            width: parent.width
                            x: 25

                            visible: showError

                            text: errorText
                            color: "red"
                            font.pointSize: 16
                        }
                    }
                }

                Rectangle {
                    id: bottomLine
                    Layout.fillWidth: true
                    height: 1
                    color: "black"
                }
            }
        }

        ListModel {
            id: otpListModel
        }
    }
}
