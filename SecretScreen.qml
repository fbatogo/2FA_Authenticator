import QtQuick 2.0
import InterfaceSingleton 1.0

Item {
    Component.onCompleted: {
        console.log("SecretScreen is ready!");

        populateListModel();

        // Start the timer.
        updateTimer.start();
    }

    Timer {
        id: updateTimer

        // Tick once a second, and update all of the timer graphics, along with
        // updating the codes when the timers hit 0.
        interval: 1000
        repeat: true

        onTriggered: {
            var updatedOtps = false;

            console.info("Tick!");

            // Increment the clock image.
            for(var i = 0; i < otpListModel.count; i++) {
                var listItem = otpListModel.get(i);

                listItem.currentTimer++;

                if (listItem.currentTimer > listItem.timeStep) {
                    // Reset the timer to 0.
                    listItem.currentTimer = 0;

                    if (!updatedOtps) {
                        console.log("Get all OTP values...");
                        updatedOtps = true;

                        populateListModel();
                    }
                }

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

    // Populate the list model.
    function populateListModel() {
        var otpEntryList = InterfaceSingleton.otpEntries();

        // Make sure the list model is empty before we start to populate it.
        otpListModel.clear();

        // Iterate each entry, and add it to the list.
        for (var i = 0; i < otpEntryList.count(); i++) {
            var temp = otpEntryList.at(i);

            otpListModel.append({ timeStep: temp.mTimeStep, identifier: temp.mIdentifier, otpCode: temp.mCurrentCode, currentTimer: temp.mStartTime,  circleShown: 0 });
        }
    }

    ListView {
        id: otpList

        width: parent.width
        height: parent.height
        model: otpListModel
        delegate: otpListDelegate
    }

    Component {
        id: otpListDelegate

        Item {
            width: parent.width
            height: entryRow.height

            Row {
                id: entryRow

                width: parent.width
                height: keyContainerFrame.height

                Rectangle {
                    id: keyContainerFrame
                    width: parent.width - clockFrame.width
                    height: rowColumnItem.height

                    Column {
                        Item {
                            id: rowColumnItem
                            width: parent.width
                            height: identifierText.height + identifierText.anchors.topMargin + otpNumberLabel.height + otpNumberLabel.anchors.topMargin

                            Text {
                                id: identifierText
                                width: parent.width

                                // The name of the site the key is for.
                                anchors.top: rowColumnItem.top
                                anchors.topMargin: 5
                                anchors.left: rowColumnItem.left
                                anchors.leftMargin: 5
                                text: identifier
                                font.pointSize: 12
                            }


                            Text {
                                id: otpNumberLabel
                                width: parent.width

                                anchors.top: identifierText.bottom
                                anchors.topMargin: 5
                                anchors.left: identifierText.left
                                anchors.leftMargin: 25     // Move it in 10% of the width of the item space.
                                text: otpCode
                                color: "blue"
                                font.pointSize: 32
                                font.bold: true
                            }
                        }
                    }
                }

                // Show the clock icon.
                Rectangle {
                    id: clockFrame

                    width: keyContainerFrame.height
                    height: keyContainerFrame.height

                    Item {
                        anchors.fill: parent
                        anchors.topMargin: 10
                        anchors.leftMargin: 10

                        ProgressCircle {
                            id: timer
                            size: clockFrame.width - 20
                            arcBegin: 0
                            arcEnd: circleShown
                        }
                    }
                    /*                    Image {
                        source: "resources/quarter-of-an-hour.svg"
                        width: clockFrame.width * 0.8
                        height: clockFrame.height * 0.8
                        anchors.centerIn: parent
                    }*/

                }
            }
        }
    }

    ListModel {
        id: otpListModel
    }
    /*
        ListElement {
            identifier: "2FA Secured Website"; otpCode: 123456; currentTimer: 1; timeStep: 30; circleShown: 0
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321; currentTimer: 10; timeStep: 30; circleShown: 0
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321; currentTimer: 10; timeStep: 30; circleShown: 0
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321; currentTimer: 10; timeStep: 30; circleShown: 0
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321; currentTimer: 10; timeStep: 30; circleShown: 0
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321; currentTimer: 10; timeStep: 30; circleShown: 0
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321; currentTimer: 10; timeStep: 30; circleShown: 0
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321; currentTimer: 10; timeStep: 30; circleShown: 0
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321; currentTimer: 10; timeStep: 30; circleShown: 0
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321; currentTimer: 10; timeStep: 30; circleShown: 0
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321; currentTimer: 10; timeStep: 30; circleShown: 0
        }

    }*/
}
