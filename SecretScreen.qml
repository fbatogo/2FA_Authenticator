import QtQuick 2.0

Item {
//    id: secretScreen

    ListView {
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

                    border.color: "red"
                    border.width: 1

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
                            size: clockFrame.width - 20
                            colorCircle: "#779933"
                            colorBackground: "#E6E6E6"
                            showBackground: true
                            isPie: true
                            arcBegin: 90
                            arcEnd: 200
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

        ListElement {
            identifier: "2FA Secured Website"; otpCode: 123456;
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321;
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321;
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321;
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321;
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321;
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321;
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321;
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321;
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321;
        }

        ListElement {
            identifier: "Other 2FA Secured Website"; otpCode: 654321;
        }

    }
}
