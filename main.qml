import QtQuick 2.11
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: window

    signal updateOtpData()

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
            source: "/resources/menu.svg"
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
                    if (menuButton.source.toString().indexOf("resources/menu.svg") >= 0) {
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
            font.pointSize: 12
        }

        Text {
            id: topToolbarTitle
            anchors.fill: parent

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            text: bannerTextSize.text
            font.bold: bannerTextSize.font.bold
            font.pointSize: bannerTextSize.font.pointSize
        }
    }

    SideBar {
        id: drawer
    }


    StackView {
        id: screenStack
        initialItem: secretScreen
        anchors.fill: parent

        SecretScreen {
            id: secretScreen
        }

        Component {
            id: showStartHereScreen

            StartHereScreen {
                id: startHereScreen

                onSignalClosing: {
                    // Force update the SecretScreen.
                    updateOtpData();
                }
            }
        }
    }
}

