#include "uikeyentries.h"

#include "logger.h"

UiKeyEntries::UiKeyEntries(QObject *parent) :
    QObject(parent)
{

}

bool UiKeyEntries::populateEntries(QList<KeyEntry> &toCopy)
{
    // We need to convert all of the KeyEntries in to dynamic allocations.
    for (int i = 0; i < toCopy.size(); i++) {
        mEntryList.push_back(new KeyEntry(toCopy.at(i)));
    }

    return true;
}

int UiKeyEntries::count()
{
    return mEntryList.size();
}

KeyEntry *UiKeyEntries::at(int i)
{
    return mEntryList.at(i);
}
