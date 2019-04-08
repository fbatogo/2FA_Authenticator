import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

import "widgets/"

Item {
    property int selected: 0

    Component.onCompleted: {

        switch (selected) {
        case 0:
            // SHA1
            sha1.checked = true;
            break;

        case 1:
            // SHA256
            sha256.checked = true;
            break;

        case 2:
            // SHA512
            sha512.checked = true;
            break;
        }
    }

    ColumnLayout {
        anchors.fill: parent

        VerticalPadding {
            size: 10
        }

        SubHeader {
            headerText: qsTr("Select the hashing algorithm to be used to generate the OTP : ");
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
                    id: sha1
                    checked: false
                    text: "SHA1"
                    onClicked: selected = 0;
                }

                RadioButton {
                    id: sha256
                    checked: false
                    text: "SHA256"
                    onClicked: selected = 1;
                }

                RadioButton {
                    id: sha512
                    checked: false
                    text: "SHA512"
                    onClicked: selected = 2;
                }
            }
        }

        FillEmptySpace {
        }
    }
}
