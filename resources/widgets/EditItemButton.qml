import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import Rollin.Logger 1.0

Rectangle {
    property int row: 0
    property int column: 0
    property var showScreen: {}      // The filename of the screen we want to show.
    property int selected: 0
    property string focusBorderColor: "blue"

    Layout.row: row
    Layout.column: column
    color: "transparent"
    width: 20
    height: 20

    onFocusChanged: {
        if (focus) {
            // Give us a border using the "focusBorderColor" color.
            border.color = focusBorderColor;
            border.width = 2;
        } else {
            // Disable the border.
            border.width = 0;
        }
    }

    Keys.onPressed: {
        if ((event.key === Qt.Key_Space) || (event.key === Qt.Key_Enter)) {
            // This is the same as clicking the mouse.
            if (showScreen === "") {
                Log.logError("ERROR : No screen set to show when button was clicked!");
                return;
            }

            screenStack.push(showScreen, { selected: selected });
        }
    }

    Image {
        id: editImage
        anchors.fill: parent
        source: "/resources/images/pencil.svg"
        fillMode: Image.Stretch
        z: 22

        MouseArea {
            anchors.fill: parent
            z:23

            onClicked: {
                if (showScreen === "") {
                    Log.logError("ERROR : No screen set to show when button was clicked!");
                    return;
                }

                screenStack.push(showScreen, { selected: selected });
            }
        }
    }
}
