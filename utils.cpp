#include "utils.h"

Utils::Utils()
{

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
