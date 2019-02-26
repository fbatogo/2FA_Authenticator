/***
 * Javascript code that is used by the NewOrEditEntryScreen.qml file.
 */

.import InterfaceSingleton.InterfaceSingleton 1.0 as InterfaceSingleton
.import QRCodeSingleton.QRCodeSingleton 1.0 as QRCodeSingleton
.import QtMultimedia 5.11 as QtMultimedia

/**
 * init() - Configure the NewOrEditEntryScreen to be either in edit mode, or
 *      new mode.
 */
function init() {
    // Change the icon on the toolbar to be the back button.
    menuButton.source = "/resources/back.svg";

    // If we are editing, we don't want to show the ZBar button, and we want to change a
    // few other things.
    if (editing) {
        getFromCameraButton.visible = false;

        var keyEntry = InterfaceSingleton.InterfaceSingleton.keyEntryFromIdentifier(identifier);

        // Set the UI to show our values.
        siteNameInput.text = keyEntry.mIdentifier;
        secretValueInput.text = keyEntry.mSecret;

        switch (keyEntry.mOtpType) {
        case 0:
            // TOTP
            otpTypeText.text = "TOTP";
            break;

        case 1:
            // HOTP
            otpTypeText.text = "HOTP";
            break;
        }

        digitCountValue.text = keyEntry.mOutNumberCount;

        switch (keyEntry.keyType) {
        case 0:
            // HEX
            secretValueText.text = "HEX";
            break;

        case 1:
            // Base32
            secretValueText.text = "Base32";
            break;
        }

        switch (keyEntry.mAlgorithm) {
        case 0:
            algorithmValue.text = "SHA1";
            break;

        case 1:
            algorithmValue.text = "SHA256";
            break;

        case 2:
            algorithmValue.text = "SHA512";
            break;
        }

        periodValue.text = keyEntry.mTimeStep;
        offsetValue.text = keyEntry.mTimeOffset;
    } else {
        // If we weren't built with ZBar, don't show the button to use the camera.
        if (InterfaceSingleton.haveZBar === false) {
            // Hide the camera button.
            getFromCameraButton.visible = false;
        } else {
            // If we don't have any cameras available, disable the button to read a QR code.
            if ((!QtMultimedia.availableCameras) || (QtMultimedia.availableCameras.length <= 0)) {
                console.log("No cameras found, disabling the acquire with camera button.");
                getFromCameraButton.enabled = false;

                // Change the text.
                getFromCameraButton.text = qsTr("No cameras found.");
            }
        }

        otpTypeText.text = "TOTP";
        digitCountValue.text = "6";
        secretValueText.text = "Base32"
        algorithmValue.text = "SHA1";
        periodValue.text = 30;
        offsetValue.text = 0;
    }

    // See if the save button should be enabled, or not.
    checkEnableSave();
}

function onVisibleChanged(visible) {
    if (visible) {
        // See if the QR code singleton is valid.
        if ((QRCodeSingleton.isOtpCode()) && (QRCodeSingleton.isCodeProcessing())) {
            var temp;

            // Read out the data, and fill in our form.

            // Populate the UI.

            // Start with the secret, since if it isn't defined, then we shouldn't update anything!
            temp = QRCodeSingleton.parameterByKey("secret");
            if (temp) {
                secretValueInput.text = temp;
            } else {
                console.log("No secret was found in the QR code!?");

                // Do nothing.
                return;
            }

            temp = QRCodeSingleton.label();
            if (temp) {
                siteNameInput.text = temp;
            }

            var digits = QRCodeSingleton.parameterByKey("digits");
            var otpType = QRCodeSingleton.type();
            //var encodingType = QRCodeSingleton.parameterByKey()

            setComboBoxes(otpType, 1, digits);

            // See if the save button should be enabled.
            checkEnableSave();
        }  // Otherwise, the user may have cancelled, so don't do anything.
    }
}

// Check the fields in the UI and see if we should enable
// the save button, or not.
function checkEnableSave() {
    var encodingType = -1;

    if ((siteNameInput.text === null) || (siteNameInput.text === "")) {
        // Don't allow saving yet.
        saveButton.enabled = false;
        return;
    }

    if ((secretValueInput.text === null) || (secretValueInput.text === "")) {
        // Don't allow saving yet.
        saveButton.enabled = false;
        return;
    }

    if (secretValueText.text == "HEX") {
        encodingType = 0;
    } else if (secretValueText.text == "Base32") {
        encodingType = 1;
    } else {
        console.log("Unknown encoding value '" + secretValueText.text + "'!");
        saveButton.enabled = false;
        return;
    }

    // Make sure the secret is encoded properly, based on the encoding type
    // selected.
    if (!InterfaceSingleton.InterfaceSingleton.isEncodedProperly(encodingType, secretValueInput.text)) {
        // Set the error text so that the user knows why they can't save yet.
        errorText.text = qsTr("The secret is not encoded properly!");
        saveButton.enabled = false;
        return;
    }

    // Otherwise, allow saving.
    saveButton.enabled = true;
}

function saveConfiguration() {
    console.log("Saving key entries...");
    var keyType, otpType, numberCount, algorithm, period, offset;
    var errorSet = false;

    // Convert the keyType index to the currect value.
    switch (otpTypeComboBox.currentIndex) {
    case 0:
        // TOTP
        otpType = 0;
        break;

    case 1:
        // HOTP
        otpType = 1;
        break;

    default:
        // Shouldn't happen, but set to an invalid value to cause the save to fail.
        otpType = -1;
        errorText.text = qsTr("Invalid OTP type!");
        errorSet = true;
        break;
    }

    if (secretValueText.text == "HEX") {
        keyType = 0;
    } else if (secretValueText.text == "Base32") {
        keyType = 1;
    } else {
        errorText.text = qsTr("Invalid secret encoding!");
        console.log("Unknown encoding value '" + secretValueText.text + "'!");
        keyType = -1;
    }

    numberCount = parseInt(digitCountValue.text);
    if ((numberCount < 6) || (numberCount > 8)) {
        errorText.text = qsTr("Invalid number of result digits!");
        numberCount = -1;
    }

    if (algorithmValue.text == "SHA1") {
        algorithm = 0;
    } else if (algorithmValue.text == "SHA256") {
        algorithm = 1;
    } else if (algorithmValue.text == "SHA512") {
        algorithm = 2;
    } else {
        errorText.text = qsTr("Unknown/unexpected algorithm value!");
        algorithm = -1;
    }

    period = parseInt(periodValue.text);
    offset = parseInt(offsetValue.text);

    if (editing) {
        if (!InterfaceSingleton.updateKeyEntry(siteNameInput.text, secretValueInput.text, keyType, otpType, numberCount, algorithm, period, offset)) {
            if (!errorSet) {
                errorText.text = qsTr("Unable to update the key entry.  Please be sure that values a provided for all of the settings above.");

                // Create a timer to make the error text disappear after a few seconds.
                Qt.createQmlObject("import QtQuick 2.0; Timer { interval: 3000; running: true; repeat: false; onTriggered: errorText.visible = false; }", parent, "timer");
                console.info("Timer set..");
            }

            InterfaceSingleton.logError("Failed to update the key data!  Error : " + errorText.text);

            // Show the text.
            errorText.visible = true;
            return;     // Don't close the window.
        }
    } else {
        if (!InterfaceSingleton.addKeyEntry(siteNameInput.text, secretValueInput.text, keyType, otpType, numberCount, algorithm, period, offset)) {
            if (!errorSet) {
                errorText.text = qsTr("Unable to save the key entry.  Please be sure that values are provided for all of the settings above.");

                // Create a timer to make the error text disappear after a few seconds.
                Qt.createQmlObject("import QtQuick 2.0; Timer { interval: 3000; running: true; repeat: false; onTriggered: errorText.visible = false; }", parent, "timer");
                console.info("Timer set..");
            }

            InterfaceSingleton.logError("Failed to save the key data!  Error : " + errorText.text);

            // Show the text.
            errorText.visible = true;
            return;     // Don't close the window.
        }
    }

    // Close the window.
    screenStack.pop();
}
