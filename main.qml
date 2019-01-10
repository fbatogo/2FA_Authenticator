import QtQuick 2.9
import QtQuick.Controls 2.2

ApplicationWindow {
    id: window

    visible: true
    width: 640
    height: 480
    title: qsTr("Scroll")

    ToolBar {
        id: topToolbar

        z: 1        // Make sure this is on top.
        width: parent.width
        height: 0.05 * parent.height

        Image {
            source: "resources/menubutton.png"
            //anchors.left: parent
            anchors.leftMargin: 5
            anchors.topMargin: 5
            //anchors.verticalCenter: true
            width: topToolbar.height
            height: topToolbar.height

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.info("Clicked hamburger.");
                    if (!drawer.opened) {
                        drawer.open()
                    }
                }
            }
        }

        Label {
            id: topToolbarTitle

            anchors.centerIn: parent
            text: "2FA Authenticator"
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
            model: 20
            delegate: ItemDelegate {
                text: "Item " + (index + 1)
                width: parent.width
            }
        }
    }
}
