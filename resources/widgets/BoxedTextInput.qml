import QtQuick 2.11

Rectangle {
    property string boxText: ""
    property int totalHeight: 5
    property string boxColor: "black"

    signal editingComplete();

    height: totalHeight
    color: "transparent"

    border.color: boxColor
    border.width: 1

    TextInput {
        id: boxInput
        text: boxText
        anchors.fill: parent
        anchors.topMargin: 3
        anchors.bottomMargin: 3
        anchors.leftMargin: 3
        anchors.rightMargin: 3
        anchors.centerIn: parent
        clip: true

        onEditingFinished: editingComplete();
    }
}
