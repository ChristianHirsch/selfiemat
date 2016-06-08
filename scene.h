#ifndef SCENE_H
#define SCENE_H


#include <vector>
#include <string>

#include <QImage>

class Scene
{
public:
    Scene(const int &_width, const int &_height);
    ~Scene();

    void initialize(const QString &_templateLocation = templateLocation);
    void clear();
    void paint();
    void save(const QString &_path);

    bool addImage(QImage &_image);
    int imagesToAdd();

    QImage getSceneImage() const;

private:
    struct ImageElement {
        float posX = 0.0f, posY = 0.0f, width = 1.0f, height = 1.0f;
        QImage image;
    };

    QImage image;
    QImage background;
    std::vector<ImageElement> imageElements;
    std::vector<ImageElement> templateImageElements;

    static QString templateLocation;
};

#endif // SCENE_H
