#include "common.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QStandardPaths>

QString Common::workDirectoryPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
        + "/selfiemat";
QString Common::fileBaseName = "/selfie";

Common::Common()
{

}

QString Common::getWorkDirectoryPath()
{
    return workDirectoryPath;
}

void Common::setWorkDirectoryPath(const QString &value)
{
    workDirectoryPath = value;
}

void Common::setWorkDirectoryPathWithDialog()
{
    setWorkDirectoryPath(QFileDialog::getExistingDirectory(0, "Select work directory", workDirectoryPath));
}

QString Common::getAbsoluteBaseFilePath()
{
    QFileInfo fileInfo(getWorkDirectoryPath() + "/" + getFileBaseName());
    return fileInfo.absoluteFilePath();
}

QString Common::getFileBaseName()
{
    return fileBaseName;
}

void Common::setFileBaseName(const QString &value)
{
    fileBaseName = value;
}

void Common::setFileBaseNameWithDialog()
{
    QFileInfo fileInfo(QFileDialog::getSaveFileName(0, "Select file base name", getWorkDirectoryPath()));

    workDirectoryPath = fileInfo.dir().absolutePath();
    fileBaseName = fileInfo.fileName();
}

