import QtQuick 2.11
import QtQuick.Layouts 1.3

RowLayout {
    property int sideMargin: 5
    property int lineWidth: 1
    property string lineColor: "grey"

    HorizontalPadding {
        size: sideMargin
    }

    Rectangle {
        height: 1
        Layout.fillWidth: true
        color: "transparent"
        border.color: lineColor
        border.width: lineWidth
    }

    HorizontalPadding {
        size: sideMargin
    }
}
