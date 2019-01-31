import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Drawer {
    id: drawer

    Component.onCompleted: {
        for (var child in menuList.contentItem.children) {
            var obj = menuList.contentItem.children[child].children[0];
            for (var innerchild in obj.children) {
                console.log(innerchild);
                console.log(obj.children[innerchild]);
            }
        }
    }

    width: parent.width / 3
    height: window.height
    interactive: true

    background: Rectangle {
        color: "#606060"
    }

    ListView {
        id: menuList
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
            objectName: outerRect
            width: parent.width
            height: menuName.height
            color: "#f0f0f0"

            RowLayout {
                objectName: rowLayout
                Layout.fillHeight: true
                Layout.fillWidth: true

                Rectangle {
                    width: 5
                }

                Text {
                    objectName: menuName
                    id: menuName
                    text: name
                    color: "black"
                    font.pointSize: 24
                }
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
                        console.info("New key entry clicked...");
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

                    case 4:
                        // Quit option
                        console.info("Quit selected.");
                        Qt.quit();
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

        ListElement {
            name: qsTr("Quit")
        }
    }
}
