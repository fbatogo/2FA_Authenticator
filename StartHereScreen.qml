import QtQuick 2.0
import QtQuick.Layouts 1.3
import Rollin.InterfaceSingleton 1.0

Item {
    onVisibleChanged: {
        if (visible) {
            // Make sure the hamburger is showing.
            menuButton.source = "/resources/menu.svg";

            // See if we have any entries yet.
            var otpEntryList = InterfaceSingleton.calculateKeyEntries();

            // If we have at least one entry, pop this screen off the stack.
            if (otpEntryList.count() > 0) {
                // Let the SecretScreen know that we are closing.
                signalClosing();

                // Then, pop this screen off the stack.
                screenStack.pop();
            }
        }
    }

    // Signal to let the SecretScreen know that we intend to close, and it should update.
    signal signalClosing()

    ColumnLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            height: 5
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 0

            Rectangle {
                width: 3
                Layout.fillHeight: true
            }

            Rectangle {
                id: container
                width: startHereArrow.sourceSize.width
                Layout.fillHeight: true

                Image {
                    Layout.alignment: Qt.AlignTop
                    id: startHereArrow
                    source: "resources/startherearrow.svg"
                }
            }

            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true

                Text {
                    anchors.fill: parent
                    anchors.topMargin: (startHereArrow.sourceSize.height - 15)
                    anchors.leftMargin: 5
                    anchors.rightMargin: 5
                    text: qsTr("Select the menu button, and then \"New\" to add your first OTP entry.")
                    font.pointSize: 13
                    wrapMode: Text.WordWrap
                }
            }
        }
    }
}
