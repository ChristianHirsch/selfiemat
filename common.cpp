#include "common.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QStandardPaths>

QString Common::workDirectoryPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
        + "/selfiemat";
QString Common::fileBaseName = "/selfie";

int Common::currentScene = 0;
std::vector<Scene *> Common::availabelScenes(0);
Scene Common::baseScene(1800, 1200);

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

Scene *Common::getScene(int _id)
{
    if(getSceneCount() <= 0)
        return &baseScene;
    if(_id < 0)
        _id = getSceneCount() - _id;
    currentScene = _id % getSceneCount();
    return availabelScenes[currentScene];
}

Scene *Common::getNextScene()
{
    currentScene++;
    return getScene(currentScene);
}

Scene *Common::getPrevScene()
{
    currentScene--;
    return getScene(currentScene);
}

int Common::getSceneCount()
{
    return availabelScenes.size();
}

Scene *Common::loadSceneFromFile(const QString &_filePath)
{
    Scene *scene;
    scene = new Scene(_filePath.size() > 0 ? _filePath :
                      QFileDialog::getOpenFileName(0, "Open scene"));
    availabelScenes.push_back(scene);
    return scene;
}

void Common::scenePopBack()
{
    Scene *scene = availabelScenes.back();
    delete(scene);
    availabelScenes.pop_back();
}

