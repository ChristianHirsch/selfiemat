#include "scene.h"

#include <QPainter>

Scene::Scene(const int &_width, const int &_height):
    image(_width, _height, QImage::Format_ARGB32),
    background(_width, _height, QImage::Format_ARGB32)
{
    clear();
}

Scene::~Scene()
{

}

void Scene::initialize(const std::string &_templateLocation)
{
    if(_templateLocation.size() == 0)
    {
        ImageElement element;
        templateImageElements.push_back(element);

        QPainter painter;
        painter.begin(&background);
        painter.fillRect(background.rect(), Qt::white);
        painter.end();

        return;
    }
}

void Scene::clear()
{
    while(imageElements.size() > 0)
    {
        templateImageElements.push_back(imageElements.back());
        imageElements.pop_back();
    }

    QPainter painter;
    painter.begin(&image);
    painter.drawImage(image.rect(), background);
    painter.end();
}

void Scene::paint()
{
    QPainter painter;
    painter.begin(&image);

    for (const ImageElement &element : imageElements)
    {
        painter.drawImage( QRectF(
                    element.posX * image.size().width(),
                    element.posY * image.size().height(),
                    element.width * image.size().width(),
                    element.height * image.size().height()),
                    element.image);
    }

    painter.drawImage(image.rect(), background);
    painter.end();
}

void Scene::save(const std::string &_path)
{
    image.save(QString::fromStdString(_path));
}

bool Scene::addImage(QImage &_image)
{
    if(templateImageElements.size() < 1)
        return false;

    ImageElement element = templateImageElements.back();
    templateImageElements.pop_back();

    element.image = _image;

    imageElements.push_back(element);

    return true;
}

