#ifndef SCENE_H
#define SCENE_H


#include <vector>
#include <string>

#include <QImage>

class Scene
{
public:
    Scene(const int &_width, const int &_height);
    Scene(const QString &_filePath);
    ~Scene();

    void initialize(const QString &_templateLocation = templateLocation);
    void clear();
    void paint();
    void save(const QString &_path);

    void loadScene(const QString &_filePath);

    bool addImage(QImage &_image);
    int imagesToAdd();

    QImage getSceneImage() const;

    QImage getPreviewImage() const;

    static void setTemplateLocation(const QString &_templateLocation);

private:
    struct ImageElement {
        float posX = 0.0f;
        float posY = 0.0f;
        float width = 1.0f;
        float height = 1.0f;
        QImage image;
    };

    QImage image;
    QImage background;
    std::vector<ImageElement> imageElements;
    std::vector<ImageElement> templateImageElements;

    static QString templateLocation;

    static QRectF getSourceFromImage(const QImage &_image, const QRectF &_target);
};

#endif // SCENE_H
