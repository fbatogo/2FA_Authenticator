import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

import "widgets/"

Item {
    property int selected: 0

    Component.onCompleted: {

        switch (selected) {
        case 0:
            // 6
            sixdigits.checked = true;
            break;

        case 1:
            // 7
            sevendigits.checked = true;
            break;

        case 2:
            // 8
            eightdigits.checked = true;
            break;
        }
    }

    ColumnLayout {
        anchors.fill: parent

        VerticalPadding {
            size: 10
        }

        SubHeader {
            headerText: qsTr("Select the number of digits of the OTP to show : ");
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
                    id: sixdigits
                    checked: false
                    text: "6"
                    onClicked: selected = 0;
                }

                RadioButton {
                    id: sevendigits
                    checked: false
                    text: "7"
                    onClicked: selected = 1;
                }

                RadioButton {
                    id: eightdigits
                    checked: false
                    text: "8"
                    onClicked: selected = 2;
                }
            }
        }

        FillEmptySpace {
        }
    }
}
