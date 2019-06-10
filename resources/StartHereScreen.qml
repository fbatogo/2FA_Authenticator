import QtQuick 2.0
import QtQuick.Layouts 1.3
import Rollin.KeyEntriesSingleton 1.0

Component {
    Item {
        onActiveFocusChanged: {
            if (activeFocus) {
                console.log("Got active focus");
                if (KeyEntriesSingleton.count() > 0) {
                    // We are done here.
                    screenStack.pop();
                    return;
                }

                // Otherwise, make sure we are showing the "hamburger".
                menuButton.source = "/resources/images/menu.svg";
            }
        }

        RowLayout {
            anchors.fill: parent

            spacing: 2

            Rectangle {
                width: 5
            }

            Image {
                Layout.alignment: Qt.AlignTop
                Layout.topMargin: 5
                id: startHereArrow
                source: "/resources/images/startherearrow.svg"
            }

            Text {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop
                Layout.topMargin: (startHereArrow.height.valueOf() + 5) - 13
                text: qsTr("Select the menu button, and then \"New\" to add your first OTP entry.")
                font.pointSize: 13
                wrapMode: Text.WordWrap
            }

            Rectangle {
                width: 5
            }
        }
    }
}
