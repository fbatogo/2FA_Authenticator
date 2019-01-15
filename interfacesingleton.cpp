#include "interfacesingleton.h"

#define APP_VERSION "0.1"

InterfaceSingleton::InterfaceSingleton() :
    QObject(nullptr)
{

}

/**
 * @brief InterfaceSingleton::getInstance - Get the singleton instance for this class.
 *
 * @return InterfaceSingleton pointer.
 */
InterfaceSingleton *InterfaceSingleton::getInstance()
{
    static InterfaceSingleton singleton;

    return &singleton;
}

/**
 * @brief InterfaceSingleton::getQmlSingleton - Return the singleton that will be used by
 *      the QML engine.
 *
 * @param engine - A pointer to the active QML engine.
 * @param scriptEngine - A pointer to the active QML script engine.
 *
 * @return QObject pointer to the singleton.
 */
QObject *InterfaceSingleton::getQmlSingleton(QQmlEngine *engine, QJSEngine *)
{
    InterfaceSingleton *cSingleton;

    cSingleton = getInstance();

    cSingleton->updateEngine(engine);

    QQmlEngine::setObjectOwnership(cSingleton, QQmlEngine::CppOwnership);

    return static_cast<QObject *>(cSingleton);
}

/**
 * @brief InterfaceSingleton::updateEngine - Save a pointer to the QML engine that was
 *      passed to us when we were inited.
 *
 * @param engine - A pointer to the engine provided by the QML engine.
 */
void InterfaceSingleton::updateEngine(QQmlEngine *engine)
{
    mQmlEngine = engine;
}

/**
 * @brief InterfaceSingleton::version - Return the version string for this version of
 *      the app.
 *
 * @return QString containing the version information for this app.
 */
QString InterfaceSingleton::version()
{
    return APP_VERSION;
}

/**
 * @brief InterfaceSingleton::keyEntries - Get the list of all key entries.
 *
 * @return QList<KeyEntry> containing all of the secret key entries store in the
 *      data store.
 */
QList<KeyEntry> InterfaceSingleton::keyEntries()
{

}

