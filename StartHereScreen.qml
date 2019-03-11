import QtQuick 2.0
import QtQuick.Layouts 1.3

ColumnLayout {
    Layout.fillHeight: true
    Layout.fillWidth: true
    spacing: 0

    Rectangle {
        Layout.fillWidth: true
        height: 5
        border.color: "blue"
        border.width: 1
    }

    RowLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true
        spacing: 0

        Rectangle {
            border.color: "yellow"
            border.width: 1
            width: 3
            Layout.fillHeight: true
        }

        Image {
            Layout.alignment: Qt.AlignTop
            id: startHereArrow
            source: "resources/startherearrow.svg"
        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 0

            Rectangle {
                Layout.fillWidth: true
                //Layout.fillHeight: true
                height: 10
                //width: 10
                /*height: {
                    console.log("Height : " + startHereArrow.height);
                    //return startHereArrow.height;
                    return imageContainer.height;
                }
                */
                //height: imageContainer.height
                color: "green"
                border.color: "green"
                border.width: 1
            }
/*
            Text {
                Layout.fillHeight: true
                text: "testing!"
            }*/
        }
    }
}

//        Layout.fillHeight: true
//        Layout.fillWidth: true

//            RowLayout {
/*                Rectangle {
                    width: 3
                    height: startHereArrow.height
                    //Layout.fillHeight: true
                    color: "transparent"
                    border.color: "green"
                    border.width: 1
                }

                Image {
                    id: startHereArrow
                    x: 0
                    y: 0
                    source: "resources/startherearrow.svg"
                }
//            }

        /*
        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            border.color: "red"
            border.width: 1
        }*/

/*
        ColumnLayout {
//                Layout.fillWidth: true
//                Layout.fillHeight: true

            Rectangle {
                id: startHereUpperSpacer
                height: startHereArrow.height * 0.9
                Layout.fillWidth: true
                color: "red"
                border.color: "red"
                border.width: 1
            }
/*
            Text {
                id: startHereText
                text: qsTr("Use the menu button to create a new OTP entry.");
            }

        } */
/*    }
}
*/
