import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

import "resources/widgets/"

Item {
    property int selected: 0

    ColumnLayout {
        anchors.fill: parent

        VerticalPadding {
            size: 10
        }

        SubHeader {
            headerText: qsTr("Select period that an OTP is valid for : ");
        }

        VerticalPadding {
            size: 10
        }

        RowLayout {
            Layout.fillWidth: true

            HorizontalPadding {
                size: 5
            }

            BoxedTextInput {
                id: timeStepInput

                Layout.fillWidth: true
                boxText: selected

                onEditingComplete: selected = boxText       // XXX Check to make sure the value provided is a number!
            }

            Text {
                text: qsTr(" second(s)");
            }
        }

        VerticalPadding {
            size: 10
        }

        Text {
            text: qsTr("The default value for this setting is usually what you should use.  Only change this if you have been instructed to do so by the OTP provider!");
            color: "red"
        }

        FillEmptySpace {
        }
    }
}
