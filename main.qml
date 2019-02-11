import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: window

    visible: true
    width: 240
    height: 320
    title: "Rollin'"
    //visibility: "FullScreen"
    header: ToolBar {
        id: topToolbar

        width: parent.width
        height: topToolbarTitle.height + 10

        background: Rectangle {
            color: "white"
        }

        Rectangle {
            anchors.fill: parent
            anchors.leftMargin: 5
//            color: "white"

            Image {
                id: menuButton
                source: "resources/menu.svg"
                width: topToolbar.height
                height: topToolbar.height

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (screenStack.depth === 1) {
                            // Handle opening and closing the side bar.
                            if (!drawer.opened) {
                                drawer.open()
                            }
                        } else {
                            // Pop the widget off the stack.
                            screenStack.pop();
                        }
                    }
                }
            }
        }

        Text {
            id: topToolbarTitle

            anchors.centerIn: parent

            text: "Rollin'"
            font.bold: true
            font.pointSize: 12
        }

        SideBar {
            id: drawer
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

