import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import Rollin.GeneralInfoSingleton 1.0

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

            HorizontalPadding {
                size: 10
            }

            RowLayout {
                Rectangle {
                    Layout.fillWidth: true
                }

                Text {
                    width: parent.width
                    horizontalAlignment: Text.Center
                    text: qsTr("Version %1").arg(GeneralInfoSingleton.version())
                }

                Rectangle {
                    Layout.fillWidth: true
                }
            }

            VerticalPadding {
                size: 5
            }

            RowLayout {
                Rectangle {
                    Layout.fillWidth: true
                }

                Text {
                    width: parent.width
                    horizontalAlignment: Text.Center
                    text: qsTr("(c) Copyright 2019, %1").arg("Chris Hessing")
                }

                Rectangle {
                    Layout.fillWidth: true
                }
            }

            VerticalPadding {
                size: 10
            }

            RowLayout {
                HorizontalPadding {
                    size: 10
                }

                Text {
                    width: parent.width
                    text: qsTr("Licenses")
                }

                Rectangle {
                    Layout.fillWidth: true
                }
            }

            RowLayout {
                HorizontalPadding {
                    size: 10
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    border.color: "black"
                    border.width: 1

                    Flickable {
                        anchors.fill: parent

                        flickableDirection: Flickable.VerticalFlick

                        TextArea.flickable: TextArea {
                            id: licenseText
                            readOnly: true
                            textFormat: TextEdit.RichText
                            wrapMode: TextArea.Wrap
                            text: GeneralInfoSingleton.licenseText()
                        }

                        ScrollBar.vertical: ScrollBar { }
                    }
                }

                HorizontalPadding {
                    size: 10
                }
            }

            VerticalPadding {
                size: 10
            }
        }
    }
}
