.pragma library

.import Rollin.InterfaceSingleton 1.0 as InterfaceSingletonImpl

function populateKeyItemModel(model) {
    var keys = InterfaceSingletonImpl.InterfaceSingleton.keyEntries();

    // Start by clearing the model.
    model.clear();

    if (keys !== null) {
        for (var i = 0; i < keys.count(); i++) {
            // Append to the model.
            model.append({ identifier: keys.at(i).mIdentifier });
        }
    }
}
