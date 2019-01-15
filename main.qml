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

            width: window.width / 3
            height: window.height
            interactive: true

            background: Rectangle {
                color: "black"
            }

            ListView {
                width: parent.width - 2
                height: parent.height - 2
                anchors.centerIn: parent
                model: drawerModel
                delegate: drawerDelegate
                spacing: 1
                anchors.leftMargin: 1
                anchors.rightMargin: 0
                anchors.topMargin: 1
                anchors.bottomMargin: 1
            }

            Component {
                id: drawerDelegate

                Rectangle {
                    //anchors.centerIn: parent
                    width: parent.width //- 2
                    //height: parent.height - 2
                    height: menuName.height
                    color: "#f0f0f0"

                    Text {
                        id: menuName
                        anchors.leftMargin: 10
                        text: name
                        color: "black"
                        font.pointSize: 24
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
                                screenStack.push(Qt.resolvedUrl("Settings.qml"));
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

/*        SecretScreen {
            id: secretScreen
        }*/

        Settings {
            id: secretScreen
        }
    }
}

