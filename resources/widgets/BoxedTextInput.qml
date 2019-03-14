import QtQuick 2.11

Rectangle {
    property string boxText: ""
    property string liveText: ""
    property int totalHeight: 5
    property string boxColor: "black"
    property string highlightColor: "blue"
    property var boxValidator: null

    signal editingComplete();

    height: totalHeight
    color: "transparent"

    border.color: boxColor
    border.width: 1

    onFocusChanged: {
        if (focus) {
            focus = false;
            boxInput.focus = true;
        }
    }

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

        onEditingFinished: {
            boxText = boxInput.text;
            editingComplete();
        }

        onFocusChanged: {
            if (focus) {
                parent.border.color = highlightColor;
                parent.border.width = 2;
            } else {
                parent.border.color = boxColor;
                parent.border.width = 1;
            }
        }

        onTextEdited: liveText = boxInput.text;
    }
}
