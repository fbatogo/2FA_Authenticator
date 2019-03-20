import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import Rollin.Logger 1.0

import "resources/widgets/"

Item {
    property int selected: 0

    Component.onCompleted: {
        Log.logDebug("OTP type : " + selected);

        switch (selected) {
        case 0:
            // TOTP
            totp.checked = true;
            break;

        case 1:
            // HOTP
            hotp.checked = true;
            break;
        }
    }

    ColumnLayout {
        anchors.fill: parent

        VerticalPadding {
            size: 10
        }

        SubHeader {
            headerText: qsTr("Select the type of OTP to generate : ");
        }

        VerticalPadding {
            size: 10
        }

        RowLayout {
            Layout.fillWidth: true

            HorizontalPadding {
                size: 5
            }

            ColumnLayout {
                RadioButton {
                    id: totp
                    checked: false
                    text: "TOTP"
                    onClicked: selected = 0;
                }

                RadioButton {
                    id: hotp
                    checked: false
                    text: "HOTP"
                    onClicked: selected = 1;
                }
            }
        }

        FillEmptySpace {
        }
    }
}
