import QtQuick 2.9
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import Rollin.SettingsHandler 1.0
import Rollin.Logger 1.0

ApplicationWindow {
    id: window

    Component.onCompleted: {
        if (SettingsHandler.logToFile()) {
            // Enable the logger.
            Log.setLogToFile(true);
        }
    }

    property string appName: "Rollin'"

    visible: true
    width: 240
    height: 320
    title: appName
    header: ToolBar {
        id: topToolbar

        width: parent.width
        height: bannerTextSize.height + 10

        position: ToolBar.Header

        background: Rectangle {
            color: "white"

            Rectangle {
                width: parent.width
                height: 1
                anchors.bottom: parent.bottom
                color: "transparent"
                border.color: "#000000"
            }
        }

        Image {
            id: menuButton
            source: "/resources/images/menu.svg"
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: 5
            width: topToolbar.height
            height: topToolbar.height
            z: 1        

            MouseArea {
                anchors.fill: parent
                z:2

                onClicked: {
                    if (menuButton.source.toString().indexOf("menu.svg") >= 0) {
                        // Open or close the side bar.
                        if (!drawer.opened) {
                            drawer.open();
                        }
                    } else {
                        // Pop the widget off the stack.
                        screenStack.pop();
                    }
                }
            }
        }

        TextMetrics {
            id: bannerTextSize

            text: appName
            font.bold: true
            font.pixelSize: 20
        }

        Text {
            id: topToolbarTitle
            anchors.fill: parent

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            text: bannerTextSize.text
            font.bold: bannerTextSize.font.bold
            font.pixelSize: bannerTextSize.font.pointSize
        }
    }

    SideBar {
        id: drawer
    }

    StackView {
        id: screenStack
        initialItem: secretScreen
        anchors.fill: parent

        onCurrentItemChanged: {
            currentItem.forceActiveFocus();
        }
    }

    SecretScreen {
        id: secretScreen
    }

    StartHereScreen {
        id: startHereScreen
    }
}

