import QtQuick 2.0

/**
  * adapted from StackOverflow:
  * http://stackoverflow.com/questions/26879266/make-toast-in-android-by-qml
  * @brief Manager that creates Toasts dynamically
  */
ListView {
    /**
      * Public
      */

    /**
      * @brief Shows a Toast
      *
      * @param {string} text Text to show
      * @param {real} duration Duration to show in milliseconds, defaults to 3000
      */
    function show(text, duration) {
        // Iterate the list model to make sure that we aren't already showing a toast
        // with the same text.
        for (var i = 0; i < model.count; i++) {
            if (text === model.get(i).text) {
                // Nothing to do.
                return;
            }
        }

        model.insert(0, {text: text, duration: duration});
    }

    /**
      * Private
      */

    id: root

    z: Infinity
    spacing: 5
    anchors.fill: parent
    anchors.bottomMargin: 10
    verticalLayoutDirection: ListView.BottomToTop

    interactive: false

    displaced: Transition {
        NumberAnimation {
            properties: "y"
            easing.type: Easing.InOutQuad
        }
    }

    delegate: Toast {
        Component.onCompleted: {
            if (typeof duration === "undefined") {
                show(text);
            }
            else {
                show(text, duration);
            }
        }
    }

    model: ListModel {id: model}
}
