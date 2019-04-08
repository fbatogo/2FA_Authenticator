.pragma library

.import Rollin.KeyEntriesSingleton 1.0 as KeyEntriesSingletonImpl

function populateKeyItemModel(model) {
    var keyCount = KeyEntriesSingletonImpl.KeyEntriesSingleton.count();

    // Start by clearing the model.
    model.clear();

    console.log("Key Count : " + keyCount);
    if (keyCount > 0) {
        for (var i = 0; i < keyCount; i++) {
            // Append to the model.
            console.log("Adding : " + KeyEntriesSingletonImpl.KeyEntriesSingleton.at(i).mIdentifier);
            model.append({ identifier: KeyEntriesSingletonImpl.KeyEntriesSingleton.at(i).mIdentifier });
        }
    }
}
