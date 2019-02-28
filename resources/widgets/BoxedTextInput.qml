import QtQuick 2.11

Rectangle {
    property string boxText: ""
    property string liveText: ""
    property int totalHeight: 5
    property string boxColor: "black"
    property var boxValidator: null

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
        validator: boxValidator

        onEditingFinished: editingComplete();

        onTextEdited: liveText = boxInput.text;
    }
}
