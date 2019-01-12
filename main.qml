import QtQuick 2.11
import QtQuick.Controls 2.2

ApplicationWindow {
    id: window

    visible: true
    width: 640
    height: 480
    title: qsTr("2 Factor Authenticator")

    ToolBar {
        id: topToolbar

        z: 1        // Make sure this is on top.
        width: parent.width
        height: 0.05 * parent.height

        Image {
            source: "resources/menubutton.png"
            anchors.leftMargin: 5
            anchors.topMargin: 5
            width: topToolbar.height
            height: topToolbar.height

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.debug("Clicked hamburger.");
                    if (!drawer.opened) {
                        drawer.open()
                    }
                }
            }
        }

        Label {
            id: topToolbarTitle

            anchors.centerIn: parent
            text: "2 Factor Authenticator"
            font.bold: true
        }
    }

    Drawer {
        id: drawer

        width: window.width / 2
        height: window.height // - topToolbar.height
        interactive: true

        ListView {
            width: parent.width
            height: parent.height
            model: drawerModel
            delegate: drawerDelegate
            spacing: 0
        }

        Component {
            id: drawerDelegate

            Item {
                width: parent.width
                height: itemRow.height

                Row {
                    id: itemRow
                    width: parent.width
                    height: itemColumn.height

                    Column {
                        id: itemColumn
                        width: parent.width
                        height: menuName.height + 2

                        Rectangle {
                            color: "black"
                            width: parent.width
                            height: 1
                        }

                        Text {
                            id: menuName
                            text: name
                            font.pointSize: 24
                        }

                        Rectangle {
                            color: "black"
                            width: parent.width
                            height: 1
                        }
                    }
                }
            }
        }

        ListModel {
            id: drawerModel

            ListElement {
                name: qsTr("New...")
            }

            ListElement {
                name: qsTr("Delete...")
            }

            ListElement {
                name: qsTr("About...")
            }
        }
        /*
        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.info("Drawer clicked.");
                if (drawer.opened) {
                    drawer.close();
                }
            }
        }
        */
    }

    ListView {
        width: parent.width
        height: parent.height - topToolbar.height
        anchors.top: topToolbar.bottom
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

                    Image {
                        source: "resources/quarter-of-an-hour.svg"
                        width: clockFrame.width * 0.8
                        height: clockFrame.height * 0.8
                        anchors.centerIn: parent
                    }

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
