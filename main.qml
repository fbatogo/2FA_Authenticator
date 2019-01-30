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

        Image {
            id: menuButton
            anchors.leftMargin: 5
            source: "resources/menu.svg"
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

