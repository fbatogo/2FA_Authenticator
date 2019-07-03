#include "utils.h"

#include <QFile>
#include <QDir>

#include "logger.h"

Utils::Utils() :
    QObject(nullptr)
{
    mLastError.clear();
}

Utils *Utils::getInstance()
{
    static Utils singletonInstance;

    return &singletonInstance;
}

QObject *Utils::getQmlSingleton(QQmlEngine *, QJSEngine *)
{
    Utils *cSingleton;

    cSingleton = getInstance();

    QQmlEngine::setObjectOwnership(cSingleton, QQmlEngine::CppOwnership);

    return static_cast<QObject *>(cSingleton);
}

/**
 * @brief Utils::concatenateFilenameAndPath - Given a path, and file name,
 *      concatenate the two, coming up with a properly formatted path name.
 *
 * @param path - The path that we want to concatenate with the file name.
 * @param filename - The file name that we want to concatenate with the path.
 *
 * @return QString containing properly formatted path and file name.
 */
QString Utils::concatenateFilenameAndPath(const QString &path, const QString &filename)
{
    QString trimmedPath = path;
    QString trimmedFilename = filename;
    bool isRoot = false;

    // If the filename is empty, we can't concatenate anything.
    if (trimmedFilename.isEmpty()) {
        // Return an empty string.
        return "";
    }

    // Start by trimming the path.

    // Make sure the path isn't a root path for either *nix or Windows.
    if ((trimmedPath != "/") && (!trimmedPath.endsWith(":/")) && (!trimmedPath.endsWith(":\\"))) {
        trimmedPath.chop(1);
    } else {
        // Set a flag that indicates this is a root path.
        isRoot = true;
    }

    // Make sure the filename doesn't start with a /, or \\.
    if ((trimmedFilename.startsWith("/")) || (trimmedFilename.startsWith("\\"))) {
        trimmedFilename = trimmedFilename.mid(1);
    }

    // If the file name contains a partial path, reject it and return an
    // empty string.
    if ((trimmedFilename.contains("/")) || (trimmedFilename.contains("\\"))) {
        return "";
    }

    // Return the full pathname.
    if (isRoot) {
        // This is a root path, so we don't need to add a /.
        return trimmedPath + trimmedFilename;
    }

    return trimmedPath + "/" + trimmedFilename;
}

/**
 * @brief Utils::fileExists - Check to see if a file exists.  The filename provided will be checked
 *      to see if it is a "file://" URL, and have the prefix stripped before checking.  This call
 *      is primarily used in the QML code, as using QFile() directly is probably easier in the
 *      C++ code.
 *
 * @param fileNameAndPath - The path and file name of the file we want to check for existance.
 *
 * @return true if the file exists.  false on error, or if the file doesn't exist.
 */
bool Utils::fileExists(QString fileNameAndPath)
{
    QString strippedFilename;

    // Start by stripping off the file:///, if it exists.
    strippedFilename = stripFilePrefix(fileNameAndPath);

    // Then, use QFile to see if the file exists.
    return QFile(strippedFilename).exists();
}

/**
 * @brief Utils::copyFile - Copy a file from one location to another.
 *
 * @param srcFile - The file name (and maybe path) that we want to copy data from.
 * @param destFile - The file name (and maybe path) that we want to copy data to.
 *
 * @return true if the file was copied.  false on error.
 */
bool Utils::copyFile(QString srcFile, QString destFile)
{
    QFile toCopyFrom;

    mLastError.clear();

    if (srcFile.isEmpty()) {
        logError("Cannot copy from an empty file name!");
        return false;
    }

    if (destFile.isEmpty()) {
        logError("Cannot copy to an empty file name!");
        return false;
    }

    // Set the path of the file we are going to copy from.
    toCopyFrom.setFileName(srcFile);

    // Copy the data.
    if (!toCopyFrom.copy(destFile)) {
        logError("Failed to copy from '" + srcFile + "' to '" + destFile + "'!  Error : " + toCopyFrom.errorString());
        return false;
    }

    return true;
}

/**
 * @brief Utils::moveFile - Move a file from one location to another.
 *
 * @param srcFile - The file name (and maybe path) that we want to move data from.
 * @param destFile - The file name (and maybe path) that we want to move data to.
 *
 * @return true if the file was moved.  false on error.
 */
bool Utils::moveFile(QString srcFile, QString destFile)
{
    QFile moveFrom;

    mLastError.clear();

    // Make sure our parameters seem valid.
    if (srcFile.isEmpty()) {
        logError("Cannot move from an empty file path!");
        return false;
    }

    if (destFile.isEmpty()) {
        logError("Cannot move to an empty file path!");
        return false;
    }

    // Make sure our destination path doesn't exist.  If it does, return an error.
    if (fileExists(destFile)) {
        logError("The destination file '" + destFile + "' already exists!  Cannot move the file '" + srcFile + "'!");
        return false;
    }

    // Load the srcFile path to our QFile object.
    moveFrom.setFileName(srcFile);

    // Move the file.
    if (!moveFrom.rename(destFile)) {
        logError("Failed to move the file '" + srcFile + "' to '" + destFile + "'!  Error : " + moveFrom.errorString());
        return false;
    }

    return true;
}

/**
 * @brief Utils::deleteFile - Delete a file from the filesystem.
 *
 * @param fileToDelete - The file name (and maybe path) that we want to delete.
 *
 * @return true if the file was deleted.  false on error.
 */
bool Utils::deleteFile(QString fileToDelete)
{
    QFile toDelete(fileToDelete);

    mLastError.clear();

    if (fileToDelete.isEmpty()) {
        logError("Cannot delete a file that is an empty string!");
        return false;
    }

    if (!toDelete.remove()) {
        logError("Failed to delete the file '" + fileToDelete + "'!  Error : " + toDelete.errorString());
        return false;
    }

    return true;
}

/**
 * @brief Utils::pathExists - Check to see if the provided path exists, or not.
 *
 * @param path - The path to check the existance of.
 *
 * @return true if the path exists.  false if it doesn't, or on error.
 */
bool Utils::pathExists(QString path)
{
    QDir toCheck(path);

    if (path.isEmpty()) {
        logError("Cannot check if a path exists for an empty string!");
        return false;
    }

    return toCheck.exists();
}

/**
 * @brief Utils::createPath - Create a new directory, and if necessary, all of the parent directories
 *      that are defined.
 *
 * @param path - The directory/directories to create.
 *
 * @return true if the directory/directories were created.  false on error.
 */
bool Utils::createPath(QString path)
{
    QDir toCreate;

    mLastError.clear();

    if (path.isEmpty()) {
        logError("Cannot create a directory from an empty string!");
        return false;
    }

    // Make sure it doesn't already exist.
    if (pathExists(path)) {
        logError("The directory '" + path + "' already exists!");
        return false;
    }

    if (!toCreate.mkpath(path)) {
        logError("Failed to create the path '" + path + "'!");
        return false;
    }

    return true;
}

/**
 * @brief Utils::lastError - Return the string for the last error that was recorded in this
 *      class.
 *
 * @return QString containing the value of mLastError.
 */
QString Utils::lastError()
{
    return mLastError;
}

/**
 * @brief Utils::stripFilePrefix - Check to see if a string starts with "file:///".  If it does,
 *      then strip it off and return the rest of the string.  If it doesn't, the string will be
 *      returned exactly how it was passed in.
 *
 * @param fileUrl - The string to have the "file:///" stripped off.
 *
 * @return QString containing the original string, with the "file:///" stripped off, if it exists.
 */
QString Utils::stripFilePrefix(const QString &fileUrl)
{
    if (fileUrl.startsWith("file:///")) {
        return fileUrl.mid(8);
    }

    // Otherwise, return the original string.
    return fileUrl;
}

/**
 * @brief Utils::logError - Save the error message to mLastError, then write the log line to
 *      the logger.
 *
 * @param errorString - The error string to store and write to the logger.
 */
void Utils::logError(const QString &errorString)
{
    mLastError = errorString;
    LOG_ERROR(mLastError);
}
