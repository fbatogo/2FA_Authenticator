import QtQuick 2.11
import QtQuick.Layouts 1.3

import "qrc:/resources/widgets/"

ColumnLayout {
    property int textLeftPadding: 10
    property string headerText: ""
    property int linePadding: 5
    property string lineColor: "grey"
    property int lineWidth: 1

    RowLayout {
        Layout.fillWidth: true

        HorizontalPadding {
            size: textLeftPadding
        }

        Text {
            anchors.left: leftPadding.right

            text: headerText
        }
    }

    PaddedRowLine {
        sideMargin: linePadding
        lineColor: lineColor
        lineWidth: lineWidth
    }
}
