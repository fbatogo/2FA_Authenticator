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
            font.pointSize: 12
            font.bold: true
        }
    }

    Drawer {
        id: drawer

        //y: topToolbar.height
        width: window.width / 2
        height: window.height // - topToolbar.height
        interactive: true

        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.info("Drawer clicked.");
                if (drawer.opened) {
                    drawer.close();
                }
            }
        }
    }

    ScrollView {
        anchors.fill: parent
        anchors.topMargin: topToolbar.height

        ListView {
            width: parent.width
            //model: 20
            model: otpListModel
            delegate: otpListDelegate
            /*
            delegate: ItemDelegate {
                text: "Item " + (index + 1)
                width: parent.width
            }*/
        }

        Component {
            id: otpListDelegate

            Item {
                width: parent.width
                height: 50

                Row {
                    width: parent.width
                    height: keyContainerFrame.height

                    Rectangle {
                        id: keyContainerFrame
                        width: parent.width
                        height: rowColumnItem.height

                        border.color: "red"
                        border.width: 2

                    Column {
                        Item {
                            id: rowColumnItem
                            width: parent.width
                            height: identifierText.height + otpNumberLabel.height

                            FontMetrics {
                                id: identifierCodeMetrics
                                font.pointSize: 12
                            }

                            Text {
                                width: identifierCodeMetrics.tightBoundingRect(identifier).width
                                height: identifierCodeMetrics.tightBoundingRect(identifier).height

                                // The name of the site the key is for.
                                anchors.top: rowColumnItem.top
                                anchors.topMargin: 5
                                anchors.left: rowColumnItem.left
                                anchors.leftMargin: 5
                                id: identifierText
                                text: identifier
                                font.pointSize: 12
                            }

                            FontMetrics {
                                id: otpCodeMetrics
                                font.bold: true
                                font.pointSize: 32
                            }

                            Text {
                                id: otpNumberLabel
                                width: otpCodeMetrics.tightBoundingRect(otpCode).width
                                height: {
                                    console.log("Width : " + otpCodeMetrics.tightBoundingRect(otpCode).height);
                                    return otpCodeMetrics.tightBoundingRect(otpCode).height
                                }

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
        }
    }
}
