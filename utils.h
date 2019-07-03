#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>
#include <QString>

class Utils : public QObject
{
    Q_OBJECT

public:
    static Utils *getInstance();
    static QObject *getQmlSingleton(QQmlEngine *engine, QJSEngine *scriptEngine);

    QString concatenateFilenameAndPath(const QString &path, const QString &filename);

    Q_INVOKABLE bool fileExists(QString fileNameAndPath);
    Q_INVOKABLE bool copyFile(QString srcFile, QString destFile);
    Q_INVOKABLE bool moveFile(QString srcFile, QString destFile);
    Q_INVOKABLE bool deleteFile(QString fileToDelete);
    Q_INVOKABLE bool pathExists(QString path);
    Q_INVOKABLE bool createPath(QString path);

    Q_INVOKABLE QString lastError();

private:
    Utils();

    QString stripFilePrefix(const QString &fileUrl);

    void logError(const QString &errorString);
    QString mLastError;
};

#endif // UTILS_H
