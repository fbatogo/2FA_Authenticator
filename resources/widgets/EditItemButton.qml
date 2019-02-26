import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

Button {
    property int row: 0
    property int column: 0
    property string showScreen: ""      // The filename of the screen we want to show.

    Layout.row: row
    Layout.column: column
    Layout.maximumWidth: threeDots.width + 8
    Layout.maximumHeight: threeDots.height + 8
    height: threeDots.height + 8
    width: threeDots.width + 8
    text: "..."

    onClicked: {
        if (showScreen === "") {
            console.log("ERROR : No screen set to show when button was clicked!");
            return;
        }

        console.log("Showing new screen...");
        screenStack.push(Qt.resolvedUrl(showScreen))
    }
}
