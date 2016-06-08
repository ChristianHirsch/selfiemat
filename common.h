#ifndef COMMON_H
#define COMMON_H

#include <QString>

class Common
{
public:
    Common();

    static QString getWorkDirectoryPath();
    static void setWorkDirectoryPath(const QString &value);
    static void setWorkDirectoryPathWithDialog();

    static QString getAbsoluteBaseFilePath();

    static QString getFileBaseName();
    static void setFileBaseName(const QString &value);
    static void setFileBaseNameWithDialog();

private:
    static QString workDirectoryPath;
    static QString fileBaseName;
};

#endif // COMMON_H
