#include "scene.h"

#include "common.h"

#include <QPainter>
#include <QFile>
#include <QFileDialog>
#include <QXmlStreamReader>

QString Scene::templateLocation = "";

Scene::Scene(const int &_width, const int &_height):
    image(_width, _height, QImage::Format_ARGB32),
    background(_width, _height, QImage::Format_ARGB32)
{
    clear();
}

Scene::Scene(const QString &_filePath)
{
    loadScene(_filePath);
}

Scene::~Scene()
{

}

void Scene::initialize(const QString &_templateLocation)
{
    if(_templateLocation.size() == 0)
    {
        ImageElement element;
        element.width = image.width();
        element.height = image.height();
        templateImageElements.push_back(element);

        QPainter painter;
        painter.begin(&background);
        painter.fillRect(background.rect(), Qt::transparent);
        painter.end();

        painter.begin(&image);
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
        QRectF target(element.posX, element.posY,
                      element.width, element.height);
        QRectF source(getSourceFromImage(element.image, target));

        painter.drawImage(target, element.image, source);

        //printf("Target: %f / %f / %f / %f\n", target.x(), target.y(), target.width(), target.height());
        //printf("Source: %f / %f / %f / %f\n", source.x(), source.y(), source.width(), source.height());
    }

    painter.drawImage(image.rect(), background);
    painter.end();
}

void Scene::save(const QString &_path)
{
    image.save(_path);
}

void Scene::saveSceneImages(const QString &_path, const QString &_ending)
{
    int i = 0;
    for (const ImageElement &element : imageElements)
    {
        element.image.save(_path + "_" + QString::number(i) + _ending);
        i++;
    }
}

void Scene::loadScene(const QString &_filePath)
{
    QFile scene(_filePath);
    QFileInfo sceneFileInfo(scene);

    if(!scene.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QXmlStreamReader xml(&scene);

    imageElements.clear();
    templateImageElements.clear();

    while(!xml.atEnd() && !xml.hasError())
    {
        QXmlStreamReader::TokenType token = xml.readNext();

        if(token == QXmlStreamReader::StartDocument)
            continue;

        if(token == QXmlStreamReader::StartElement)
        {
            if(xml.name() == "frame")
            {
                int width  = xml.attributes().value("width").toString().toInt();
                int height = xml.attributes().value("height").toString().toInt();
                image = QImage(width, height, QImage::Format_ARGB32);
                QFile backgroundFile(xml.attributes().value("background").toString());
                if(QDir::isRelativePath(backgroundFile.fileName()))
                {
                    QDir dir(sceneFileInfo.absoluteDir());
                    backgroundFile.setFileName(dir.absoluteFilePath(backgroundFile.fileName()));
                }
                background.load(backgroundFile.fileName());

                while(!(xml.name() == "frame" && token == QXmlStreamReader::EndElement))
                {
                    if(xml.name() == "image" && token == QXmlStreamReader::StartElement)
                    {
                        ImageElement ie;
                        ie.posX   = xml.attributes().value("x").toString().toFloat();
                        ie.posY   = xml.attributes().value("y").toString().toFloat();
                        ie.width  = xml.attributes().value("width").toString().toFloat();
                        ie.height = xml.attributes().value("height").toString().toFloat();
                        templateImageElements.insert(templateImageElements.begin(), ie);
                    }

                    token = xml.readNext();
                }
            }
        }
    }
}

bool Scene::addImage(QImage &_image)
{
    if(templateImageElements.size() < 1)
        return false;

    ImageElement element = templateImageElements.back();
    templateImageElements.pop_back();

    element.image = QImage(_image);

    imageElements.push_back(element);

    return true;
}

int Scene::imagesToAdd()
{
    return templateImageElements.size();
}

QImage Scene::getSceneImage() const
{
    return QImage(image);
}

QImage Scene::getPreviewImage() const
{
    QImage preview(image);

    QPainter painter;
    painter.begin(&preview);

    for (const ImageElement &element : imageElements)
    {
        painter.fillRect(QRectF(
                    element.posX,
                    element.posY,
                    element.width,
                    element.height),
                    Qt::black);
    }

    painter.drawImage(image.rect(), background);
    painter.end();

    return preview;
}

void Scene::setTemplateLocation(const QString &_templateLocation)
{
    templateLocation = _templateLocation;
}

QRectF Scene::getSourceFromImage(const QImage &_image, const QRectF &_target)
{
    float ratio = _target.width() / _target.height();

    if(_target.width() < _target.height())
    {
        float width = (float)_image.height() * ratio;

        return QRectF((_image.width() - width) / 2.0,
                      0.0f,
                      width, _image.height());
    }

    float height = (float)_image.width() / ratio;

    return QRectF(0.0f,
                  (_image.height() - height) / 2.0f,
                  _image.width(), height);
}
