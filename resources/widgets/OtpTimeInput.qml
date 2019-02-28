import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

Item {
    property int selected: 0
    property string promptText: ""
    property string warningText: qsTr("The default value for this setting is usually what you should use.  Only change this if you have been instructed to do so by the OTP provider!");

    ColumnLayout {
        anchors.fill: parent

        VerticalPadding {
            size: 10
        }

        SubHeader {
            headerText: promptText
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
                totalHeight: secondsText.height + 8
                boxValidator: IntValidator{bottom: 0; top: 86400;}

                onLiveTextChanged: selected = liveText;
            }

            Text {
                id: secondsText
                text: qsTr(" second(s)");
            }

            HorizontalPadding {
                size: 5
            }
        }

        VerticalPadding {
            size: 10
        }

        RowLayout {
            Layout.fillWidth: true

            HorizontalPadding {
                size: 5
            }

            Text {
                Layout.fillWidth: true
                text: warningText
                color: "red"
                wrapMode: Text.WordWrap
            }

            HorizontalPadding {
                size: 5
            }
        }

        FillEmptySpace {
        }
    }
}
