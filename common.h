#ifndef COMMON_H
#define COMMON_H

#include <QString>

#include "scene.h"

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

    static Scene *getScene(int _id=currentScene);
    static Scene *getNextScene();
    static Scene *getPrevScene();
    static int getSceneCount();
    static Scene *loadSceneFromFile(const QString &_filePath = "");
    static void scenePopBack();

private:
    static QString workDirectoryPath;
    static QString fileBaseName;

    static int currentScene;
    static std::vector<Scene *> availabelScenes;
    static Scene baseScene;
};

#endif // COMMON_H
