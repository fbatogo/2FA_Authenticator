#ifndef UTILS_H
#define UTILS_H

#include <QString>

class Utils
{
public:
    Utils();

    static QString concatenateFilenameAndPath(const QString &path, const QString &filename);
};

#endif // UTILS_H
