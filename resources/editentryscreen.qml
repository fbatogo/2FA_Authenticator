import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

import "javascript/EntryScreenTools.js" as EntryScreenTools
import "widgets/"

Item {
    id: editEntryScreen

    Component.onCompleted: {
        // Change the icon on the toolbar to be the back button.
        menuButton.source = "/resources/images/back.svg";

        EntryScreenTools.populateKeyItemModel(toEditModel);
    }

    ColumnLayout {
        anchors.fill: parent

        VerticalPadding {
            size: 10
        }

        SubHeader {
            headerText: qsTr("Please select the OTP entry to edit : ")
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Rectangle {
                width: 10
                color: "transparent"
            }

            ListView {
                id: itemToEdit

                Layout.fillWidth: true
                Layout.fillHeight: true

                model: toEditModel
                delegate: itemsToEditDelegate
                clip: true
            }

            Component {
                id: itemsToEditDelegate

                Text {
                    Layout.fillWidth: true
                    id: toEditText
                    text: identifier
                    font.pixelSize: 16

                    MouseArea {
                        anchors.fill: parent
                        z:1

                        onClicked: screenStack.push(Qt.resolvedUrl("NewOrEditEntryScreen.qml"), {"editing" : true, "identifier" : identifier})
                    }
                }
            }

            ListModel {
                id: toEditModel
            }

            VerticalPadding {
                size: 10
            }
        }

        VerticalPadding {
            size: 5
        }
    }
}
