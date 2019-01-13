import QtQuick 2.0

Item {
    Rectangle {
        anchors.fill: parent
        color: "green"

        MouseArea {
            anchors.fill: parent
            onClicked: {
                // Remove ourselves.
                console.log("Pop!");
                screenStack.pop()
            }
        }
    }
}
