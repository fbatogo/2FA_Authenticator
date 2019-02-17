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

        setComboBoxes(keyEntry.mOtpType, keyEntry.mKeyType, keyEntry.mOutNumberCount);
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

function setComboBoxes(otpType, encodingType, digitCount) {

    if (digitCount) {
        switch (parseInt(digitCount)) {
        case 6:
            // Set the combo box to index 0.
            numberCountComboBox.currentIndex = 0;
            break;

        case 7:
            // Set the combo box to index 1.
            numberCountComboBox.currentIndex = 1;
            break;

        case 8:
            // Set the combo box to index 2.
            numberCountComboBox.currentIndex = 2;
            break;

        default:
            console.log("Got an invalid number of digits!");
            return;
        }
    } else {
        // Set it to the default value of '6'.
        numberCountComboBox.currentIndex = 0;
    }

    if (otpType) {
        switch (parseInt(otpType)) {
        case 0:
            // TOTP
            otpTypeComboBox.currentIndex = 0;
            break;

        case 1:
            // HOTP
            otpTypeComboBox.currentIndex = 1;
            break;
        }
    } else {
        // Set to the default of TOTP.
        otpTypeComboBox.currentIndex = 0;
    }

    if (encodingType) {
        switch (parseInt(digitCount)) {
        case 0:
            // HEX
            secretValueTypeComboBox.currentIndex = 1;
            break;

        case 1:
            // Base32
            secretValueTypeComboBox.currentIndex = 0;
            break;
        }
    } else {
        // Set the default of BASE32.
        secretValueTypeComboBox.currentIndex = 0;
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

    // Figure out how the value should be encoded.
    switch (secretValueTypeComboBox.currentIndex) {
    case 0:
        // Base 32
        encodingType = 1;
        break;

    case 1:
        // HEX
        encodingType = 0;
        break;

    default:
        console.log("Unknown encoding type selected for the secret value encoding!");
        break;
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
