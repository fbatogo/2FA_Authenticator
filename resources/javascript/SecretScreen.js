.import Rollin.KeyEntriesSingleton 1.0 as KeyEntriesSingletonImpl
.import "utils.js" as Utils
.import Rollin.Logger 1.0 as Logger

function updateScreen(listModel) {
    // Make sure the hamburger is showing.
    menuButton.source = "/resources/images/menu.svg";

    if ((listModel === null) || (listModel === undefined)) {
        console.error("No list model was defined to populate in updateScreen()!");
        return;
    }

    //Logger.Log.logDebug("Populating OTP list...");
    // Also update the list model.
    populateListModel(listModel);
}

// Populate the list model.
function populateListModel(listModel) {
    var keyEntryCount = KeyEntriesSingletonImpl.KeyEntriesSingleton.count();

    if ((listModel === null) || (listModel === undefined)) {
        console.error("No list model was defined to populate in populateListModel()!");
        return;
    }

    // Make sure the list model is empty before we start to populate it.
    listModel.clear();

    // If the list is empty, don't try to populate it.
    if (keyEntryCount <= 0) {
        // Nothing to do.
        return;
    }

    // Iterate each entry, and add it to the list.
    for (var i = 0; i < keyEntryCount; i++) {
        if (KeyEntriesSingletonImpl.KeyEntriesSingleton.at(i).mValid === true) {
            // XXX Audit these, we probably don't need all of them.  And update the timer to only be called when we need to update, not every second.
            listModel.append({ otpObject: KeyEntriesSingletonImpl.KeyEntriesSingleton.at(i), showError: false, errorText: "" });
        } else {
            listModel.append({ otpObject: KeyEntriesSingletonImpl.KeyEntriesSingleton.at(i), showError: true, errorText: KeyEntriesSingletonImpl.KeyEntriesSingleton.at(i).mInvalidReason });
        }
    }
}
