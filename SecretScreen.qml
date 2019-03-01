import QtQuick 2.0
import Rollin.InterfaceSingleton 1.0
import QtQuick.Layouts 1.3
import UiClipboard 1.0

Component {
    Item {
        Component.onCompleted: {
            populateListModel();

            // Start the timer.
            updateTimer.start();
        }

        UiClipboard {
            id: clipboard
        }

        onVisibleChanged: {
            if (visible) {
                // Make sure the hamburger is showing.
                menuButton.source = "resources/menu.svg";

                // Also update the list model.
                populateListModel();
            }
        }

        Timer {
            id: updateTimer

            // Tick once a second, and update all of the timer graphics, along with
            // updating the codes when the timers hit 0.
            interval: 1000
            repeat: true

            onTriggered: {
                var updatedOtps = false;

                // Increment the clock image.
                for(var i = 0; i < otpListModel.count; i++) {
                    var listItem = otpListModel.get(i);

                    if (!listItem.showError) {
                        listItem.currentTimer++;

                        if (listItem.currentTimer > listItem.timeStep) {
                            // Reset the timer to 0.
                            listItem.currentTimer = 0;

                            if (!updatedOtps) {
                                console.log("Get all OTP values...");
                                updatedOtps = true;

                                populateListModel();
                                return;
                            }
                        }

                        if (listItem.timeStep <= 0) {
                            console.error("Time step is <= 0.  Ignoring!");
                        } else {
                            if (listItem.currentTimer === 0) {
                                listItem.circleShown = 0;
                            } else {
                                // Calculate the percentage of time that has passed.
                                var timePercent = listItem.currentTimer / listItem.timeStep;

                                // Then, figure out how much of the circle to draw.
                                listItem.circleShown = 360 * timePercent;
                            }
                        }
                    }
                }
            }
        }

        // Populate the list model.
        function populateListModel() {
            var otpEntryList = InterfaceSingleton.otpEntries();

            // Make sure the list model is empty before we start to populate it.
            otpListModel.clear();

            // If the list is empty, don't try to populate it.
            if (otpEntryList === null) {
                // Nothing to do.
                return;
            }

            // Iterate each entry, and add it to the list.
            console.log("Iterating entries...");
            for (var i = 0; i < otpEntryList.count(); i++) {
                var temp = otpEntryList.at(i);
                var code;
                var parts;

                if (temp.mValid === true) {
                    switch (temp.mCurrentCode.length) {
                    case 6:
                        // Add a space between the first and 2nd 3 characters.
                        parts = temp.mCurrentCode.match(/.{1,3}/g);
                        code = parts[0] + " " + parts[1];
                        break;

                    case 7:
                        // Add a space between the first 4 and second 3 characters.
                        parts = temp.mCurrentCode.match(/.{1,4}/g);
                        code = parts[0] + " " + parts[1];
                        break;

                    case 8:
                        // Add a space between the first and 2nd 4 characters.
                        parts = temp.mCurrentCode.match(/.{1,4}/g);
                        code = parts[0] + " " + parts[1];
                        break;

                    default:
                        // We should only allow 6, 7, and 8, but if we get something else, just
                        // display the number with no spaces.
                        code = temp.mCurrentCode
                        break;
                    }

                    // Calculate the percentage of time that has passed.
                    var timePercent = temp.mStartTime / temp.mTimeStep;

                    otpListModel.append({ timeStep: temp.mTimeStep, identifier: temp.mIdentifier, otpCode: code, currentTimer: temp.mStartTime,  circleShown: (360 * timePercent), showError: false, errorText: "" });
                } else {
                    otpListModel.append({ identifier: temp.mIdentifier, otpCode: "", showError: true, errorText: temp.mInvalidReason });
                }
            }
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
                //            }
            }
        }

        ListModel {
            id: otpListModel
        }
    }
}
