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

    void initialize(const std::string &_templateLocation = templateLocation);
    void clear();
    void paint();
    void save(const std::string &_path);

    bool addImage(QImage &_image);

private:
    struct ImageElement {
        float posX = 0.0f, posY = 0.0f, width = 1.0f, height = 1.0f;
        QImage image;
    };

    QImage image;
    QImage background;
    std::vector<ImageElement> imageElements;
    std::vector<ImageElement> templateImageElements;

    static std::string templateLocation;
};

#endif // SCENE_H
