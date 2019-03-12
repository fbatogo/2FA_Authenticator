.import Rollin.InterfaceSingleton 1.0 as InterfaceSingletonImpl
.import "utils.js" as Utils

function updateScreen(listModel) {
    // Make sure the hamburger is showing.
    menuButton.source = "resources/menu.svg";

    if ((listModel === null) || (listModel === undefined)) {
        console.error("No list model was defined to populate in updateScreen()!");
        return;
    }

    console.log("Populating OTP list...");
    // Also update the list model.
    populateListModel(listModel);
}

function updateEntryState(listModel) {
    var updatedOtps = false;

    if ((listModel === null) || (listModel === undefined)) {
        console.error("No list model was defined to populate in updateEntryState()!");
        return;
    }

    // Increment the clock image.
    for(var i = 0; i < listModel.count; i++) {
        var listItem = listModel.get(i);

        if (!listItem.showError) {
            listItem.currentTimer++;

            if (listItem.currentTimer > listItem.timeStep) {
                // Reset the timer to 0.
                listItem.currentTimer = 0;

                if (!updatedOtps) {
                    console.log("Get all OTP values...");
                    updatedOtps = true;

                    populateListModel(listModel);
                    return;
                }
            }

            if (listItem.timeStep <= 0) {
                console.error("Time step is <= 0.  Ignoring!");
            } else {
                if (listItem.currentTimer === 0) {
                    listItem.circleShown = 0;
                } else {
                    // Calculate the percentage of time that has passed.
                    var timePercent = listItem.currentTimer / listItem.timeStep;

                    // Then, figure out how much of the circle to draw.
                    listItem.circleShown = 360 * timePercent;
                }
            }
        }
    }
}

// Populate the list model.
function populateListModel(listModel) {
    var otpEntryList = InterfaceSingletonImpl.InterfaceSingleton.calculateKeyEntries();

    if ((listModel === null) || (listModel === undefined)) {
        console.error("No list model was defined to populate in populateListModel()!");
        return;
    }

    // Make sure the list model is empty before we start to populate it.
    listModel.clear();

    // If the list is empty, don't try to populate it.
    if (otpEntryList === null) {
        // Nothing to do.
        return;
    }

    // Iterate each entry, and add it to the list.
    console.log("Iterating entries...");
    for (var i = 0; i < otpEntryList.count(); i++) {
        var temp = otpEntryList.at(i);
        var code;
        var parts;

        if (temp.mValid === true) {
            switch (temp.mCurrentCode.length) {
            case 6:
                // Add a space between the first and 2nd 3 characters.
                parts = temp.mCurrentCode.match(/.{1,3}/g);
                code = parts[0] + " " + parts[1];
                break;

            case 7:
                // Add a space between the first 4 and second 3 characters.
                parts = temp.mCurrentCode.match(/.{1,4}/g);
                code = parts[0] + " " + parts[1];
                break;

            case 8:
                // Add a space between the first and 2nd 4 characters.
                parts = temp.mCurrentCode.match(/.{1,4}/g);
                code = parts[0] + " " + parts[1];
                break;

            default:
                // We should only allow 6, 7, and 8, but if we get something else, just
                // display the number with no spaces.
                code = temp.mCurrentCode
                break;
            }

            // Calculate the percentage of time that has passed.
            var timePercent = temp.mStartTime / temp.mTimeStep;

            listModel.append({ identifier: temp.mIdentifier, timeStep: temp.mTimeStep, otpCode: code, currentTimer: temp.mStartTime,  issuerText: temp.mIssuer, otpType: temp.mOtpType, hotpCounter: temp.mHotpCounter, algorithm: temp.mAlgorithm, circleShown: (360 * timePercent), showError: false, errorText: "" });
        } else {
            listModel.append({ identifier: temp.mIdentifier, otpCode: "", issuerText: "", otpType: -1, hotpCounter: -1, algorithm: -1, showError: true, errorText: temp.mInvalidReason });
        }
    }
}
