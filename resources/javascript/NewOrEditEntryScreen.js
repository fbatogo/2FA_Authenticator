/***
 * Javascript code that is used by the NewOrEditEntryScreen.qml file.
 */

.import Rollin.GeneralInfoSingleton 1.0 as GeneralInfoSingletonImpl
.import Rollin.KeyEntriesSingleton 1.0 as KeyEntriesSingletonImpl
.import Rollin.QRCodeSingleton 1.0 as QRCodeSingleton
.import QtMultimedia 5.11 as QtMultimedia
.import "utils.js" as Utils
.import Rollin.Logger 1.0 as Logger
.import ToastManagerSingleton 1.0 as Toast

/**
 * init() - Configure the NewOrEditEntryScreen to be either in edit mode, or
 *      new mode.
 */
function init() {
    // Change the icon on the toolbar to be the back button.
    menuButton.source = "/resources/images/back.svg";

    // If we are editing, we don't want to show the ZBar button, and we want to change a
    // few other things.
    if (editing) {
        getFromCameraButton.visible = false;

        var keyEntry = KeyEntriesSingletonImpl.KeyEntriesSingleton.fromIdentifier(identifier);

        if (keyEntry === null) {
            Logger.Log.logError("Unable to locate the key entry to edit.");
            //  XXX Show an error.  However, this shouldn't happen!
            return;
        }

        // Set the UI to show our values.
        siteNameInput.boxText = keyEntry.mIdentifier;
        secretValueInput.boxText = keyEntry.mSecret;

        otpTypeText.text = Utils.otpTypeIntToString(keyEntry.mOtpType);

        digitCountValue.text = keyEntry.mOutNumberCount;

        secretValueText.text = Utils.secretTypeIntToString(keyEntry.mKeyType);

        algorithmValue.text = Utils.hashAlgIntToString(keyEntry.mAlgorithm);

        periodValue.text = keyEntry.mTimeStep;
        offsetValue.text = keyEntry.mTimeOffset;
    } else {
        // If we weren't built with ZBar, don't show the button to use the camera.
        if (GeneralInfoSingletonImpl.haveZBar === false) {
            // Hide the camera button.
            getFromCameraButton.visible = false;
        } else {
            // If we don't have any cameras available, disable the button to read a QR code.
            if ((!QtMultimedia.availableCameras) || (QtMultimedia.availableCameras.length <= 0)) {
                Logger.Log.logDebug("No cameras found, disabling the acquire with camera button.");
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

function onActiveFocusChanged(myfocus) {
    if (myfocus) {       
        console.log("Got active focus!");
        if (GeneralInfoSingletonImpl.GeneralInfoSingleton.haveZBar()) {
            // See if the QR code singleton is valid.
            if ((QRCodeSingleton.isOtpCode()) && (QRCodeSingleton.isCodeProcessing())) {
                var temp;

                // Read out the data, and fill in our form.

                // Populate the UI.

                // Start with the secret, since if it isn't defined, then we shouldn't update anything!
                temp = QRCodeSingleton.parameterByKey("secret");
                if (temp) {
                    secretValueInput.boxText = temp;
                } else {
                    Logger.Log.logError("No secret was found in the QR code!?");

                    // Do nothing.
                    return;
                }

                temp = QRCodeSingleton.label();
                if (temp) {
                    siteNameInput.boxText = temp;
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
}

// Check the fields in the UI and see if we should enable
// the save button, or not.
function checkEnableSave() {
    var encodingType = -1;

    if ((siteNameInput.boxText === null) || (siteNameInput.boxText === "")) {
        // Don't allow saving yet.
        saveButton.enabled = false;
        //Logger.Log.logError("Site name is empty.");
        return;
    }

    if ((secretValueInput.boxText === null) || (secretValueInput.boxText === "")) {
        // Don't allow saving yet.
        saveButton.enabled = false;
        //Logger.Log.logError("Secret value is empty!");
        return;
    }

    if (secretValueText.text == "HEX") {
        encodingType = 0;
    } else if (secretValueText.text == "Base32") {
        encodingType = 1;
    } else {
        Logger.Log.logError("Unknown encoding value '" + secretValueText.text + "'!");
        saveButton.enabled = false;
        return;
    }

    // Make sure the secret is encoded properly, based on the encoding type
    // selected.
    if (!GeneralInfoSingletonImpl.GeneralInfoSingleton.isEncodedProperly(encodingType, secretValueInput.boxText)) {
        // Set the error text so that the user knows why they can't save yet.
        showError(qsTr("The secret is not encoded properly!"));
        saveButton.enabled = false;
        //Logger.Log.logError("Secret not encoded properly.");
        return;
    }

    // Otherwise, allow saving.
    saveButton.enabled = true;
}

// Show text in the error field.
function showError(newErrorText) {
    Logger.Log.logError(newErrorText);

    // Show a toast with the error text in it.
    toast.show(newErrorText, 3000);
}

function saveConfiguration() {
    Logger.Log.logDebug("Saving key entries...");
    var keyType, otpType, numberCount, algorithm, period, offset;
    var errorSet = false;

    // Convert the keyType index to the currect value.
    otpType = Utils.otpTypeToInt(otpTypeText.text);
    if (otpType < 0) {
        // Shouldn't be possible, but check anyway.
        showError(qsTr("Invalid OTP type!"));
        return;
    }

    keyType = Utils.secretTypeToInt(secretValueText.text);
    if (keyType < 0) {
        // Shouldn't be possible, but check anyway.
        showError(qsTr("Invalid secret encoding!"));
        return;
    }

    numberCount = parseInt(digitCountValue.text);
    if ((numberCount < 6) || (numberCount > 8)) {
        showError(qsTr("Invalid number of result digits!"));
        numberCount = -1;
        return;
    }
    Logger.Log.logDebug("number count = " + numberCount);

    algorithm = Utils.hashAlgToInt(algorithmValue.text);
    if (algorithm < 0) {
        // Should be impossible... but...
        showError(qsTr("Invalid algorithm value!"));
        return;
    }

    period = parseInt(periodValue.text);
    if (period < 0) {
        // Should be impossible.
        showError(qsTr("Invalid period value!"));
        return;
    }

    offset = parseInt(offsetValue.text);
    if (offset < 0) {
        // Should be impossible.
        showError(qsTr("Invalid offset value!"));
        return;
    }

    if (editing) {
        if (!KeyEntriesSingletonImpl.KeyEntriesSingleton.updateKeyEntry(siteNameInput.boxText, "", secretValueInput.boxText, keyType, otpType, numberCount, algorithm, period, offset)) {
            showError(qsTr("Unable to update the key entry.  Please be sure that values a provided for all of the settings above."));
            return;     // Don't close the window.
        }
    } else {
        if (!KeyEntriesSingletonImpl.KeyEntriesSingleton.addKeyEntry(siteNameInput.boxText, "", secretValueInput.boxText, keyType, otpType, numberCount, algorithm, period, offset)) {
            showError(qsTr("Unable to save the key entry.  Please be sure that values are provided for all of the settings above."));
            return;     // Don't close the window.
        }
    }

    // Close the window.
    screenStack.pop();

    if (editing) {
        // We need to pop 2 screens.
        screenStack.pop();
    }
}
