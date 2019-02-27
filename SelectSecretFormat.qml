import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

import "resources/widgets/"

Item {
    property int selected: 0

    Component.onCompleted: {
        console.log("Secret format : " + selected);

        switch (selected) {
        case 0:
            // HEX
            hex.checked = true;
            break;

        case 1:
            // Base32
            base32.checked = true;
            break;
        }
    }

    ColumnLayout {
        anchors.fill: parent

        VerticalPadding {
            size: 10
        }

        SubHeader {
            headerText: qsTr("Select the encoding for the OTP secret : ");
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
                    id: hex
                    checked: false
                    text: "HEX"
                    onClicked: selected = 0;
                }

                RadioButton {
                    id: base32
                    checked: false
                    text: "Base32"
                    onClicked: selected = 1;
                }
            }
        }

        FillEmptySpace {
        }
    }
}
