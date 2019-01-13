import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: window

    visible: true
    width: 640
    height: 480
    title: qsTr("2 Factor Authenticator")
    header: ToolBar {
        id: topToolbar

        width: parent.width
        height: topToolbarTitle.height + 10

        Image {
            id: menuButton
            anchors.leftMargin: 5
            source: "resources/menubutton.png"
            width: topToolbar.height
            height: topToolbar.height

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (!drawer.opened) {
                        drawer.open()
                    }
                }
            }
        }

        Text {
            id: topToolbarTitle

            anchors.centerIn: parent

            text: "2 Factor Authenticator"
            font.bold: true
            font.pointSize: 24
        }

        Drawer {
            id: drawer

            width: window.width / 2
            height: window.height
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
                            height: singleItem.height

                            Item {
                                id: singleItem
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

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        if (drawer.opened) {
                                            drawer.close();
                                        }

                                        // Figure out what was clicked based on the index.
                                        switch(index) {
                                        case 0:
                                            // New Key Entry screen.
                                            console.info("Ney key entry clicked...");
                                            screenStack.push(Qt.resolvedUrl("NewEntryScreen.qml"));
                                            break;

                                        case 1:
                                            // Delete Key Entry screen.
                                            console.info("Delete key entry clicked...");
                                            screenStack.push(Qt.resolvedUrl("DeleteEntryScreen.qml"));
                                            break;

                                        case 2:
                                            // Setting screen
                                            console.info("Settings entry clicked...");
                                            break;

                                        case 3:
                                            // About screen.
                                            console.info("About screen clicked...");
                                            screenStack.push(Qt.resolvedUrl("AboutScreen.qml"));
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            ListModel {
                id: drawerModel

                // If you change something here, be sure to change the indicies for onClicked above!
                ListElement {
                    name: qsTr("New")
                }

                ListElement {
                    name: qsTr("Delete")
                }

                ListElement {
                    name: qsTr("Settings")
                }

                ListElement {
                    name: qsTr("About")
                }
            }
        }
    }

    StackView {
        id: screenStack
        initialItem: secretScreen
        anchors.fill: parent

        SecretScreen {
            id: secretScreen
        }
    }
}

